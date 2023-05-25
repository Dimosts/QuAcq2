#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

#include "solver.h" 

#define INF 1000000

using namespace std;

// Class Solver methods -------------------------------------------------------------------------------

	// constructors ----------------------------------

Solver::Solver(){	// default: size of variable array : VAR_NUM 
		
	for (int i = 0; i < VAR_NUM; i++){

		Variable var(i);

		variable.push_back(var);

	}

	StatesDomains = new bool*[VAR_NUM];

	for( int i = 0; i < VAR_NUM; i++){
		
		StatesDomains[i] = new bool[VAR_NUM];

		for( int j = 0; j < VAR_NUM; j++){
			StatesDomains[i][j] = false;
		}

	}

}

Solver::Solver(vector<Variable> v){

	variable = v;

	StatesDomains = new bool*[VAR_NUM];

	for( int i = 0; i < VAR_NUM; i++){
		
		StatesDomains[i] = new bool[VAR_NUM];

		for( int j = 0; j < VAR_NUM; j++){
			StatesDomains[i][j] = false;
		}

	}
	
}	

Solver::Solver(Constraint_Net c, Constraint_Net b){	// default: size of variable array : VAR_NUM 
	
	for (int i = 0; i < VAR_NUM; i++){

		Variable var(i);

		variable.push_back(var);

	}

	StatesDomains = new bool*[VAR_NUM];

	for( int i = 0; i < VAR_NUM; i++){
		
		StatesDomains[i] = new bool[VAR_NUM];

		for( int j = 0; j < VAR_NUM; j++){
			StatesDomains[i][j] = false;
		}

	}

	set_constraints(c, b);

}

Solver::Solver(vector<Variable> v, Constraint_Net c, Constraint_Net b){

	variable = v;

	StatesDomains = new bool*[VAR_NUM];

	for( int i = 0; i < VAR_NUM; i++){
		
		StatesDomains[i] = new bool[VAR_NUM];

		for( int j = 0; j < VAR_NUM; j++){
			StatesDomains[i][j] = false;
		}

	}

	set_constraints(c, b);

}

void Solver::initialize(){

	for (int i = 0; i < variable.size(); i++){
		variable[i].initialize();
	}

	vars_queue.clear();

	used_vars_queue.clear();

	bias_rejects.clear();

	bias_checked.clear();

	bias_remain.clear();

	set_constraints(cl, bias);

}

void Solver::initialize(vector<Variable> v){

	variable = v;

	vars_queue.clear();

	used_vars_queue.clear();

	bias_rejects.clear();

	bias_checked.clear();

	bias_remain.clear();

	set_constraints(cl, bias);

}

void Solver::initialize(Constraint_Net c, Constraint_Net b){

	for (int i = 0; i < variable.size(); i++){
		variable[i].initialize();
	}

	vars_queue.clear();

	used_vars_queue.clear();

	bias_rejects.clear();

	bias_checked.clear();

	bias_remain.clear();

	set_constraints(c, b);

}

	// set -------------------------------------------

void Solver::set_variables(vector<Variable> v){	// set all variables
	variable = v;
}

void Solver::set_variable(Variable v, int i){	// set variable i
	variable[i] = v;
}

void Solver::set_variable_value(int v, int i){	// set value v to variable i
	variable[i].set_value(v);
}

void Solver::set_variable_domain(Domain d, int i){		// set domain d to variable i
	variable[i].set_domain(d);
}

void Solver::set_variable_domain_state(int state, int var, int dom){	// set variable's var, domain value dom, to state state
	variable[var].set_domain_state(state, dom);
}

void Solver::set_cl(Constraint_Net c){

	cl = c;

	// for each constraint, push it to its variables related constraints
	for (int i = 0; i < cl.get_size(); i++){	
		for (int j = 0; j < cl.get_con(i).get_scope_size(); j++){
			variable[ cl.get_con(i).get_var(j) ].push_con(i); 	
		}
	}

}

void Solver::set_bias(Constraint_Net b){

	bias = b;

	bias_remain.clear();

	// for each bias constraint, push it to its variables related constraints
	for (int i = 0; i < bias.get_size(); i++){	

		bias_remain.push_back(0);

		vector<short int> scope = bias.get_con(i).get_scope().get_distinct_variables();

		for (int j = 0; j < scope.size(); j++){
			variable[ scope[j] ].push_bias_con(i); 	
		}

	}

}

void Solver::set_constraints(Constraint_Net c, Constraint_Net b){ 

	set_cl(c);
	set_bias(b);

}

void Solver::increase_con_weight(int c){
	cl.increase_con_weight(c);
}


	// get -------------------------------------------

vector<Variable> Solver::get_variables(){		// return all variables
	return variable;
}

Variable Solver::get_variable(int i){		// return variable i
	return variable[i];
}

int Solver::get_variable_value(int i){		// get variable i value
	return variable[i].get_value();
}

Domain Solver::get_variable_domain(int i){	// get value of domain value dom, of variable var
	return variable[i].get_domain();
}

int Solver::get_variable_domain_state(int var, int dom){	// get state of domain value dom, of variable var
	return variable[var].get_domain_state(dom);
}

int Solver::get_variable_domain_value(int var, int dom){	// get value of domain value dom, of variable var
	return variable[var].get_domain_value(dom);
}

Constraint_Net Solver::get_cl(){			// return cl
	return cl;
}

Constraint_Net Solver::get_bias(){			// return bias
	return bias;
}

int Solver::get_con_weight(int c){		// return con c weight
	return cl.get_con_weight(c);
}

	// other methods ---------------------------------

