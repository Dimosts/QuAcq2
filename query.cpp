#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h> 

#define measure_queries_cut 100

#include "query.h" 

using namespace std;

// Class Query methods -------------------------------------------------------------------------------

Query::Query(){
	initialize();
}

void Query::set_state(short int s){
	state = s;
}

void Query::set_var(int v, int i){	// v: variable value, i: variable indicator
	var[i] = v;
}

short int Query::get_state() const{
	return state;
}

int Query::get_var(int i) const{
	return var[i];
}

int Query::get_size(){

	int query_size = 0;

	for (int i = 0; i < VAR_NUM; i++)
		if ( var[i] >= 0 )
			query_size++;
		
		
	return query_size;
	
}


vector<int> Query::get_vars() const{
	return var;
}

void Query::print(){	// sudoku print for 4 * 4

	int x;

	cout << "Query :" << endl;

	int cell_r = sqrt(DOM_SIZE);

	if ( VAR_NUM < 80) {

		for (int i = 0; i < VAR_NUM; i++){
			
			cout << " | " << var[i];

			if ( i % 9 == 0 )
				cout << endl << endl;

		}

		cout << endl;

	} else {

		for (int i=0; i<DOM_SIZE; i++){

			if ( (i % cell_r) == 0 ) {

				cout << " ";

				for (int j=0; j<11; j++){
					cout << " __" ;
				}

				cout << endl;

			}

			for (int j=0; j<DOM_SIZE; j++){

				if ( (j % cell_r) == 0 ) {
					cout << " | ";
				}

				x = var[j + i*DOM_SIZE];

				if ( x >= 0 ){
					cout << " " << x << " ";
				} else {
					cout << " x ";
				}

			}

			cout << endl;
		}
	}

}	// end of method print()

void Query::initialize(){

	var.clear();

	for (int i=0; i<VAR_NUM; i++){
		var.push_back(wrong);
	}

	state = 0;
}


bool Query::ask(){

	string ans;

	bool answer;

	bool flag = true;

	int query_size = 0;

	counter_queries++;

	end_time = clock();

	double tmp_time = (double) (end_time - start_time)/CLOCKS_PER_SEC - f_time; // time from previous query ( previous f_time )!!

	if ( tmp_time > max_query_time )
		max_query_time = tmp_time;


	f_time = (double) (end_time - start_time)/CLOCKS_PER_SEC;

	for (int i = 0; i < VAR_NUM; i++)
		if ( var[i] >= 0 )
			query_size++;
	
	if ( query_size == VAR_NUM )
		complete_queries++;
		
	queries_size_sum += query_size;	

	if ( user ) {

		while ( flag ){

			cout << "Is that query true ? (y or n)" << endl << endl;

			print();

			cout << endl << "Answer: ";
			cin >> ans;

			if ( ans.compare("y") == 0 || ans.compare("Y") == 0 ){
				answer = true;
				flag = false;
			} else if ( ans.compare("n") == 0 || ans.compare("N") == 0 ){
				answer = false;
				flag = false;
			} else {
				cout << endl << "Did not understand, please answer again" << endl;
			}

		}

	} else {

		print();

		cout << endl << "Answer: ";

		if ( Sol.rejects(*this) ){
			
			answer = false;
			cout << "No" << endl << endl;
			
			// for evaluation metrics ---
			start_time_pos = clock();
			// --------------------------

		} else {

			answer = true;
			cout << "Yes" << endl << endl;

			// for evaluation metrics ---
			counter_queries_conv++;
			// --------------------------

		}

	}

	start_cut = clock();

	return answer;

}	// end of method ask()


Query Query::partial(vector<short int> vars){

	Query query;

	int i;

	int tmp_var[VAR_NUM];

	// create the partial query ( wrong value to the other variables ) ----------------

	for (i = 0; i < VAR_NUM; i++){
		tmp_var[i] = wrong;
	}

	for (i = 0; i < vars.size(); i++){	
		tmp_var[ vars[i] ] = var[ vars[i] ];
	}

	for (i = 0; i < VAR_NUM; i++){
		query.set_var( tmp_var[i], i );
	}

	return query;

}	// end of method partial()


bool Query::subqueryof(Query query){ 	// return true if this query is a sub-query of the query given. Return false otherwise

	for (int i = 0; i < var.size(); i++){

		if ( var[i] != query.get_var(i) && var[i] != wrong )
			return false;

	}

	return true;

}

bool Query::operator !=(Query const &query){
		
    
	if ( var.size() != query.get_vars().size() )
		return true;
	
	for (int i = 0; i < var.size(); i++)
		if ( var[i] != query.get_var(i) )
			return true;
	
	
	if ( state != query.get_state() )
		return true;
	
	return false;
}

bool Query::operator ==(Query const &query){
	return !(*this != query);
}

