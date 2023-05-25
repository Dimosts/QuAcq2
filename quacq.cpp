#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include "quacq.h"

using namespace std;

// Globals only for this file -----------------------------------------------------------------------

bool collapse = false; 

int bias_rej; // for findScope-2

double gen_time1, gen_time2;


// debugging vars ----------------------------------

int counter_return_bias = 0;
int counter_return_end = 0;
int counter_return_exists = 0;
int counter_return_answer = 0;

// ---------------------------

// -------------------------------------------------------------------------------------------------------


int cl_init = 0;
int bias_init = 0;
bool bk_flag = false;

int counter_queries_generated = 0;
int counter_queries_fs = 0;
int counter_queries_fc = 0;

int counter_red = 0;

Constraint_Net real_cl;

Constraint_Net quacq(){

	// file to write results ------------------

	string res_file = "results_";
	res_file += algorithm_string;
	res_file += "_";
	res_file += benchmark;

	ofstream res;
	res.open (res_file.c_str(), std::ofstream::out | std::ofstream::app);

	string queries_res_file = "results_queries_";
	queries_res_file += algorithm_string;
	queries_res_file += "_";
	queries_res_file += benchmark;

	ofstream qres;
	qres.open (queries_res_file.c_str(), std::ofstream::out | std::ofstream::app);

	// ----------------------------------------

	Constraint_Net cl;

	CL = &cl;

	cl.push_con(CL_init);

	Constraint_Net bias(VAR_NUM);

	B = &bias;

	bias.remove_existing(cl);

	Constraint con;

	bool answer;

	Query query;

	vector<short int> scope;

	vector<short int> vars;	// all the vars of the problem

	clock_t start_fs_time, end_fs_time;
	clock_t start_findc_time, end_findc_time;
	clock_t start_query_time, end_query_time;

	clock_t start_gen_time, end_gen_time;

	double tmp_time;

	double total_fs_time = 0;
	double total_findc_time = 0;

	double total_generating_time1 = 0, total_generating_time2 = 0;

	for ( int i = 0; i < VAR_NUM; i++ ){
		vars.push_back( i );
	}

	// main loop of quacq --------------------------------------------------------------------------------------------

	start_time = clock();	// starting time of MAC algorithm ( to end after one sec if it has found at least one query )

	while ( true ) {


		// for evaluation metrics ---
		if ( !answer ) {		// if previous answer was no

			start_time_pos = clock();
			counter_queries_conv = 0;

		}
		// --------------------------

	// Generate query -----------------------------------------------------------------------------

		cout << "Bias size: " << bias.get_size() << endl;
		cout << "cl size: " << cl.get_size() << endl;

		if ( counter_queries > 0 ){
			cout << "Queries: " << counter_queries << endl;
			cout << "Complete Queries = " << complete_queries << endl;
			cout << "Average Quwey Size = " << queries_size_sum/counter_queries << endl;
			cout << "Time per Query (secs) = " << f_time/counter_queries << endl;
			cout << "Total time for queries (secs) = " << f_time << endl;
			cout << "Total generating time1 = " << total_generating_time1 << endl;
			cout << "Total generating time2 = " << total_generating_time2 << endl;
			cout << "Total findScope time = " << total_fs_time << endl;
			cout << "Total findC time = " << total_findc_time << endl;
		}


		Solver mac;

		mac.set_cl(cl);

		if ( check_collapse ) {

			if ( mac.mac().get_state() == 2 ){	// collapsed 

				cout << "Collapsed 1............... " << endl;

				cl.clear();

				return cl;
			}
		
		}

		cout << "Genarate query" << endl;

start_gen_time = clock();

		if ( ge == false )
			query = generate_query(bias, cl);
		else if ( ge == true )
			query = generate_example(bias, cl);			

end_gen_time = clock();

tmp_time = (double) (end_gen_time - start_gen_time)/CLOCKS_PER_SEC;
total_generating_time1 += gen_time1;
total_generating_time2 += gen_time2;

end_query_time = clock();			// to measure time from a generated query to another

if ( counter_queries > 0 )
	tmp_time = (double) (end_gen_time - start_gen_time)/CLOCKS_PER_SEC;
else
	tmp_time = (double) (end_query_time - start_time)/CLOCKS_PER_SEC;

qres << counter_queries+1 << "\t" << tmp_time << "\t" <<  (double) (end_query_time - start_time)/CLOCKS_PER_SEC << "\t";
qres << counter_queries_generated << "\t" << counter_queries_fs << "\t" << counter_queries_fc << "\t";


start_query_time = clock();			// to measure time from a generated query to another

		if ( query.get_state() == 3 || query.get_state() == 5 ){	// converged

			real_cl.write("real_cl");
			cl.write("cl");
			bias.write("bias");

			if ( query.get_state() == 3 )
				cout << "Converged ............... " << endl;
			else 
				cout << "Prematurely Converged ............... " << endl;


			Solver mac2;

			mac2.set_cl(cl);
			mac2.initialize();
/*
		cout << "\n----------------------------\n";
			if ( mac2.mac().get_state() == 2 ){	// collapsed 

				cout << "Collapsed !!............... " << endl;


			} else 
				cout << "Not Collapsed ............... " << endl;

		cout << "\n----------------------------\n";
*/
			end_time = clock();

			total_time = (double) (end_time - start_time)/CLOCKS_PER_SEC;

			cout << "Total time (secs): " << total_time << endl;

			// write to file ---------------------------

			time_only_pos = (double) (end_time - start_time_pos)/CLOCKS_PER_SEC;	// calculate the time needed to converge -- AFTER -- L is learned

			res << Sol.get_size() << "\t";			// The size of the target network

			if ( background_percentage > 0 ){

				res << cl_init << "\t";
//				res << cl.get_size() - cl_init - counter_red << "\t";
//                    res << "\t";
				res << real_cl.get_size() - cl_init << "\t";

					res << bias_init << "\t";
			} else if ( CL_init.get_size() > 0 ){

				cl_init = CL_init.get_size();

                                res << cl_init << "\t";
//                              res << cl.get_size() - cl_init - counter_red << "\t";
				res << real_cl.get_size() << "\t";

			} else {
				res << 0 << "\t";
//				res << cl.get_size() - counter_red << "\t";			// the size of the learned network
				res << real_cl.get_size() << "\t"; // - counter_red << "\t";			// the size of the learned network
			}

			res << counter_queries - counter_queries_conv<< "\t";			// number of queries until learning L
			res << counter_queries << "\t";	// number of queries until convergence proof
			res << counter_queries_generated << "\t";	// number of queries generated
			res << counter_queries_fs << "\t";			// number of queries from findscope
			res << counter_queries_fc << "\t";			// number of queries from Findc
			res << queries_size_sum/counter_queries << "\t"; // average size of query
			//res << f_time << "\t";							// total time for queries ( T_queries )
			res << total_time - time_only_pos << "\t";			// time until L is learned
			res << total_time << "\t";						// total time(T_total)
			res << f_time/counter_queries << "\t";	// average time per query
			res << max_query_time << "\t";					// max time of query
//			res << complete_queries << "\t";				// number of complete queries


			if ( query.get_state() == 3 )
				res << 0;		// convergence
			else 
				res << 1;		// premature convergence

//			res << "\t" << real_cl.get_size();

//			res << "\t";

//			res << time_only_pos;

//			res << "\t";
			

			//res << "\t";

			//res << B->get_size() << "\t";					// final bias size
/*
			int ctInB = 0;
			bool tmp_flag;

			for (int i = 0; i < Sol.get_size(); i++){
				
				tmp_flag = false;
				for (int j = 0; (j < B->get_size()) && !tmp_flag; j++){

					if ( Sol.get_con(i).compare(B->get_con(j)) ){
						ctInB++;
						tmp_flag =true;
					}

				}
			}

			res << ctInB << "\t";				// ct in bias when (prematurely) converged
*/

            //            res << "\t";

			//res << remove_counter1;

            //            res << "\t";

			//res << remove_counter2;

			//res << "\t";

			//res << counter_findc;

			res << endl;

			// -----------------------------------------

			return cl;
		}

	// Ask query and update cl and bias -----------------------------------------------------------

		answer = query.ask();
		counter_queries_generated++;
		

		qres << answer << endl;
//int in;
//cin >> in;
		if ( pos_query ) {

			cout << "Found Solution ............... " << endl;

			res << cl.get_size() << "\t";			// the size of the learned network
			res << counter_queries << "\t";			// number of queries
			res << f_time << "\t";							// total time for queries ( T_queries )
			res << endl;

			return cl;

		} else if ( answer ){
			bias.remove_rejects(query);
		} else {

			bias_rej = bias.count_rejects(query);

			random_shuffle( vars.begin(), vars.end() );

start_fs_time = clock();

			if ( fs2 )
				scope = find_scope2(query, scope, vars, false, bias);
			else if ( fs3 )
				scope = find_scope3(query, scope, vars, false, bias);				
			else
				scope = find_scope(query, scope, vars, false, bias);

end_fs_time = clock();

tmp_time = (double) (end_fs_time - start_fs_time)/CLOCKS_PER_SEC;
total_fs_time += tmp_time;

start_findc_time = clock();

			// FindC -----------------------------------------------
			if ( fc == 0 ) {				

				con = find_con( query, scope, bias, cl );

				if ( con.get_type() == -1 )	// then collapse
					collapse = true;
				else{
					real_cl.push_con(con);
					cl.push_con(con);		
				}

			} else if (fc == 1)
				collapse = !findC( query, scope, bias, cl );

			if ( collapse ){
			
				cout << "Collapsed 2............... " << endl;

				//cl.print();
				cl.clear();

				return cl;

			}
			// -----------------------------------------------------

end_findc_time = clock();

tmp_time = (double) (end_findc_time - start_findc_time)/CLOCKS_PER_SEC;
total_findc_time += tmp_time;

			scope.clear();

// for the percentage background knowledge -------------
if ( background_percentage > 0 && bk_flag == false && round(background_percentage*Sol.get_size()) <= real_cl.get_size() )   {

	bk_flag = true;

        cl_init = real_cl.get_size();

	Constraint_Net bias22(VAR_NUM);

	bias = bias22;

	bias_init = bias.get_size();

        counter_queries = 0;                 // number of queries
        queries_size_sum = 0; // average size of query
        max_query_time = 0;                                  // max time of query
        complete_queries = 0;                                // number of complete queries

		counter_queries_fc = 0;
		counter_queries_fs = 0;
		counter_queries_generated = 0;

        start_time = clock();

}


		}

	}

} 	// end of quacq() algorithm