void Solver::print_variables(){

	cout << endl << "The variables of the problem are: " << endl << endl;

	for (int i = 0; i < variable.size(); i++){
		cout << "Variable " << i << ": " << endl;
		variable[i].print();
	}

}

bool Solver::gac(vector<short int> scope){
	return gac(-2, scope);
}

bool Solver::gac(int state, vector<short int> scope){		// gac algorithm for arc consistency
															// state: For mac algorithm, which var's assignation deleted these domain values
															// run for all variables in vector scope

	int i,j;  // for the loops
	int var_size = scope.size();	

	bool del; // if revise deleted any value

	vector<int> vars;	// vector vars: the queue of gac

	int var;	// the variable to operate (extracted from the queue vars)
	int var2;	// the variable that has constraint with var

	int c;	// the index of a constraint

	Constraint con;

	bool flag = true;	// if gac failed or not

	bool found;

	int var_pos;	// to store the position of a var in the scope of the constraint

	vector<short int> distinct_variables;	// distinct variables of a scope

	if ( state < 0 ) {	// if it is a call outside - before mac

		// initialize vector vars with all variables of scope
		for (i = 0; i <	var_size; i++){
			vars.push_back( scope[i] );
			variable[ scope[i] ].set_in_queue(true);
		}

	} else {		// if it is a call after the assignation of a value

		for (i = 0; i <	var_size; i++){
			variable[ scope[i] ].set_in_queue(false);
		}

		vars.push_back(state);	// initialize only with the variable that has been assigned a value
		variable[state].set_in_queue(true);

	}

	// gac main loop
	while ( vars.size() > 0 && flag ) {

		// fifo : take first element of vector and erase it
		var = vars[0];
		vars.erase(vars.begin());

		variable[var].set_in_queue(false);

		// for every constraint that var is involved
		for (i = 0; i < variable[var].get_con_size() && flag; i++){

			var_pos = -1; 	// until we find the position of the var in the scope of the constraint

			c = variable[var].get_con(i);
			con = cl.get_con(c);

			if ( con.get_type() >= 0 && (con.get_type() > 23 || con.get_type() < 18 )){

				found = true;
				distinct_variables = con.get_scope().get_distinct_variables();

				// find the position of the variable in the scope of the constraint
				// Also check if all the variables in the scope of the constraint belong to the scope given
				for (j = 0; j < distinct_variables.size() && found; j++){
					
					if ( distinct_variables[j] == var )
						var_pos = j;
					else {
						
						if ( find(scope.begin(), scope.end(), distinct_variables[j] ) == scope.end() ) {
							found = false;
						}

					}

				}


				// call revise -------------------------------------------------------------------

				if ( found ) {	// if all the variables in the scope of the constraint belong to the scope given

					for (j = 0; j < distinct_variables.size() && flag; j++){

						del = false;

						if ( j != var_pos && variable[ distinct_variables[j] ].get_value() == wrong ) { // check all the neighboors of the variable in var_pos
																					//  if it already has been assigned a value then no domain value has to be deleted
							del = revise(con, j, state);
							var2 = con.get_var(j);

							if ( del ){	// if at least one value was deleted from the domain of variable i

								if ( variable[var2].get_domain_size() > 0 ){

									// if variable i is not already in queue
									if ( !variable[var2].get_in_queue() ) {		
										variable[var2].set_in_queue(true);		// put it in the queue
										vars.push_back(var2);
									}

								} else {

									flag = false;	// if a domain is empty then fail !

									cl.increase_con_weight(c);	// increase the weight of constraint c if failed

								}
							}

						}

					}
				
				}

			} else if ( state == -2 ) {

				int value, var_domain_size = variable[ var ].get_domain_values().size();

				for (int d = 0; d < var_domain_size; d++){		// for each domain value of the variable we check

					if ( variable[ var ].get_domain_state(d) == -1 ){	// if it has not already been deleted
			
						value = variable[ var ].get_domain_value(d);

						if ( !con.satisfy( value ) ){
							variable[ var ].set_domain_state(state, d);
						
						}

					} 
					
				}

			}
		}

	}

	return flag;

}	// end of method gac()

bool Solver::revise(Constraint con, int var_pos, int state){	// revise function for gac algorithm

	int i, j, v;  // for the loops

	bool del = false; // if revise deleted any value

	bool support;	// if a domain value is supported

	vector<short int> distinct_variables = con.get_scope().get_distinct_variables();

	int scope_size = distinct_variables.size();


	// new arrays for the backtracking search for support
	int *var = new int[ scope_size ];
	int *value = new int[ scope_size ];
	int *var_domain_size = new int[ scope_size ];
	
	int *dom_pos = new int[ scope_size - 1 ];
	int *vars_q = new int[ scope_size - 1 ]; // the other variables from the scope, except the one we are operating

	int identifier = 0;

	for (i = 0; i < scope_size; i++){

		value[i] = wrong;
		var[i] = distinct_variables[i];

		if ( variable[ var[i] ].get_value() == wrong )
			var_domain_size[i] = variable[ var[i] ].get_domain_values().size();		
		else
			var_domain_size[i] = 1;

		if ( i != var_pos ){	// save the other variables from the scope, except the one we are operating
			vars_q[identifier] = i;
			identifier++;		
		}

	}

	for (i = 0; i < var_domain_size[ var_pos ]; i++){		// for each domain value of the variable we check

		if ( variable[ var[var_pos] ].get_domain_state(i) == -1 ){	// if it has not already been deleted

			// start of backtracking search of support ----------------------------------------------------

			support = false;	// initialize to false and check

			value[ var_pos ] = variable[ var[var_pos] ].get_domain_value(i);


			identifier = 0; //	begin with vars_q[0]			

			for (j = 0; j < (scope_size - 1); j++)	// begin from domain value 0 for every variable
				dom_pos[j] = 0;

			// backtracking search until we find a support or searched all the combinations
			while ( !support && identifier >= 0 ){

				if ( identifier < (scope_size-1) ) {		// if we have not selected a value for all the variables
									// (scope_size -1) <- vars_q.size()

					// if we searched all the domain values of the variable vars_q[identifier]
					while ( dom_pos[identifier] == var_domain_size[ vars_q[identifier] ] ) {

						// backtrack ------
						dom_pos[identifier] = 0;
						identifier--;

					}

					if ( identifier >= 0 ){

						// if the variable we are operating is instantiated
						if ( variable[ var[ vars_q[identifier]]].get_value() != wrong ){

							value[ vars_q[identifier] ] = variable[ var[ vars_q[identifier]]].get_value();
							dom_pos[identifier]++;
							identifier++;
						
						} else {	// if the variable we are operating is uninstantiated

							// if the current domain value is not already deleted
							if ( variable[ var[ vars_q[identifier]]].get_domain_state( dom_pos[identifier] ) == -1 ){

								value[ vars_q[identifier] ] = variable[ var[ vars_q[identifier]]].get_domain_value( dom_pos[identifier] );

								dom_pos[identifier]++;
								identifier++;

							} else {
								dom_pos[identifier]++;
							}

						}
					
					}

				} else {						// if we have selected a value for all the variables

					vector<int> e;

					for (int x = 0; x < VAR_NUM; x++)
						e.push_back(wrong);
					

					for (int x = 0; x < distinct_variables.size(); x++)		// for the distinct variables of the scope
						e[distinct_variables[x]] = value[x];
			

					if ( con.satisfy(e) )
						support = true;

					dom_pos[identifier] = 0;
					identifier--;

				}

			}

			if ( !support ){

				del = true;
				variable[ var[var_pos] ].set_domain_state(state, i);

				if ( state >= 0 )
					StatesDomains[state][var[var_pos]] = true;

			}
		}

	}
			// end of backtracking search of support ------------------------------------------------------

	return del;

}	// end of function revise() of gac 

Query Solver::generate_query(){	// if no arguments then run for all variables

	vector<short int> scope;

	for (int i = 0; i < VAR_NUM; i++){
		scope.push_back(i);
	}

	return generate_query(scope);

}

Query Solver::generate_query(Query query){	// save the predefined values and try to expand the query given (run for the rest of the variables)

	return generate_query(query, time_bound);
}

Query Solver::generate_query(Query query, double bound){	// save the predefined values and try to expand the query given (run for the rest of the variables)

int in;

	bool gac_flag;

	clock_t start_time, end_time;	// to run for 1 sec

	vector<short int> scope, scope_arc;

	double time;

	start_time = clock();	// starting time

	for (int i = 0; i < VAR_NUM; i++){
		
		variable[i].set_value(query.get_var(i));

		if ( variable[i].get_value() != wrong ){

			scope_arc.push_back(i);

		} 
		
		scope.push_back(i);

	}

	query.print();

	cout << "Expand ..." << endl;

	end_time = clock();

	time = (double) (end_time - start_time)/CLOCKS_PER_SEC;

	query = generate_query(scope, bound - time);

	return query; 

//	return generate_query(scope);

}

Query Solver::generate_query(vector<short int> scope){

	if ( !sol_opt ){

// for line 3 of Generate Example
		sol_opt = true;

		Query query = generate_query(scope, INF);

		sol_opt = false;

		return query;

	} else{

		return generate_query(scope, time_bound);

	}

}