// Generate query 1
Query generate_query(Constraint_Net &bias, Constraint_Net &cl){

	Constraint_Net bias2;

	Solver mac;

	mac.set_cl(cl);

	mac.set_bias(bias);

	Query query;

	query.set_state(1);

	query = mac.generate_query();

	Constraint c;

	if ( query.get_state() == 1 || query.get_state() == 4 )	// found query
		return query;
	else if ( query.get_state() == 3 ){	// converged

		cout << "Converged ............... ok" << endl;

		return query;

	}  else if ( query.get_state() == 5 || query.get_state() == 2 ){ // not found in T secs

		cout << "Prematurely converged ............... ok" << endl;
		query.set_state(5);

		return query;		

	} 
	
	query.set_state(3);	// converged

	return query;

} // end of generate query 1

// GenerateExample
Query generate_example(Constraint_Net &bias, Constraint_Net &cl){

	Constraint_Net bias2;

	Constraint con;

	Solver mac;

	mac.set_cl(cl);

	Query query;

	query.set_state(1);

	bool query_flag = false;

	vector<short int> scope;

	clock_t start_time2, end_time2;	// to run for 1 sec

	clock_t start_gen_time1, end_gen_time1;
	clock_t start_gen_time2, end_gen_time2;

	gen_time1 = 0;
	gen_time2 = 0;

	double time = 0;

	int i;

	start_time2 = clock();	// starting time

	while( bias.get_size() > 0 ) {

		start_gen_time1 = clock();	// starting time

		// pick S s.t. there exists c_S in B
		scope = bias.get_con(0).get_scope().get_distinct_variables();

		// C \leftarrow every c_S in B
		bias2.clear();

		bias2.push_con( bias.get_scope(scope) );

		query_flag = false;

		mac.set_bias(bias2);

bias2.print();
		mac.initialize();
		// ---------------------------

		// solve (L[S] ^ \lnot C)
		query = mac.generate_query(scope);	// generate a query with the defined L and B
		
		end_gen_time1 = clock();

		gen_time1 += (double) (end_gen_time1 - start_gen_time1)/CLOCKS_PER_SEC;

		time += gen_time1;

		cout << "line 3 ..." << endl;

		if ( query.get_state() == 3 ){

			if (cl.get_scope(scope).get_size() == 0){
				cl.push_con(bias.get_scope(scope));
				counter_red += bias.get_scope(scope).get_size();
			}

			remove_counter1 +=  bias.get_scope(scope).get_size();

			bias.remove_scope(scope);

		} else {

			start_gen_time2 = clock();	// starting time

			Query query2;
		
			cout << "Query found, trying to expand it" << endl;

			mac.set_bias(bias);

			mac.initialize();

			end_time2 = clock();

			time = (double) (end_time2 - start_time2)/CLOCKS_PER_SEC;

			if ( (time_bound - time) < (cutoff - gen_time1) )
				query2 = mac.generate_query(query, time_bound - time);
			else 
				query2 = mac.generate_query(query, cutoff - gen_time1);

			end_gen_time2 = clock();

			gen_time2 += (double) (end_gen_time2 - start_gen_time2)/CLOCKS_PER_SEC;

			if ( query2.get_state() == 0 || query2.get_state() == 1 || query2.get_state() == 4 )	// found query
				return query2;
			else if ( query2.get_state() == 3 ){	// proved that no solution exists

				cl.push_con(bias.get_scope(scope));
				counter_red += bias.get_scope(scope).get_size();

	                        remove_counter2 +=  bias.get_scope(scope).get_size();

				bias.remove_scope(scope);


			} else if ( query2.get_state() == 2 || query2.get_state() == 5 ) // not found in T secs
				return query;

		}


// for the percentage background knowledge -------------
if ( bk_flag == false && background_percentage > 0 && round(background_percentage*Sol.get_size()) <= real_cl.get_size() )   {

	bk_flag = true;

        cl_init = real_cl.get_size();
        bias_init = bias.get_size();

        counter_queries = 0;                 // number of queries
        queries_size_sum = 0; // average size of query
        max_query_time = 0;                                  // max time of query
        complete_queries = 0;                                // number of complete queries

        start_time = clock();

}

		cout << "." << flush;		// One dot for each Scope tried.		

		end_time2 = clock();

		time = (double) (end_time2 - start_time2)/CLOCKS_PER_SEC;

	}


	query.set_state(3);	// converged
	
	return query;

} // generate_example()