Query Solver::generate_query(vector<short int> scope, double bound){	// generate query with only with variables in scope in maximum time = bound

	int var_index, i, j;	// var_index: index for variables

	int first_var;

	int domain_pos;	// index of domain - var.

	bool gac_flag ;

	bool bias_rej_flag ;	// if we can maximize best_bias_rej

	clock_t start_time, end_time;	// to run for 1 sec

	double time;

	int bias_rej;	// how many constraints of the bias are rejecting the query

	int sum_rejects = 0;	// counter of bias constraints violated 

	int bias_to_check = bias.get_size();	// counter of bias constraints left to check ( bias.get_size() - bias.checked() )

	int best_bias_rej = 0;	// max bias cons violated by a query  

	int best_vars_inst = 0;	// max vars instantiated

	Query query, best_query;

	int nodes_visited = 1;

	start_time = clock();	// starting time

cout << "scope: " << endl;

	for (i = 0; i < variable.size(); i++){
		query.set_var(variable[i].get_value(), i);
		variable[i].set_value(wrong);
	}

	gac_flag = gac(scope);	// gac algorithm in the beginning


	// if given with some variables instantiated -----------------
	for (i = 0; i < scope.size(); i++){

cout << scope[i] << ", ";

		variable[scope[i]].set_value(query.get_var(scope[i]));				


		if ( query.get_var(scope[i]) != wrong ){	// If a variable is already instantiated
													// count the bias rejects and bias checked

			gac_flag = gac(scope[i], scope);	

			if ( gac_flag ) {

				bias_rej = count_bias_rejects(scope[i]);
				sum_rejects += bias_rej;
				bias_to_check -= bias_checked.back();

				used_vars_queue.push_back(scope[i]);	// push it in used vars


				// increase the instantiated variables number for the constraints it is involved
				for(int c = 0; c < variable[scope[i]].get_bias_con_size(); c++){	

					bias.increase_inst_vars(variable[scope[i]].get_bias_con(c));

					for (int k = 0; k < bias.get_con( variable[scope[i]].get_bias_con(c) ).get_scope_size(); k++){

						if ( bias.get_con( variable[scope[i]].get_bias_con(c) ).get_scope_vars()[k] != scope[i] )
							variable[ bias.get_con( variable[scope[i]].get_bias_con(c) ).get_scope_vars()[k] ].increase_bwdeg();
					}

				}


			} else {


				variable[scope[i]].set_value(wrong);				

				restore_domain(scope[i]);	// restore previous changes to domains to continue

				vars_queue.push_back(scope[i]);

			}

		} else {

			// Initialize Variables queue -----------
			vars_queue.push_back(scope[i]);
			
		}	

	}

cout << endl;


	for (i = 0; i < variable.size(); i++)
		query.set_var(variable[i].get_value(), i);

	best_query = query;
	best_bias_rej = sum_rejects;
	// -----------------------------------------------------------

	// get first variable -------------------
	if ( var_heuristic == 2 ) {	// if var_heuristic is wdeg or dom / wdeg

		for (i = 0; i < vars_queue.size(); i++){	// calculate wdeg for all the variables
			calc_wdeg(vars_queue[i]);
		}

	}

	var_index = next_var();	// get next variable with max wdeg

	first_var = var_index;

	// --------------------------------------

	// main loop of mac algorithm -------------------------------------------------------------------------------------

	while ( true ){	// until converge, find query or collapse

		end_time = clock();

		time = (double) (end_time - start_time)/CLOCKS_PER_SEC;

		// End after one sec if have found at least one query being rejected by the bias

		if ( !sol_opt && (time > cutoff || time > bound) && best_bias_rej > 0 ){

			cout << endl << "best_bias_rej = " << best_bias_rej << endl << endl;
			cout << "nodes visited = " << nodes_visited << endl;
cout << "here?" << endl;
			return best_query;
			
		} else if ( time > bound && (sol_opt || solp_opt || max_b) ){
cout << "2 here?" << endl;

			best_query.set_state(5);
			return best_query;
			
		}
		
		// if variable var_index has no valid domain value then backtrack to last variable ------------------------
		while ( variable[var_index].get_domain_size() == 0 ){

			vars_queue.push_back( var_index );	// push this var to vars_queue

			variable[var_index].set_value(wrong);	// set its value to wrong ( no value )

		// decrease the instantiated variables number for the constraints it is involved
			for (int i = 0; i < variable[var_index].get_bias_con_size(); i++){	
				
				bias.decrease_inst_vars( variable[var_index].get_bias_con(i) );

				for (int j = 0; j < bias.get_con( variable[var_index].get_bias_con(i) ).get_scope_size(); j++){

					if ( bias.get_con( variable[var_index].get_bias_con(i) ).get_scope_vars()[j] != var_index )
						variable[ bias.get_con( variable[var_index].get_bias_con(i) ).get_scope_vars()[j] ].decrease_bwdeg();
				
				}

			}

		// -----------------------------------------------------------------------------

			var_index = prev_var();

			if ( var_index >= 0 ) {	// If mac has not ended

				variable[var_index].set_value(wrong);

				restore_domain(var_index);	// restore previous changes to domains to continue	

				// for the bias violating check ------

				bias_rej = bias_rejects.back();

				sum_rejects -= bias_rej;

				bias_to_check += bias_checked.back();

				bias_rejects.pop_back();

				bias_checked.pop_back();

				bias_remain_restore(var_index);

				// -----------------------------------
				
			} else if ( var_index == -1 ){	// then end mac algorithm

				if ( best_bias_rej == 0 )		// converge ------------
					best_query.set_state(3);
				else if ( best_bias_rej == bias.get_size() )	// if all constraints of bias are rejecting it
					best_query.set_state(4);

				return best_query;

			}

		}

		// choose randomely a valid domain value and assign to variable var_index --------------------------------
		domain_pos = pick_domain_value(var_index);
		variable[var_index].set_value( variable[var_index].get_domain_value(domain_pos) );	

		// Count Bias rejects by the chosen value and check if it can maximaze best query's bias rejects ----------

		bias_rej = count_bias_rejects(var_index);

		sum_rejects += bias_rej;

		bias_to_check -= bias_checked.back();

		// --------------------------------------------------------------------------------------------------------

		nodes_visited++;

		if ( vars_queue.size() > 0 ){

			// Decide if we want to branch or cut

			if ( (max_opt || max_b) && (sum_rejects + bias_to_check) > best_bias_rej )
				bias_rej_flag = true;
			else if( min_opt && (sum_rejects < best_bias_rej || best_bias_rej == 0) && (sum_rejects + bias_to_check) > 0)
				bias_rej_flag = true;
			else if ( (sol_opt || solp_opt) && (sum_rejects + bias_to_check) > 0)
				bias_rej_flag = true;
			else
				bias_rej_flag = false;

			// Run gac ------------------------------------------------------------------------------------------------

			if ( var_heuristic == 2 ) {	// if var_heuristic is wdeg or dom / wdeg

				// calculate wdeg for all the variables in queue
				for (i = 0; i < vars_queue.size(); i++){	
					calc_wdeg(vars_queue[i]);
				}

			}

			gac_flag = gac(var_index, scope);	

			if ( gac_flag && bias_rej_flag ){

				if ( max_b || solp_opt ) {	// if partial heuristic (max_B or solp) is used

					// check how many bias constraints are violated from this query ----------------------------------------------------

					if ( max_b && (best_bias_rej < sum_rejects) ){ //&& (sum_rejects != bias.get_size() || scope.size() == VAR_NUM || bias.get_size() == 1) ) {

						best_bias_rej = sum_rejects;
						
						query.initialize();
						query.set_state(1);

						for (i = 0; i < variable.size(); i++)
							query.set_var(variable[i].get_value(), i);

						best_query = query;				
						
					} else if ( solp_opt && best_vars_inst < used_vars_queue.size()+1 && sum_rejects > 0 ){

						best_vars_inst = used_vars_queue.size()+1;
						best_bias_rej = sum_rejects;

						query.initialize();
						query.set_state(1);

						for (i = 0; i < variable.size(); i++)
							query.set_var(variable[i].get_value(), i);

						best_query = query;	

					}

				}

				used_vars_queue.push_back(var_index);	// push it in used vars

				var_index = next_var();	// get next variable

			} else {

				variable[var_index].set_value(wrong);				

				restore_domain(var_index);	// restore previous changes to domains to continue

				// for the bias violating check ------

				bias_rej = bias_rejects.back();

				sum_rejects -= bias_rej;

				bias_to_check += bias_checked.back();

				bias_rejects.pop_back();

				bias_checked.pop_back();

				bias_remain_restore(var_index);

				// -----------------------------------

			}

		} else if ( vars_queue.size() == 0 ){ 

			if ( used_vars_queue.size() < VAR_NUM-1 ) {			// To check if theoretically it could lead to a solution

				if ( var_heuristic == 2 ) {	// if var_heuristic is wdeg or dom / wdeg

					// calculate wdeg for all the variables in queue
					for (i = 0; i < vars_queue.size(); i++){	
						calc_wdeg(vars_queue[i]);
					}

				}

				gac_flag = gac(var_index, scope);	

			} else
				gac_flag = true;
			
			
			if ( gac_flag ) {

		// if reached this point there was no collapse, now create query --------------------------------------------------

				query.initialize();
				query.set_state(1);

				for (i = 0; i < variable.size(); i++)
					query.set_var(variable[i].get_value(), i);

		// check how many bias constraints are violated from the query ----------------------------------------------------
					
				if ( (max_opt || max_b) && (best_bias_rej < sum_rejects)){ // && (sum_rejects != bias.get_size() || scope.size() == VAR_NUM) ) {

					best_bias_rej = sum_rejects;		
					best_query = query;				

				} else if ( min_opt && (sum_rejects < best_bias_rej || best_bias_rej == 0) && sum_rejects > 0 ){

					best_bias_rej = sum_rejects;		
					best_query = query;				

				} 
				
				if ( (sol_opt || solp_opt) && sum_rejects > 0)// && (sum_rejects != bias.get_size() || scope.size() == VAR_NUM || bias.get_size() == 1) )
					return query;
				
			}

			variable[var_index].set_value(wrong);				

			restore_domain(var_index);	// restore previous changes to domains to continue

			// for the bias violating check ------

			bias_rej = bias_rejects.back();

			sum_rejects -= bias_rej;

			bias_to_check += bias_checked.back();

			bias_rejects.pop_back();

			bias_checked.pop_back();

			bias_remain_restore(var_index);

			// -----------------------------------

		}

	}

	return best_query;

}	// end of function generate query() -----------------------------------------------------------------


Query Solver::generate_findc_query(vector<short int> scope, Constraint_Net Delta_p){

	// In this query generator with 'bias' we mean the Delta set.

	int var_index, i, j;	// var_index: index for variables

	int first_var;

	int domain_pos;	// index of domain - var.

	bool gac_flag ;

	bool bias_rej_flag;	// if we can maximize max_delta_rej

	clock_t start_time, end_time;	// to run for 1 sec

	double time;

	int bias_rej;	// how many constraints of the delta are rejecting the query

	int delta_p_rej;	// how many constraints of the delta_p are rejecting the query

	int sum_rejects = 0;	// counter of Delta constraints violated 

	int bias_to_check = bias.get_size();	// counter of delta constraints left to check ( bias.get_size() - bias.checked() )

	Query query, best_query;

	int nodes_visited = 1;

	gac_flag = gac(scope);	// gac algorithm in the begining

	// Initialize Variables queue -----------
	for (i = scope.size()-1; i >= 0; i--){	
		vars_queue.push_back(scope[i]) ;
	}

	// get first variable -------------------

	if ( var_heuristic == 2 ) {	// if var_heuristic is wdeg or dom / wdeg

		for (i = 0; i < vars_queue.size(); i++){	// calculate wdeg for all the variables
			calc_wdeg(vars_queue[i]);
		}

	}

	var_index = next_var();	// get next variable with max wdeg

	first_var = var_index;

	// --------------------------------------

	// main loop of mac algorithm -------------------------------------------------------------------------------------

	start_time = clock();	// starting time of mac algorithm ( to end after one sec if it has found at least one query )

	while ( true ){	// until converge, find query or collapse

		end_time = clock();

		time = (double) (end_time - start_time)/CLOCKS_PER_SEC;

		// if variable var_index has no valid domain value then backtrack to last variable ------------------------
		while ( variable[var_index].get_domain_size() == 0 ){

			vars_queue.push_back( var_index );	// push this var to vars_queue

			variable[var_index].set_value(wrong);	// set its value to wrong ( no value )

		// decrease the instantiated variables number for the constraints it is involved
			for (int i = 0; i < variable[var_index].get_bias_con_size(); i++){	
				
				bias.decrease_inst_vars( variable[var_index].get_bias_con(i) );

				for (int j = 0; j < bias.get_con( variable[var_index].get_bias_con(i) ).get_scope_size(); j++){

					if ( bias.get_con( variable[var_index].get_bias_con(i) ).get_scope_vars()[j] != var_index )
						variable[ bias.get_con( variable[var_index].get_bias_con(i) ).get_scope_vars()[j] ].decrease_bwdeg();
				
				}

			}

		// -----------------------------------------------------------------------------

			var_index = prev_var();

			if ( var_index >= 0 ) {	// If mac has not ended

				variable[var_index].set_value(wrong);

				restore_domain(var_index);	// restore previous changes to domains to continue	

				// for the bias violating check ------

				bias_rej = bias_rejects.back();

				sum_rejects -= bias_rej;

				bias_to_check += bias_checked.back();

				bias_rejects.pop_back();

				bias_checked.pop_back();

				bias_remain_restore(var_index);

				// -----------------------------------
				
			} else if ( var_index == -1 )	// then end mac algorithm
				return best_query;			// if state == 0 it is nil

		}

		// choose randomely a valid domain value and assign to variable var_index --------------------------------
		domain_pos = pick_domain_value(var_index);
		variable[var_index].set_value( variable[var_index].get_domain_value(domain_pos) );	

		// Count Bias rejects by the chosen value -----------------------------------------------------------------

		bias_rej = count_bias_rejects(var_index);

		sum_rejects += bias_rej;

		delta_p_rej = Delta_p.count_rejects(variable);

		bias_to_check -= bias_checked.back();

		// --------------------------------------------------------------------------------------------------------

		nodes_visited++;

		// Run gac ------------------------------------------------------------------------------------------------

		if ( vars_queue.size() > 0 ){

			if ( var_heuristic == 2 ) {	// if var_heuristic is wdeg or dom / wdeg

				// calculate wdeg for all the variables in queue
				for (i = 0; i < vars_queue.size(); i++){	
					calc_wdeg(vars_queue[i]);
				}

			}

			gac_flag = gac(var_index, scope);		


			if ( gac_flag ) {

				used_vars_queue.push_back(var_index);	// push it in used vars

				var_index = next_var();	// get next variable with max wdeg

			} else {

				variable[var_index].set_value(wrong);				

				restore_domain(var_index);	// restore previous changes to domains to continue

				// for the bias violating check ------

				bias_rej = bias_rejects.back();

				sum_rejects -= bias_rej;

				bias_to_check += bias_checked.back();

				bias_rejects.pop_back();

				bias_checked.pop_back();

				bias_remain_restore(var_index);

				// -----------------------------------

			}

		} else if ( vars_queue.size() == 0 ){ 

	// if reached this point there was no collapse, now create query --------------------------------------------------

			query.initialize();
			query.set_state(1);

			for (i = 0; i < variable.size(); i++)
				query.set_var(variable[i].get_value(), i);

	// check how many bias constraints are violated from the query ----------------------------------------------------

			if ( 0 < delta_p_rej && delta_p_rej < Delta_p.get_size() )	// If we found a query with this condition
				return query;	// return the query

			variable[var_index].set_value(wrong);				

			restore_domain(var_index);	// restore previous changes to domains to continue

			// for the bias violating check ------

			bias_rej = bias_rejects.back();

			sum_rejects -= bias_rej;

			bias_to_check += bias_checked.back();

			bias_rejects.pop_back();

			bias_checked.pop_back();

			bias_remain_restore(var_index);

			// -----------------------------------

		}

	}

}	// end of function generate_findc_query() -----------------------------------------------------------------