vector<short int> find_scope(Query query, vector<short int> R, vector<short int> Y, bool flag, Constraint_Net &bias){

	int answer;

	Query partial_query;

	vector<short int> Y1, Y2, X;

	vector<short int> S1, S2;

	int i;

	if ( flag ){

		// ask partial query for variables R

		partial_query = query.partial( R );

		answer = partial_query.ask();

		if ( answer )
			bias.remove_rejects( partial_query );
		else 
			return S1;	// return empty vector of vars		

	} 

	if ( Y.size() == 1 )
		return Y;


	// Y -> < Y1, Y2 > with |Y1| = [ |Y| / 2 ]
	for (i = 0; i < Y.size(); i++){

		if ( i < (Y.size()/2) )
			Y1.push_back(Y[i]);
		else
			Y2.push_back(Y[i]);

	}

	// R U Y1
	X.insert( X.end(), R.begin(), R.end() );
	X.insert( X.end(), Y1.begin(), Y1.end() );

	S1 = find_scope(query, X, Y2, true, bias);

	X.clear();

	// R U S1
	X.insert( X.end(), R.begin(), R.end() );
	X.insert( X.end(), S1.begin(), S1.end() );	

	S2 = find_scope(query, X, Y1, S1.size() != 0, bias);

	X.clear();

	// S1 U S2
	X.insert( X.end(), S1.begin(), S1.end() );
	X.insert( X.end(), S2.begin(), S2.end() );	
	
	return X;

}	// end of function find_scope()

vector<short int> find_scope2(Query query, vector<short int> R, vector<short int> Y, bool flag, Constraint_Net &bias){

	int answer;

	Query partial_query;

	vector<short int> Y1, Y2, X;

	vector<short int> S1, S2;

	Constraint con;

	int var1, var2;

	int i;

	bool flag_con;

	if ( flag ){

		// ask partial query for variables R

		partial_query = query.partial( R );

		if ( bias.rejects(partial_query) ){

			if ( bias.count_rejects(partial_query) != bias_rej ){

				answer = partial_query.ask();


				if ( answer == true )					
					bias.remove_rejects( partial_query );
				else if ( answer == false ){

					bias_rej = bias.count_rejects(partial_query);

					return S1;	// return empty vector of vars		
		
				} 

			} else
				return S1;	// return empty vector of vars		

		}

	} 

	if ( Y.size() == 1 )
		return Y;


	// Y -> < Y1, Y2 > with |Y1| = [ |Y| / 2 ]
	for (i = 0; i < Y.size(); i++){

		if ( i < (Y.size()/2) )
			Y1.push_back(Y[i]);
		else
			Y2.push_back(Y[i]);

	}

	// R U Y1
	X.insert( X.end(), R.begin(), R.end() );
	X.insert( X.end(), Y1.begin(), Y1.end() );

	S1 = find_scope2(query, X, Y2, true, bias);

	X.clear();

	// R U S1
	X.insert( X.end(), R.begin(), R.end() );
	X.insert( X.end(), S1.begin(), S1.end() );	

	S2 = find_scope2(query, X, Y1, S1.size() != 0, bias);

	X.clear();

	// S1 U S2
	X.insert( X.end(), S1.begin(), S1.end() );
	X.insert( X.end(), S2.begin(), S2.end() );	
	
	return X;

}	// end of function find_scope2()