Query Solver::mac(){	// if no arguments then run for all varaibles

	vector<short int> scope;

	for (int i = 0; i < VAR_NUM; i++){
		scope.push_back(i);
	}


	return mac(scope);

}

Query Solver::mac(vector<short int> scope){

	int var_index, i, j;	// var_index: index for variables

	int first_var;

	int domain_pos;	// index of domain.	

	bool found_query = false;	// if a query was found

	bool gac_flag = true;

	clock_t start_time, end_time;	// to run for 1 sec

	double time;

	Query query;

	gac_flag = gac(scope);	// gac algorithm in the begining

	int nodes_visited = 1;

	int counter = 1;

	// Initialize Variables queue -----------

	for (i = scope.size()-1; i >= 0; i--){	
		vars_queue.push_back(scope[i]) ;
	}

	// get first variable -------------------

	if ( var_heuristic == 2 ) {	// if var_heuristic is wdeg or dom / wdeg

		for (i = 0; i < vars_queue.size(); i++){	// calculate wdeg for all the variables
			calc_wdeg(vars_queue[i]);
		}

	}

	var_index = next_var(1);	// get next variable with max wdeg

	first_var = var_index;

	// --------------------------------------

	// main loop of mac algorithm -------------------------------------------------------------------------------------

	start_time = clock();	// starting time of mac algorithm ( to end after one sec if it has found at least one query )

	while ( true ){	// until converge, find query or collapse

		end_time = clock();

		time = (double) (end_time - start_time)/CLOCKS_PER_SEC;

		// if variable var_index has no valid domain value then backtrack to last variable ------------------------
		while ( variable[var_index].get_domain_size() == 0 ){

			vars_queue.push_back( var_index );	// push this var to vars_queue

			variable[var_index].set_value(wrong);	// set its value to wrong ( no value )

			var_index = prev_var();

			if ( var_index >= 0 ) {	// If mac has not ended

				variable[var_index].set_value(wrong);

				restore_domain(var_index);	// restore previous changes to domains to continue	

			} else if ( var_index == -1 ){	// then end mac algorithm

				query.set_state(2);	// not found query

				return query;
	
			}

		}

		// end of backtrack --------------------------------------------------------------------------------------


		// choose randomely a valid domain value and assign to variable var_index --------------------------------
		domain_pos = pick_domain_value(var_index);

		variable[var_index].set_value( variable[var_index].get_domain_value(domain_pos) );	

		nodes_visited++;

		// Run gac ------------------------------------------------------------------------------------------------

		if ( vars_queue.size() > 0 ){

			gac_flag = gac(var_index, scope);		

			if ( gac_flag ) {

				if ( var_heuristic == 2 ) {	// if var_heuristic is wdeg or dom / wdeg

					// calculate wdeg for all the variables in queue
					for (i = 0; i < vars_queue.size(); i++){
						calc_wdeg(vars_queue[i]);
					}

				}

				used_vars_queue.push_back(var_index);	// push it in used vars

				var_index = next_var(1);	// get next variable with max wdeg

			} else {

				variable[var_index].set_value(wrong);

				restore_domain(var_index);	// restore previous changes to domains to continue

			}

		} else if ( vars_queue.size() == 0 ){ 

	// if reached this point there was no collapse, now return query --------------------------------------------------
			query.initialize();
			query.set_state(1);

			for (i = 0; i < variable.size(); i++)
				query.set_var(variable[i].get_value(), i);

			return query;
		}

	}

}	// end of function mac()