vector<short int> find_scope3(Query query, vector<short int> R, vector<short int> Y, bool flag, Constraint_Net &bias){

	bool answer;

	Query partial_query, partial_query2;

	vector<short int> Y1, Y2, X, X2;

	vector<short int> S1, S2;

	int i;

	if ( flag ){

		// ask partial query for variables R

		partial_query = query.partial( R );

		if ( bias.rejects(partial_query) ){

			answer = partial_query.ask();
			counter_queries_fs++;
			
			if ( answer )
				bias.remove_rejects( partial_query );
			else 				return S1;	// return empty vector of vars		

		}

	} 

	if ( Y.size() == 1 )
		return Y;


	// Y -> < Y1, Y2 > with |Y1| = [ |Y| / 2 ]
	for (i = 0; i < Y.size(); i++){

		if ( i < (Y.size()/2) )
			Y1.push_back(Y[i]);
		else
			Y2.push_back(Y[i]);

	}

	// R U Y1
	X.insert( X.end(), R.begin(), R.end() );
	X.insert( X.end(), Y1.begin(), Y1.end() );

	partial_query = query.partial( X );

	// R U Y
	X2.insert( X2.end(), R.begin(), R.end() );
	X2.insert( X2.end(), Y.begin(), Y.end() );

	partial_query2 = query.partial( X2 );

	if ( bias.count_rejects( partial_query ) < bias.count_rejects( partial_query2 ) )
		S1 = find_scope3(query, X, Y2, true, bias);

	X.clear();

	// R U S1
	X.insert( X.end(), R.begin(), R.end() );
	X.insert( X.end(), S1.begin(), S1.end() );	

	partial_query = query.partial( X );

	if ( bias.count_rejects( partial_query ) < bias.count_rejects( partial_query2 ) )
		S2 = find_scope3(query, X, Y1, S1.size() != 0, bias);

	X.clear();

	// S1 U S2
	X.insert( X.end(), S1.begin(), S1.end() );
	X.insert( X.end(), S2.begin(), S2.end() );	
	
	return X;

}	// end of function find_scope3 -----------

// ------------------------------------------------------------------------------------------------------------------------------------------------------

// find the constraint that is violated (ijcai version)
Constraint find_con(Query query, vector<short int> Y, Constraint_Net &bias, Constraint_Net cl){	

	Constraint_Net D;

	Constraint temp;	// to compare scopes

	temp.set_scope(Y);	// to compare scope

	Constraint c;

	int r;

	int var1, var2;

	int answer;	// user's answer to query

	// initialize constraint network D	------------------------------------------

	for (int i = 0; i < bias.get_size(); i++){	// for each constraint in bias

		Constraint con = bias.get_con(i);

		if ( con.compare_scope( temp ) ){	// check only constraints with scope Y

			var1 = con.get_var(0);
			var2 = con.get_var(1);

			if ( !con.satisfy( query ) )
				D.push_con(con);
			
		}

	}

//	cout << "D: ";
//	D.print();

	// main loop of findC -------------------------------------------------------------

	while ( true ) {

		if ( D.get_size() == 0 )
			return c;	// empty constraint to collapse
		else if ( D.get_size() == 1 ){

			c = D.get_con(0);

			bias.remove_con(c);

			return c;
		}

	// Generate query -----------------------------------------------------------------------------

		Query temp_query;

		Solver mac;

		mac.set_cl(cl);

		mac.set_bias(D);

		temp_query = mac.generate_query(Y);

		// if no constraint, or all constraints, of D are rejecting the query
		if ( temp_query.get_state() == 3 || temp_query.get_state() == 4 ){

			if ( D.get_size() == 0 )
				return c;	// empty constraint to collapse
			else {

				r = ( rand() % D.get_size() );	// randomely pick one constraint of D to return

				c = D.get_con(r);

				for (int i=0; i < D.get_size(); i++ ){
					bias.remove_con(D.get_con(i));
				}

//	cout << "D: ";
//	D.print();

				D.clear();

				return c;

			}


		}

	// Ask query and update D and bias -----------------------------------------------------------
		answer = temp_query.ask();

		if ( answer == true ){	// If classified positive, all constraints rejecting it are removed from bias and D

			bias.remove_rejects( temp_query );
			D.remove_rejects( temp_query );
			
		} else
			D.remove_accepts( temp_query );

	}

}	// end of function find_con()