int Solver::pick_domain_value(int var_index){

	int dom_value;

	int domain_pos;

	int counter = 0;	

	int i, r;

	int max = -1;

	int bias_rej;

	if ( domain_heuristic == 0 ) {	

		r = 1;

		for (i = 0; i < variable[var_index].get_domain_values().size() && counter < r; i++){	// for each domain value

			if ( variable[var_index].get_domain_state(i) == -1 )
				counter++;
			
			if ( counter == r )
				domain_pos = i;	// which domain value was assigned to the variable
			
		}

	} else if ( domain_heuristic == 1 ) {	

		r = ( rand() % variable[var_index].get_domain_size() ) + 1;

		for (i = 0; i < variable[var_index].get_domain_values().size() && counter < r; i++){	// for each domain value

			if ( variable[var_index].get_domain_state(i) == -1 )
				counter++;
			
			if ( counter == r )
				domain_pos = i;	// which domain value was assigned to the variable
			
		}

	} else if ( domain_heuristic == 2 ) {	


		for (i = 0; i < variable[var_index].get_domain_values().size(); i++){	// for each domain value

			if ( variable[var_index].get_domain_state(i) == -1 ){

				variable[var_index].set_value( variable[var_index].get_domain_value(i) );	

				bias_rej = check_bias_rejects(var_index);

				variable[var_index].set_value( wrong );	

				if ( max < bias_rej ) {

					domain_pos = i;

					max = bias_rej;

				}

			}

		}

		if ( max == 0 ){			// if not found a value violating at least one constraint then make random pick

			domain_heuristic = 1;	// random heuristic
			domain_pos = pick_domain_value(var_index);
			domain_heuristic = 2;	// max heuristic

		}

	}

	if ( used_vars_queue.size() > 0 ) {

	// To avoid chosing the same value ....
	// to prev var : to restore when restore the removed from the assignation of previous variable 
		variable[var_index].set_domain_state(used_vars_queue.back(), domain_pos);

		StatesDomains[used_vars_queue.back()][var_index] = true;

	} else if ( used_vars_queue.size() == 0 ){

	// delete those domain values because no solution comes from them .. 
		variable[var_index].set_domain_state(-2, domain_pos);	
	}

	return domain_pos;

}

void Solver::restore_domain(int state){	// restore domain values with state = state

	int i, j;

	int var_size = variable.size();

	int dom_size;

	for (i = 0; i < var_size; i++){

		if ( StatesDomains[state][i] ) {

			dom_size = variable[i].get_domain_states().size();


			for (j = 0; j < dom_size; j++){

				if ( variable[i].get_domain_state(j) == state ){
					
					variable[i].set_domain_state(-1, j);	// restore to default		

				}

			}

			StatesDomains[state][i] = false;
			
		}

	}

}	// end of method restore_domains()


int Solver::next_var(){
	return next_var(var_heuristic);
}	// end of method next_var()

int Solver::next_var(int h){

	int max = -1;

	double min = 1000000;

	int position = -1;

	int var = -1;

	vector<int> vars;

	if ( h == 0) {	// no var_heuristic, LIFO ----------------------- ( LIFO and NOT FIFO: to backtrack )

		if ( vars_queue.size() > 0 ) {

			var = vars_queue.back();
			vars_queue.pop_back();

		}

	} else if ( h == 1) {	// dom heuristic -------------------

		// choose next variable with minimum DOM size	---------------------------

		for (int i = 0; i < vars_queue.size(); i++){

			if ( variable[ vars_queue[i] ].get_domain_size() <= min ){

				min = variable[ vars_queue[i] ].get_domain_size();
				position = i ;
				var = vars_queue[i];

			} 
		}

	} else if ( h == 2) {

		// choose next variable with minimum domain_size / wdeg	--------------------

		for (int i = 0; i < vars_queue.size(); i++){

			if ( min >= variable[ vars_queue[i] ].get_dom_wdeg() ){

				min = variable[ vars_queue[i] ].get_dom_wdeg();
				position = i ;
				var = vars_queue[i];

			} 
		}

	} else if ( h == 3) {

		// choose next variable with max bias constraints heuristic ------------------

		for (int i = 0; i < vars_queue.size(); i++){

			if ( max <= variable[ vars_queue[i] ].get_bias_con_size() ){

				max = variable[ vars_queue[i] ].get_bias_con_size();
				position = i ;
				var = vars_queue[i];

			}
		}

	} else if ( h == 4) {

		// choose next variable with max wbdeg var_heuristic ------------------

		for (int i = 0; i < vars_queue.size(); i++){

			if ( max < variable[ vars_queue[i] ].get_bwdeg() ){

				max = variable[ vars_queue[i] ].get_bwdeg();

				vars.clear();
				vars.push_back( i );

			} else if ( max == variable[ vars_queue[i] ].get_bwdeg() ){
				vars.push_back( i );
			}

		}

		if ( vars.size() > 1 ) {

			max = 0;

			// if more than one have the same bwdeg then use var_heuristic 3 to chose among them
			for (int i = 0; i < vars.size(); i++){

				if ( max <= variable[ vars_queue[ vars[i] ] ].get_bias_con_size() ){

					max = variable[ vars_queue[ vars[i] ] ].get_bias_con_size();
					position = vars[i] ;
					var = vars_queue[ vars[i] ];

				}
			}	

		} else {

			position = vars[0];
			var = vars_queue[ position ];

		}


	}

		if ( position > -1 ){
			vars_queue.erase(vars_queue.begin() + position );	// erase it from the queue of vars
		}

	// increase the instantiated variables number for the constraints it is involved
	for(int i = 0; i < variable[var].get_bias_con_size(); i++){	

		bias.increase_inst_vars(variable[var].get_bias_con(i));

		for (int j = 0; j < bias.get_con( variable[var].get_bias_con(i) ).get_scope_size(); j++){

			if ( bias.get_con( variable[var].get_bias_con(i) ).get_scope_vars()[j] != var )
				variable[ bias.get_con( variable[var].get_bias_con(i) ).get_scope_vars()[j] ].increase_bwdeg();
		}

	}

	return var;

}	// end of method next_var()