// find the constraint that is violated (aij version with \Delta_p)
bool findC(Query query, vector<short int> Y, Constraint_Net &bias, Constraint_Net &cl){	

	Constraint_Net D, Delta_p;

	Constraint_Net kD, temp_net;

	Constraint temp;	// to compare scopes

	temp.set_scope(Y);	// to compare scope

	Constraint c;

	int p, max_p;	// for the creation of Delta_p

	int r;		// for the generation of a random number

	bool answer;	// user's answer to query

int in;

//	temp.get_scope().println();

	// initialize constraint network D to By  -----------------------------------------
	for (int i = 0; i < bias.get_size(); i++){	// for each constraint in bias

		Constraint con = bias.get_con(i);

		if ( con.compare_scope( temp ) ){	// check only constraints with scope Y

			D.push_con(con);
			
		}

	}

//	D.print();

	// conjunction with kD(ey) ---------------------------------------------------------
	kD = D.get_rejects(query);	// kD(ey)

	
//cout << "kD size: " << kD.get_size() << endl;

	D.joint(kD);

//	D.print();

if ( kD.get_size() == 0 )
	cin >> in;

	// main loop of findC -------------------------------------------------------------

	while ( true ) {

	// Generate query -----------------------------------------------------------------------------

		Query temp_query;

		Solver solver;

		solver.set_cl(cl);

		solver.set_bias(D);

		p = 1;

		max_p = D.get_max_conjunction_size();

		do{

cout << "p = " << p << endl;

			// create Delta_p -----------------------------
			Delta_p.clear();

			for(int i = 0; i < D.get_size(); i++){

				c = D.get_con(i);
				
				if ( c.get_conjunction_size() == p  )
					Delta_p.push_con(c);

			}
			// --------------------------------------------

			if ( Delta_p.get_size() > 0)
				temp_query = solver.generate_findc_query(Y, Delta_p);
			else
				temp_query.initialize();
			
			p++;
		} while ( temp_query.get_state() == 0 && p <= max_p );

	// --------------------------------------------------------------------------------------------

	// found constraint(s) ------------------------------------------------------------------------

		// if no constraint, or all constraints, of D are rejecting the query
		if ( temp_query.get_state() == 0){

//			D.print();

			if ( D.get_size() == 0 )	
				return false;	// to collapse
			else {

				p = D.get_min_conjunction_size();

				// create Delta_p to pick a constraint---------
				Delta_p.clear();

				for(int i = 0; i < D.get_size(); i++){

					temp = D.get_con(i);
					
					if ( temp.get_conjunction_size() == p  )
						Delta_p.push_con(temp);

				}
				// --------------------------------------------				

				r = ( rand() % Delta_p.get_size() );	// randomely pick one constraint of Delta_p to return

				c = Delta_p.get_con(r);

				D.clear();
				Delta_p.clear();

				for (int i = 0; i < c.get_conjunction_constraints().size(); i++){
					cl.push_con( c.get_conjunction_constraint(i) );
					real_cl.push_con( c.get_conjunction_constraint(i) );
					counter_findc++;
				}

				// remove every c_Y from B ------------------------------------------------------
				bias.remove_scope(Y);

				return true;
			}

		}

	// --------------------------------------------------------------------------------------------

	// Ask query and update D and bias ------------------------------------------------------------

		answer = temp_query.ask();
		counter_queries_fc++;

		if ( answer ){	// If classified positive, all constraints rejecting it are removed from bias and D

			D.remove_rejects( temp_query );
		
		} else {

			// call again findScope and findC to find the subset constraints -------------
				vector<short int> scope;

				if ( fs2 )
					scope = find_scope2(temp_query, scope, Y, false, bias);
				else if ( fs3 )
					scope = find_scope3(temp_query, scope, Y, false, bias);
				else 
					scope = find_scope(temp_query, scope, Y, false, bias);				
								

				if ( scope.size() < Y.size() ){
					
					if ( !findC( temp_query, scope, bias, cl ) ) // if false is returned
						return false;
					
				} else
					D.joint( D.get_rejects( temp_query ) );
					
			// ---------------------------------------------------------------------------
			
		}

	}

}	// end of function findC5()