int Solver::prev_var(){	// returns previous variable, if no previous then return -1 to end mac algorithm

	int var;

	if ( used_vars_queue.size() > 0 ) {

		var = used_vars_queue.back();
		used_vars_queue.pop_back();

	} else
		var = -1;

	return var;

}


void Solver::calc_wdeg(short int v){		// calculate wdeg of the variable, v: the index of current var
	
	int wdeg = 0;

	int i,j;

	bool uninstantiated; // initialize to false for each constraint. 
						 //If at least one is uninstantiated then make it true

	int pos;

	for (i = 0; i < variable[v].get_con_size(); i++){

		uninstantiated = false;

		pos = variable[v].get_con(i);

		Constraint con = cl.get_con(pos);

		for (j = 0; j < con.get_scope_size() && !uninstantiated; j++){

			if ( con.get_var(j) != v ){

				if ( variable[ con.get_var(j) ].get_value() == wrong )
					uninstantiated = true;

			}

			if ( uninstantiated )
				wdeg += con.get_weight();

		}

	}

	variable[v].set_wdeg(wdeg);
	variable[v].calc_dom_wdeg();

}


int Solver::count_bias_rejects(int var){		// count rejects after Variable var assignation

	int cons = variable[var].get_bias_con_size();

	int rejects = 0;
	
	int checked = 0;

	int pos, i, j;

	bool uninstantiated;

	int *value = new int[4];

	for (i = 0; i < cons; i++){		// for every constraint in bias that the variable is in its scope

		uninstantiated = true;

		pos = variable[var].get_bias_con(i);

		bias_remain[pos]++ ;

		if ( bias_remain[pos] == bias.get_con(pos).get_scope().get_real_size() ){
			
			checked++;
			uninstantiated = false;

		}

		if ( !uninstantiated ){

			for (j = 0; j < bias.get_con(pos).get_scope_size(); j++){

				value[j] = variable[ bias.get_con(pos).get_var(j) ].get_value();

			}

			if ( bias.get_con(pos).get_scope_size() == 1 ){

				if ( !bias.get_con(pos).satisfy( value ) )
					rejects++;
					
			}

			if ( bias.get_con(pos).get_scope_size() == 2 ){

				if ( !bias.get_con(pos).satisfy( value[0], value[1] ) )
					rejects++;

			} else if ( bias.get_con(pos).get_scope_size() == 4 ){

				if ( !bias.get_con(pos).satisfy( value[0], value[1], value[2], value[3] ) )
					rejects++;

			}
			
		}

	}

	bias_checked.push_back(checked);

	bias_rejects.push_back(rejects);

	return rejects;

}

int Solver::check_bias_rejects(int var){		// count rejects before  Variable var assignation,

	int cons = variable[var].get_bias_con_size();

	int rejects = 0;
	
	int pos, i, j;

	bool uninstantiated;

	int *value = new int[4];

	for (i = 0; i < cons; i++){		// for every constraint in bias that the variable is in its scope

		uninstantiated = true;

		pos = variable[var].get_bias_con(i);

		if ( bias_remain[pos] == bias.get_con(pos).get_scope_size()-1 ){	// if all the other vars are instantiated !!
			uninstantiated = false;
		}

		if ( !uninstantiated ){

			for (j = 0; j < bias.get_con(pos).get_scope_size(); j++){

				value[j] = variable[ bias.get_con(pos).get_var(j) ].get_value();

			}

			if ( bias.get_con(pos).get_scope_size() == 1 ){

				if ( !bias.get_con(pos).satisfy( value ) )
					rejects++;
					
			}

			if ( bias.get_con(pos).get_scope_size() == 2 ){

				if ( !bias.get_con(pos).satisfy( value[0], value[1] ) )
					rejects++;

			} else if ( bias.get_con(pos).get_scope_size() == 4 ){

				if ( !bias.get_con(pos).satisfy( value[0], value[1], value[2], value[3] ) )
					rejects++;

			}
			
		}
		
	}

	return rejects;

}

void Solver::bias_remain_restore(int var){

	int cons = variable[var].get_bias_con_size();

	for (int i = 0; i < cons; i++){		// for every constraint in bias that the variable is in its scope

		bias_remain[ variable[var].get_bias_con(i) ]-- ;	

	}

}
