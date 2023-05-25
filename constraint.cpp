#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

#include "solver.h" 

using namespace std;


// Class Scope methods -------------------------------------------------------------------------------

	// Constructor --------------------------

Scope::Scope(){}

Scope::Scope(vector<short int> var){
	set_scope(var);
}


	// set - push ---------------------------

void Scope::set_scope(vector<short int>  var){
	variables = var;
	set_distinct_variables();
}

void Scope::push_var(short int v){

	variables.push_back(v);

	if ( find(distinct_variables.begin(), distinct_variables.end(), v) == distinct_variables.end()  )
		distinct_variables.push_back(v);

}

void Scope::set_distinct_variables(){

	int size = variables.size();

	distinct_variables.clear();

	for (int i = 0; i < size; i++)
		if ( find(distinct_variables.begin(), distinct_variables.end(), variables[i]) == distinct_variables.end()  )
 			distinct_variables.push_back(variables[i]);

}


	// get ----------------------------------

vector<short int> Scope::get_scope(){
	return variables;
}

vector<short int> Scope::get_variables(){
	return variables;
}

vector<short int> Scope::get_distinct_variables(){	// get individual variables
	return distinct_variables;
}

short int Scope::get_var(short int i){
	return variables[i];
}

int Scope::get_size(){
	return variables.size();
}

int Scope::get_real_size(){
	return distinct_variables.size();
}


	// other methods ------------------------

bool Scope::compare(Scope scope){	// compare with Scope scope

	if ( distinct_variables.size() != scope.get_real_size() )
		return false;
	else {

		vector<short int> var = scope.get_scope();

		for (int i = 0; i < var.size(); i++)
			if ( find(variables.begin(), variables.end(), var[i]) == variables.end()  )
				return false;

		for (int i = 0; i < variables.size(); i++)
			if ( find(var.begin(), var.end(), variables[i]) == var.end()  )
				return false;

	}

	return true;

}

bool Scope::compare(vector<short int> var){	// compare with variables

	for (int i = 0; i < var.size(); i++)
		if ( find(variables.begin(), variables.end(), var[i]) == variables.end()  )
			return false;

	for (int i = 0; i < variables.size(); i++)
		if ( find(var.begin(), var.end(), variables[i]) == var.end()  )
			return false;

	return true;

}

bool Scope::subset_of(Scope scope){	// check if this Scope is a subset of scope

	if ( variables.size() >= scope.get_size() )
		return false;
	else {

		vector<short int> var = scope.get_scope();

		for (int i = 0; i < variables.size(); i++)
			if ( find(var.begin(), var.end(), variables[i]) == var.end()  )
				return false;

	}

	return true;

}

bool Scope::subset_of(vector<short int> var){	// check if this Scope is a subset of variables var

	if ( variables.size() >= var.size() )
		return false;
	else
		for (int i = 0; i < variables.size(); i++)
			if ( find(var.begin(), var.end(), variables[i]) == var.end()  )
				return false;

	return true;

}

void Scope::print(){	// clear 
	
	cout << "Scope : ";

	for (int i = 0; i < variables.size(); i++){
		cout << variables[i] << ", ";
	}

}

void Scope::println(){	// clear 
	
	print();
	cout << endl;

}

void Scope::clear(){	// clear 
	variables.clear();
	distinct_variables.clear();
}


// Class Constraint methods -------------------------------------------------------------------------------

	// Constructors -------------------------

Constraint::Constraint(){

	type = -1;
	weight = 1;

	value = 0;

	reified = false;

	satisfied = UNDEF;
	state = -1;

	inst_variables = 0;

}

Constraint::Constraint(short int t){

	set_type(t);
	weight = 1;

	value = 0;

	reified = false;

	satisfied = UNDEF;
	state = -1;

	inst_variables = 0;

}

Constraint::Constraint(short int t, int v){	// type and value 

	set_type(t);
	weight = 1;

	value = v;

	reified = false;

	satisfied = UNDEF;
	state = -1;

	inst_variables = 0;

}

Constraint::Constraint(short int t, bool r){

	set_type(t);
	weight = 1;

	value = 0;

	reified = r;

	satisfied = UNDEF;
	state = -1;

	inst_variables = 0;

}

Constraint::Constraint(short int t, vector<short int>  v){

	set_type(t);
	set_scope(v);

	weight = 1;

	value = 0;

	reified = false;

	satisfied = UNDEF;
	state = -1;

	inst_variables = 0;

}

Constraint::Constraint(short int t, vector<short int>  v, bool r){

	set_type(t);
	set_scope(v);

	weight = 1;

	value = 0;

	reified = r;

	satisfied = UNDEF;
	state = -1;

	inst_variables = 0;

}

Constraint::Constraint(short int t, Scope s){

	set_type(t);
	set_scope(s);

	weight = 1;

	value = 0;

	reified = false;

	satisfied = UNDEF;
	state = -1;

 	inst_variables = 0;

}

Constraint::Constraint(short int t, Scope s, bool r){

	set_type(t);
	set_scope(s);

	weight = 1;

	value = 0;

	reified = r;

	satisfied = UNDEF;
	state = -1;

	inst_variables = 0;

}

	// set - push ---------------------------

void Constraint::set_type(short int t){

	type = t;

	if ( type == 0 )
		con = "!=";
	else if ( type == 1 )
		con = "=";
	else if ( type == 2 )
		con = ">";
	else if ( type == 3 )
		con = "<";
	else if ( type == 4 )
		con = ">=";
	else if ( type == 5 )
		con = "<=";
	else if ( type == 6 )
		con = "-6";
	else if ( type == 7 )
		con = "-7";
	else if ( type == 8 )
		con = "-8";
	else if ( type == 9 )
		con = "-9";
	else if ( type == 10 )
		con = "-10";
	else if ( type == 11 )
		con = "-11";
	else if ( type == 12 )
		con = "-12";
	else if ( type == 13 )
		con = "-13";	
	else if ( type == 14 )
		con = "-14";
	else if ( type == 15 )
		con = "-15";
	else if ( type == 16 )
		con = "-15";
	else if ( type == 17 )
		con = "-17";
	else if ( type == 18 )
		con = "!=";
	else if ( type == 19 )
		con = "=";	
	else if ( type == 20 )
		con = ">";
	else if ( type == 21 )
		con = "<";
	else if ( type == 22 )
		con = ">=";
	else if ( type == 23 )
		con = "<=";
	else {
		cout << "Wrong type in constraint !!" << endl;
		exit(-1);
	}
	
}

void Constraint::set_scope(vector<short int>  v){
	scope.set_scope(v);
}

void Constraint::set_scope(Scope s){
	scope = s;
}

void Constraint::push_var(short int v){
	scope.push_var(v);
}

void Constraint::set_weight(int w){
	weight = w;
}

void Constraint::set_value(int v){
	value = v;
}

void Constraint::set_reified(bool r){
	reified = r;
}

void Constraint::set_satisfied(int s){	// satisfied = s;
	satisfied = s;
}

void Constraint::set_state(int s){	// state = s;
	state = s;
}

void Constraint::reify(){
	reified = true;
}

void Constraint::unreify(){
	reified = false;
}

void Constraint::increase_weight(){
	weight++;
}

void Constraint::increase_inst_vars(){	// increase instantiated variables by 1
	
	inst_variables++;

	if ( inst_variables > scope.get_real_size() ) {	// check if wrong call
		cout << "Error: increasing instantiated variables more than scope size" << endl;
		cout << "Instantiated variables = " << inst_variables << " , scope size = " << scope.get_real_size() << endl;
		scope.println();

		exit(-1);
	}

}

void Constraint::decrease_inst_vars(){	// decrease instantiated variables by 1

	inst_variables--;

	if ( inst_variables < 0 ) {	// check if wrong call
		cout << "Error: decreasing instantiated variables less than zero (0)" << endl;
		exit(-1);
	}

}

void Constraint::set_inst_vars(int n){	// set instantiated variables to n

	inst_variables = n;

	if ( inst_variables < 0 ) {	// check if wrong call
		cout << "Error: setting instantiated variables less than zero (0)" << endl;
		exit(-1);
	} else if ( inst_variables > scope.get_size() ) {	// check if wrong call
		cout << "Error: setting instantiated variables more than scope size" << endl;
		exit(-1);
	}

}


void Constraint::push_conjunction_constraint(Constraint con){

	if ( type != CONJUNCTION ){
		cout << "Error: the constraint is not a conjunction constraint, cannot push" << endl;
		exit(-1);
	} else if ( !scope.compare( con.get_scope() ) ){
		cout << "wrong scope while pushing in Conjuction constraint" << endl;
		exit(-1);
	}

	if ( con.get_type() == CONJUNCTION ){

		int size = con.get_conjunction_constraints().size();

		for (int i = 0; i < size; i++){	// if flag gets false then it stops
			conjunction.push_back( con.get_conjunction_constraint(i) );
		}

	} else
	conjunction.push_back(con);
	
}


	// get ----------------------------------

short int Constraint::get_type(){
	return type;
}

vector<short int> Constraint::get_scope_vars(){
	return scope.get_variables();
}

Scope Constraint::get_scope(){
	return scope;
}

int Constraint::get_scope_size(){
	return scope.get_size();
}

short int Constraint::get_var(short int i){
	return scope.get_var(i);
}

string Constraint::get_con(){
	return con;
}

int Constraint::get_weight(){
	return weight;
}

int Constraint::get_value(){
	return value;
}

bool Constraint::get_reified(){
	return reified;
}

int Constraint::get_satisfied(){
	return satisfied;
}

int Constraint::get_state(){
	return state;
}

int Constraint::get_inst_vars(){	// get the number of instantiated variables
	return inst_variables;
}

Constraint Constraint::get_conjunction_constraint(int i){

	if ( type != CONJUNCTION ){
		cout << "Error: the constraint is not a conjunction constraint, cannot get conjunction constraint" << endl;
		exit(-1);
	}

	return conjunction[i];

}
	
vector<Constraint> Constraint::get_conjunction_constraints(){

	if ( type != CONJUNCTION ){
		cout << "Error: the constraint is not a conjunction constraint, cannot get conjunction constraints" << endl;
		exit(-1);
	}

	return conjunction;

}

int Constraint::get_conjunction_size(){

	if ( type != CONJUNCTION )
		return 0;
	else
		return conjunction.size();

}


	// other methods ------------------------

bool Constraint::satisfy(vector<int> e){	// if an example e satisfies the constraint

	bool flag = false;

	if ( type == CONJUNCTION ){

		flag = true;	// initialize to true for this case

		int size = conjunction.size();

		for (int i = 0; i < size && flag; i++){	// if flag gets false then it stops
			flag = conjunction[i].satisfy(e);
		}

	} else if ( type < 9 ) {

		if ( e[scope.get_var(0)] != wrong && e[scope.get_var(1)] != wrong ) {

			if (type == 0){

				if( e[scope.get_var(0)] != e[scope.get_var(1)] ){
					flag = true;
				} 

			} else if (type == 1){

				if( e[scope.get_var(0)] == e[scope.get_var(1)] ){
					flag = true;
				}

			} else if (type == 2){

				if( e[scope.get_var(0)] > e[scope.get_var(1)] ){
					flag = true;
				}

			} else if (type == 3){

				if( e[scope.get_var(0)] < e[scope.get_var(1)] ){
					flag = true;
				}

			} else if (type == 4){

				if( e[scope.get_var(0)] >= e[scope.get_var(1)] ){
					flag = true;
				}

			} else if (type == 5){

				if( e[scope.get_var(0)] <= e[scope.get_var(1)] ){
					flag = true;
				}

			} else if (type == 6){

				if( (e[scope.get_var(0)] - e[scope.get_var(1)]) == 1 ){
					flag = true;
				}

			} else if (type == 7){

				if( (e[scope.get_var(1)] - e[scope.get_var(0)]) == 1 ){
					flag = true;
				}

			} else if (type == 8){

				if( abs(e[scope.get_var(0)] - e[scope.get_var(1)]) == 1 ){
					flag = true;
				}

			}

		}

	} else if ( type < 11) {

		if (type == 9){

			// |x_var(0) - x_var(1)| != |x_var(2) - x_var(3)|
			if( abs(e[scope.get_var(0)] - e[scope.get_var(1)]) !=  abs(e[scope.get_var(2)] - e[scope.get_var(3)]) ){
				flag = true;
			}

		} else if (type == 10){

			// |x_var(0) - x_var(1)| == |x_var(2) - x_var(3)|
			if( abs(e[scope.get_var(0)] - e[scope.get_var(1)]) ==  abs(e[scope.get_var(2)] - e[scope.get_var(3)]) ){
				flag = true;
			}

		}

	} else if ( type < 14) {

		if (type == 11){

			// |x_var(0) - x_var(1)| > value
			if( abs(e[scope.get_var(0)] - e[scope.get_var(1)]) >  value ){
				flag = true;
			}

		} else if (type == 12){

			// |x_var(0) - x_var(1)| == value
			if( abs(e[scope.get_var(0)] - e[scope.get_var(1)]) ==  value ){
				flag = true;
			}

		} else if (type == 13){

			// x_var(0) - x_var(1) == x_var(3)
			if( e[scope.get_var(0)] - e[scope.get_var(1)] ==  e[scope.get_var(2)] ){
				flag = true;
			}

		}

	} else if ( type < 16) {

		if (type == 14){

			// |x_var(0) - x_var(1)| > |x_var(2) - x_var(3)|
			if( abs(e[scope.get_var(0)] - e[scope.get_var(1)]) >  abs(e[scope.get_var(2)] - e[scope.get_var(3)]) ){
				flag = true;
			}

		} else if (type == 15){

			// |x_var(0) - x_var(1)| < |x_var(2) - x_var(3)|
			if( abs(e[scope.get_var(0)] - e[scope.get_var(1)]) <  abs(e[scope.get_var(2)] - e[scope.get_var(3)]) ){
				flag = true;
			}

		}

	} else if ( type < 18) {

		if (type == 16){

			// |x_var(0) - x_var(1)| < value
			if( abs(e[scope.get_var(0)] - e[scope.get_var(1)]) < value ){
				flag = true;
			}

		} else if (type == 17){

			// |x_var(0)/3 - x_var(1)/3| > value
			if( abs((int) (floor(e[scope.get_var(0)]/3) - floor(e[scope.get_var(1)]/3))) > value ){
				flag = true;
			}


		}

	} else if ( type < 24 ) {

		if ( type == 18 ) {

			if ( e[scope.get_var(0)] != value ) 
				flag = true;

		} else if ( type == 19 ){

			if ( e[scope.get_var(0)] == value ) 
				flag = true;

		} else if ( type == 20 ){
			
			if ( e[scope.get_var(0)] > value ) 
				flag = true;

		} else if ( type == 21 ){

			if ( e[scope.get_var(0)] < value ) 
				flag = true;

		} else if ( type == 22 ){

			if ( e[scope.get_var(0)] >= value ) 
				flag = true;

		} else if ( type == 23 ){

			if ( e[scope.get_var(0)] <= value ) 
				flag = true;

		}
		
	}

	return flag;

}

bool Constraint::satisfy(Query query){	// if Query query satisfies the constraint

	vector<int> e = query.get_vars();

	return satisfy(e);

}

// ONLY FOR BINARY CONSTRAINTS !!!!
bool Constraint::satisfy(int var1, int var2){	// if the two vars satisfy the constraint

	bool flag = false;

	if ( (type > 8 && type < 11) || (type > 12 && type < 16) || type > 17 ){
		cout << "Wrong satisfy function!! Only for binary constraints!" << endl;
		exit(-1);
	}

	if ( var1 != wrong && var2 != wrong ){

		if ( type == CONJUNCTION ){

			flag = true;	// initialize to true for this case

			int size = conjunction.size();

			for (int i = 0; i < size && flag; i++){	// if flag gets false then it stops
				flag = conjunction[i].satisfy(var1, var2);
			}

		} else if (type == 0){

			if( var1 != var2 ){
				flag = true;
			} 

		} else if (type == 1){

			if( var1 == var2 ){
				flag = true;
			}

		} else if (type == 2){

			if( var1 > var2 ){
				flag = true;
			}

		} else if (type == 3){

			if( var1 < var2 ){
				flag = true;
			}

		} else if (type == 4){

			if( var1 >= var2 ){
				flag = true;
			}

		} else if (type == 5){

			if( var1 <= var2 ){
				flag = true;
			}

		} else if (type == 6){

			if( (var1 - var2) == 1 ){
				flag = true;
			}

		} else if (type == 7){

			if( (var2 - var1) == 1 ){
				flag = true;
			}

		} else if (type == 8){

			if( abs(var1 - var2) == 1 ){
				flag = true;
			}

		} else if (type == 11){

			if( abs(var1 - var2) > value ){
				flag = true;
			}

		} else if (type == 12){

			if( abs(var1 - var2) == value ){
				flag = true;
			}

		} else if ( type < 18) {

			if (type == 16){

				// |x_var(0) - x_var(1)| > value
				if( abs(var1 - var2) < value ){
					flag = true;
				}

			} else if (type == 17){

				// |x_var(0) - x_var(1)| > value
				if( abs((int) (floor(var1/3) - floor(var2/3)) ) > value ){
					flag = true;
				}

			}

		} else {
			cout << "Error: Wrong type of constraint on satisfy() function!" << endl;
			exit(-1);
		}

	}

	return flag;

}

bool Constraint::satisfy(int var1){	// for unary constraints !!

	bool flag = false;


	if ( (type < 18 || type > 23) && type != CONJUNCTION){
		cout << "Wrong satisfy function!! Only for unary constraints!" << endl;
		exit(-1);
	}

	if ( var1 != wrong ){

		if ( type == CONJUNCTION ){

			flag = true;	// initialize to true for this case

			int size = conjunction.size();

			for (int i = 0; i < size && flag; i++){	// if flag gets false then it stops
				flag = conjunction[i].satisfy(var1);
			}

		} else 	if ( type == 18 ) {

			if ( var1 != value ) 
				flag = true;

		} else if ( type == 19 ){

			if ( var1 == value ) 
				flag = true;

		} else if ( type == 20 ){
			
			if ( var1 > value ) 
				flag = true;

		} else if ( type == 21 ){

			if ( var1 < value ) 
				flag = true;

		} else if ( type == 22 ){

			if ( var1 >= value ) 
				flag = true;

		} else if ( type == 23 ){

			if ( var1 <= value ) 
				flag = true;

		}

	}

	return flag;

}

// For Constraints of type 9 and 10, 14, 15 ( quaternary ) !
bool Constraint::satisfy(int var1, int var2, int var3, int var4){	// if the two vars satisfy the constraint

	bool flag = false;

	if (type != 9 && type != 10 && type != 14 && type != 15 && type != CONJUNCTION){
		cout << "Wrong satisfy function!! Only for types 9, 10, 14, 15!" << endl;
		exit(-1);
	}

	if ( var1 != wrong && var2 != wrong && var3 != wrong && var4 != wrong ){

		if ( type == CONJUNCTION ){

			flag = true;	// initialize to true for this case

			int size = conjunction.size();

			for (int i = 0; i < size && flag; i++){	// if flag gets false then it stops
				flag = conjunction[i].satisfy(var1, var2, var3, var4);
			}

		} else if (type == 9){

			// |x_var(0) - x_var(1)| != |x_var(2) - x_var(3)|
			if( abs(var1 - var2) != abs(var3 - var4) ){
				flag = true;
			}

		} else if (type == 10){

			// |x_var(0) - x_var(1)| == |x_var(2) - x_var(3)|
			if( abs(var1 - var2) == abs(var3 - var4) ){
				flag = true;
			}

		} else if (type == 14){

			// |x_var(0) - x_var(1)| > |x_var(2) - x_var(3)|
			if( abs(var1 - var2) > abs(var3 - var4) ){
				flag = true;
			}

		} else if (type == 15){

			// |x_var(0) - x_var(1)| < |x_var(2) - x_var(3)|
			if( abs(var1 - var2) < abs(var3 - var4) ){
				flag = true;
			}

		}

	}

	return flag;

}

// For Constraints of type 13 (ternary) !
bool Constraint::satisfy(int var1, int var2, int var3){	// if the three vars satisfy the constraint

	bool flag = false;

	if ( type != 13 && type != CONJUNCTION ){
		cout << "Wrong satisfy function!! Only for type 13!" << endl;
		exit(-1);
	}

	if ( var1 != wrong && var2 != wrong && var3 != wrong ){

		if ( type == CONJUNCTION ){

			flag = true;	// initialize to true for this case

			int size = conjunction.size();

			for (int i = 0; i < size && flag; i++){	// if flag gets false then it stops
				flag = conjunction[i].satisfy(var1, var2, var3);
			}

		} else if ( type == 13 ){

			// x_var(0) - x_var(1) != x_var(2)
			if( var1 - var2 != var3 ){
				flag = true;
			}

		} 

	}

	return flag;

}

bool Constraint::satisfy(int v[]){	// if these values (v[]) for the variables of its scope, satisfies the constraint

	bool flag = false;

	if ( type < 9 ) {

		if ( type == CONJUNCTION ){

			flag = true;	// initialize to true for this case

			int size = conjunction.size();

			for (int i = 0; i < size && flag; i++){	// if flag gets false then it stops
				flag = conjunction[i].satisfy(v);
			}

		} else if ( v[0] != wrong && v[1] != wrong ) {

			if (type == 0){

				if( v[0] != v[1] ){
					flag = true;					
				}

			} else if (type == 1){

				if( v[0] == v[1] ){
					flag = true;
				}

			} else if (type == 2){

				if( v[0] > v[1] ){
					flag = true;
				}

			} else if (type == 3){

				if( v[0] < v[1] ){
					flag = true;
				}

			} else if (type == 4){

				if ( v[0] >= v[1] ){
					flag = true;
				}

			} else if (type == 5){

				if( v[0] <= v[1] ){
					flag = true;
				}

			} else if (type == 6){

				if( (v[0] - v[1]) == 1 ){
					flag = true;
				}

			} else if (type == 7){

				if( (v[1] - v[0]) == 1 ){
					flag = true;
				}

			} else if (type == 8){

				if( abs(v[0] - v[1]) == 1 ){
					flag = true;
				}

			}

		}

	} else if ( type < 11) {

		if (type == 9){

			// |x_var(0) - x_var(1)| != |x_var(2) - x_var(3)|
			if( abs(v[0] - v[1]) != abs(v[2] - v[3]) ){
				flag = true;
			}

		} else if (type == 10){

			// |x_var(0) - x_var(1)| == |x_var(2) - x_var(3)|
			if( abs(v[0] - v[1]) == abs(v[2] - v[3]) ){
				flag = true;
			}

		}

	} else if ( type < 14) {

		if (type == 11){

			// |x_var(0) - x_var(1)| > value
			if( abs(v[0] - v[1]) > value ){
				flag = true;
			}

		} else if (type == 12){ 

	 		// |x_var(0) - x_var(1)| == value
	 		if( abs(v[0] - v[1]) == value ){
	 			flag = true;
	 		} 

		 } else if (type == 13){

			// x_var(0) - x_var(1) == x_var(2)
			if( v[0] - v[1] == v[2] ){
				flag = true;
			}

		}

	} else if ( type < 16) {

		if (type == 14){

			// |x_var(0) - x_var(1)| > |x_var(2) - x_var(3)|
			if( abs(v[0] - v[1]) > abs(v[2] - v[3]) ){
				flag = true;
			}

		} else if (type == 15){

			// |x_var(0) - x_var(1)| < |x_var(2) - x_var(3)|
			if( abs(v[0] - v[1]) < abs(v[2] - v[3]) ){
				flag = true;
			}

		}

	} else if ( type < 18) {

		if (type == 16){

			// |x_var(0) - x_var(1)| > value
			if( abs(v[0] - v[1]) < value ){
				flag = true;
			}

		} else if (type == 17){

			// |x_var(0) - x_var(1)| > value
			if( abs((int) (floor(v[0]/3) - floor(v[1]/3))) > value ){
				flag = true;
			}

		}


	} else if ( type < 24 ) {

                if ( type == 18 ) {

                        if ( v[0] != value )
                                flag = true;

                } else if ( type == 19 ){

                        if ( v[0] == value )
                                flag = true;

                } else if ( type == 20 ){

                        if ( v[0] > value )
                                flag = true;

                } else if ( type == 21 ){

                        if ( v[0] < value )
                                flag = true;

                } else if ( type == 22 ){

                        if ( v[0] >= value )
                                flag = true;

                } else if ( type == 23 ){

                        if ( v[0] <= value )
                                flag = true;

                }

    } else {
		cout << "Error: Wrong type of constraint on satisfy() function!" << endl;
		exit(-1);
	}


	return flag;

}

// WORKING ONLY FOR BINARY CONSTRAINTS, DO NOT USE !!
bool Constraint::propagate(vector<Variable> variable, int st){	// if the two vars of constraints scope can satisfy the constraint

	if ( satisfied != UNDEF )
		return true;


	bool flag = true;

	bool support = false;	// if the constraint can be satisfied
	bool supportviol = false;	// if the constraint can be violated

	int i, j, k, l;

	int var1 = scope.get_var(0);
	int var2 = scope.get_var(1);

	int var3, var4;

	int value1, value2, value3, value4;

	int x1,x2,x3,x4;

	x1 = variable[var1].get_domain_values().size();
	x2 = variable[var2].get_domain_values().size();

	if ( scope.get_size() == 4 ){

		var3 = scope.get_var(2);
		var4 = scope.get_var(3);

		x3 = variable[var3].get_domain_values().size();		
		x4 = variable[var4].get_domain_values().size();

	}

	for (i = 0; i < x1 && ( !support || !supportviol ); i++){

		if ( variable[var1].get_value() != wrong ){	
			x1 = 1;
			value1 = variable[var1].get_value();
		} else {

			if ( variable[var1].get_domain_state(i) != -1 )
				value1 = variable[var1].get_domain_value(i);
			else {
				continue;
			}

		}

		for (j = 0; j < x2 && ( !support || !supportviol ); j++){

			if ( variable[var2].get_value() != wrong ){	
				x2 = 1;
				value2 = variable[var2].get_value();
			} else {

				if ( variable[var2].get_domain_state(j) != -1 )
					value2 = variable[var2].get_domain_value(j);
				else {
					continue;
				}

			}

			if ( scope.get_size() == 4 ) {

				for (k = 0; k < x3 && ( !support || !supportviol ); k++){

					if ( variable[var3].get_value() != wrong ){	
						x3 = 1;
						value3 = variable[var3].get_value();
					} else {

						if ( variable[var3].get_domain_state(k) != -1 )
							value3 = variable[var3].get_domain_value(k);
						else {
							continue;
						}

					}

					for (l = 0; l < x4 && ( !support || !supportviol ); l++){

						if ( variable[var4].get_value() != wrong ){	
							x4 = 1;
							value4 = variable[var4].get_value();
						} else {

							if ( variable[var4].get_domain_state(l) != -1 )
								value4 = variable[var4].get_domain_value(l);
							else {
								continue;
							}
						}

						// check if the combination of the values chosen satisfy the constraint
						if ( satisfy( value1, value2, value3, value4 ) ){
							support = true;
						} else {
							supportviol = true;
						}

					}
				}

			} else if ( scope.get_size() == 2 ) {

				if ( satisfy( value1, value2 ) ){
					support = true;
				} else {
					supportviol = true;
				}	

			}

		}

	}

	if ( support && supportviol) {
		satisfied = UNDEF;
	} else if( support ) {
		satisfied = SAT;
		state = st;
	} else if( supportviol ) {
		satisfied = VIOL;
		state = st;
	} else {
		flag = false;
	}

	return flag;

}

bool Constraint::compare(Constraint con){	// compare with constraint con, only for binary constraints

	bool flag = true;

	if ( type != con.get_type() )	// if con is a constraint of other type
		flag = false;
	else 
		flag = compare_scope(con);

	if ( flag ) {
		
		if ( type == 11 || type == 12 || (type > 15 && type < 24) ) {
			if ( value != con.get_value() )
				flag = false;
		} else if ( type == 9 || type == 10 || type == 14 || type == 15 ){

			vector<short int> vars1 = get_scope_vars();
			vector<short int> vars2 = con.get_scope_vars();


			if ( vars1[0] == vars2[0] && vars1[1] == vars2[1] && vars1[2] == vars2[2] && vars1[3] == vars2[3] )
				flag = true;
			else if ( vars1[0] == vars2[2] && vars1[1] == vars2[3] && vars1[2] == vars2[0] && vars1[3] == vars2[1] )
				flag = true;
			else
				flag = false;
				

		} else if ( type == CONJUNCTION ){

			int size = conjunction.size();
			int size2 = con.get_conjunction_constraints().size();

			if ( size != size2 )
				flag = false;
			else {

				bool flag2 = false;

				for (int i = 0; i < size && flag; i++){ // until it finds one constraint that is not in con ...

					for (int j = 0; j < size2 && !flag2; j++){ // until it finds it (flag2 == true)

						if ( conjunction[i].compare( con.get_conjunction_constraint( j ) ) )
							flag2 = true;

					}

					if ( !flag2 )
						flag = false;

				}

			}

		}

	}

	return flag;

}

bool Constraint::compare_scope(Constraint con){	// compare scope with constraint con

	bool flag = scope.compare( con.get_scope() );

	return flag;

}

bool Constraint::compare_scope(Scope sc){	// compare scope with constraint con

	bool flag = scope.compare( sc );

	return flag;

}

// NOT WORKING FOR THE MOST OF THE CONSTRAINTS DO NOT USE, NO NEED TO USE IT !!!
Constraint Constraint::complement(){	// return the complementary of this constraint ( only for = and != until now)

	Constraint c;

	c.set_scope(scope);

	if ( type == 0 )
		c.set_type(1);
	else if ( type == 1 )
		c.set_type(0);

	return c;

}

void Constraint::to_conjunction(){	// make the constraint a conjunction constraint

	if ( type != CONJUNCTION ){

		Constraint con(this->type, this->scope);

		if ( type == 11 || type == 12 || (type > 15 && type < 22))
			con.set_value(value);

		conjunction.push_back(con);

		type = CONJUNCTION;

	}

}

void Constraint::print(){	// print the constraint to user friendly

int in;

	if (type == CONJUNCTION ){

		int size = conjunction.size();

		cout << "Conjuction constraint. It contains:" << endl;

		for (int i = 0; i < size; i++){
			conjunction[i].println();
		}

	} else if ( type == -1 ){
		cout << "Type = -1 " << endl;
		cin >> in;	
	} else if ( type < 6 )
		cout << scope.get_var(0) << " " << con << " " << scope.get_var(1);
	else if ( type == 6 )
		cout << scope.get_var(0) << " - " << scope.get_var(1) << " = 1";
	else if ( type == 7 )
		cout << scope.get_var(1) << " - " << scope.get_var(0) << " = 1";
	else if ( type == 8 )
		cout << "|" << scope.get_var(0) << " - " << scope.get_var(1) << "| = 1";		
	else if ( type == 9 )
		cout << "|" << scope.get_var(0) << " - " << scope.get_var(1) << "| != |" << scope.get_var(2) << " - " << scope.get_var(3) << "|";	
	else if ( type == 10 )
		cout << "|" << scope.get_var(0) << " - " << scope.get_var(1) << "| == |" << scope.get_var(2) << " - " << scope.get_var(3) << "|";
	else if ( type == 11 )
		cout << "|" << scope.get_var(0) << " - " << scope.get_var(1) << "| > " << value;	
	else if ( type == 12 )
		cout << "|" << scope.get_var(0) << " - " << scope.get_var(1) << "| == " << value;	
	else if ( type == 13 )
		cout << scope.get_var(0) << " - " << scope.get_var(1) << " != " << scope.get_var(2);	
	else if ( type == 14 )
		cout << "|" << scope.get_var(0) << " - " << scope.get_var(1) << "| > |" << scope.get_var(2) << " - " << scope.get_var(3) << "|";	
	else if ( type == 15 )
		cout << "|" << scope.get_var(0) << " - " << scope.get_var(1) << "| < |" << scope.get_var(2) << " - " << scope.get_var(3) << "|";
	else if ( type == 16 )
		cout << "|" << scope.get_var(0) << " - " << scope.get_var(1) << "| < " << value;	
	else if ( type == 17 )
		cout << "|floor(" << scope.get_var(0) << "/3) - floor(" << scope.get_var(1) << "/3)| > " << value;		
	else if ( type < 24 )
		cout << "x" << scope.get_var(0) << " " << con << " " << value;

}

void Constraint::println(){	// print the constraint to user friendly, with end line
	print();
	cout << endl;
}

void Constraint::write(ofstream *fout){

	*fout << type;

	for (int i = 0; i < scope.get_size(); i++)
		*fout << " " << scope.get_var(i);

	if ( type == 11 || type == 12 || type > 15)
		*fout << " " << value;

}

void Constraint::clear(){	// clear 

	type = -1;
	value = 0;

	scope.clear();
	con.clear();
	
}


// Class Constraint_Net methods --------------------------------------------------------------------------------------

	// Constructor --------------------------

Constraint_Net::Constraint_Net(){
	
	size = 0;

	nb_sat = 0;
	nb_undef = 0;
	nb_viol = 0;	
	
}

Constraint_Net::Constraint_Net(int var){

	initialize_bias(var);

}


	// set - push ---------------------------

void Constraint_Net::set_network(vector<Constraint> c){
	constraints = c;
	size = c.size();
}

void Constraint_Net::push_con(Constraint c){
	constraints.push_back(c);
	size++;
	nb_undef++;
}

void Constraint_Net::push_con(Constraint_Net Cn){		// push constraint network Cn to the network

	for (int i = 0; i < Cn.get_size(); i++)
		push_con( Cn.get_con(i) );

}


void Constraint_Net::increase_con_weight(int c){
	constraints[c].increase_weight();
}

void Constraint_Net::increase_inst_vars(int c){	// increase instantiated variables by 1, to con c
	constraints[c].increase_inst_vars();
}

void Constraint_Net::decrease_inst_vars(int c){	// decrease instantiated variables by 1, to con c
	constraints[c].decrease_inst_vars();
}

// NOT USED ----------------------------------------
void Constraint_Net::check_cons(){		// update nb_sat, nb_undef, nb_viol

	nb_sat = 0;
	nb_undef = 0;
	nb_viol = 0;

	for (int i = 0; i < size; i++){

		if ( constraints[i].get_satisfied() == SAT ){
			nb_sat++;
		} else if ( constraints[i].get_satisfied() == UNDEF ){
			nb_undef++;
		} else if ( constraints[i].get_satisfied() == VIOL ){
			nb_viol++;
		}

	}
	
}

	// get ----------------------------------

vector<Constraint> Constraint_Net::get_network(){
	return constraints;
}

vector<Constraint> Constraint_Net::get_constraints(){
	return constraints;
}

vector<short int> Constraint_Net::get_variables(){ // return the variables that included in the scope of at least one constraint

	vector<short int> variables;

	int scope_size;

	short int var;

	for (int i = 0; i < constraints.size(); i++){

		scope_size = constraints[i].get_scope_size();

		for (short int j = 0; j < scope_size; j++){

			var = constraints[i].get_var(j);

			if ( find(variables.begin(), variables.end(), var ) == variables.end() )
				variables.push_back(var);

		}

	}

	return variables;

}

Constraint Constraint_Net::get_con(int i){
	return constraints[i];
}

Constraint_Net Constraint_Net::get_scope(Scope scope){	// return all the constraints with Scope scope

	Constraint_Net tmp;

	for (int i = 0; i < size; i++)
		if (constraints[i].compare_scope(scope) )
			tmp.push_con(constraints[i]);

	return tmp;

}

Constraint_Net Constraint_Net::get_scope(vector<short int> scope){	// return all the constraints with Scope scope

	Scope s(scope);

	return get_scope(s);

}


Constraint_Net Constraint_Net::get_subnetwork(Scope scope){	// return all the constraints with a scope var(c) \in scope

	Constraint_Net tmp;

	for (int i = 0; i < size; i++)
		if ( constraints[i].get_scope().subset_of(scope) || constraints[i].compare_scope(scope) )
			tmp.push_con(constraints[i]);

	return tmp;

}

Constraint_Net Constraint_Net::get_subnetwork(vector<short int> scope){	// return all the constraints with a scope var(c) \in scope

	Scope s(scope);

	return get_subnetwork(s);

}

int Constraint_Net::get_size(){
	return size;
}

int Constraint_Net::get_max_conjunction_size(){
	
	int max = 0;

	for (int i = 0; i < size; i++)
		if ( constraints[i].get_conjunction_size() > max)
			max = constraints[i].get_conjunction_size();

	return max;

}

int Constraint_Net::get_min_conjunction_size(){
	
	int min = 99;

	for (int i = 0; i < size; i++)
		if ( constraints[i].get_conjunction_size() < min)
			min = constraints[i].get_conjunction_size();

	return min;

}

int Constraint_Net::get_con_weight(int c){
	return constraints[c].get_weight();
}

int Constraint_Net::get_nb_sat(){	// get number of constraints being satisfied
	return nb_sat;
}

int Constraint_Net::get_nb_undef(){	// get number of constraints undefined if satisfied
	return nb_undef;
}

int Constraint_Net::get_nb_viol(){	// get number of constraints violated
	return nb_viol;
}

Constraint_Net Constraint_Net::get_rejects(Query query){	// return k_B(query)

	int var1, var2, var3, var4;

	Constraint_Net Kappa;

	for (int i = 0; i < size; i++){	// for each constraint in the constraint network

		if ( constraints[i].get_scope_size() == 2 ) {	// constraint types 0 - 9, 11 - 12, 16 - 17

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong ) {

				if ( !constraints[i].satisfy( query.get_var(var1), query.get_var(var2) ) )
					Kappa.push_con(constraints[i]);
				
			}

		} else if ( constraints[i].get_scope_size() == 4 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);
			var4 = constraints[i].get_var(3);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong && query.get_var(var4) != wrong ) {

				if ( !constraints[i].satisfy( query ) )
					Kappa.push_con(constraints[i]);

			}

		} else if ( constraints[i].get_scope_size() == 3 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong ) {

				if ( !constraints[i].satisfy( query.get_var(var1), query.get_var(var2), query.get_var(var3) ) )
					Kappa.push_con(constraints[i]);

			}		

		} else if  ( constraints[i].get_scope_size() == 1 ){

                        var1 = constraints[i].get_var(0);

                        if ( query.get_var(var1) != wrong ) {

                                if ( !constraints[i].satisfy( query.get_var(var1) ) ){
                                        Kappa.push_con(constraints[i]);
                                }

                        }
                }


	}

	return Kappa;

}

Constraint_Net Constraint_Net::get_accepts(Query query){	// return l_B(query)

	int var1, var2, var3, var4;

	Constraint_Net Sat;

	for (int i = 0; i < size; i++){	// for each constraint in the constraint network

		if ( constraints[i].get_scope_size() == 2 ) {	// constraint types 0 - 9, 11 - 12

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong ) {

				if ( constraints[i].satisfy( query.get_var(var1), query.get_var(var2) ) )
					Sat.push_con(constraints[i]);
				
			}

		} else if ( constraints[i].get_scope_size() == 4 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);
			var4 = constraints[i].get_var(3);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong && query.get_var(var4) != wrong ) {

				if ( constraints[i].satisfy( query ) )
					Sat.push_con(constraints[i]);

			}

		} else if ( constraints[i].get_scope_size() == 3 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong ) {

				if ( constraints[i].satisfy( query.get_var(var1), query.get_var(var2), query.get_var(var3) ) )
					Sat.push_con(constraints[i]);

			}		

		}

	}

	return Sat;

}

	// other methods ------------------------

bool Constraint_Net::contains(Constraint con){	// if this constraint network contains the constraint ``con''

	for (int i = 0; i < size; i++)		
		if ( constraints[i].compare(con) )
			return true;

	return false;

}

void Constraint_Net::joint(Constraint_Net net){	// joint with constraint network net

	Constraint temp, net_con;

	Constraint_Net temp_net;

	for (int i = 0; i < size; i++){	// for each constraint in the constraint network

		Constraint con = constraints[i];

		con.to_conjunction();

		for (int j = 0; j < net.get_size(); j++){

			temp.clear();
			temp = con;	// to be able to temporary change it

			net_con = net.get_con(j);

			net_con.to_conjunction();	// if not already a conjunction, make it

			for (int l = 0; l < net_con.get_conjunction_constraints().size(); l++){	// for each constraint of the conjunction

				bool flag = true;	// assume that it is not already included in the conjunction of the constraint

				for (int k = 0; k < temp.get_conjunction_constraints().size() && flag; k++){

					if ( temp.get_conjunction_constraint(k).compare( net_con.get_conjunction_constraint(l) ) )
						flag = false;
				
				}

				if ( flag )
					temp.push_conjunction_constraint( net_con.get_conjunction_constraint(l) );	// if it does not exist to the conjunction, add it

			}

			bool flag = true; // assume that it is not already included in the temp_net

			for (int n = 0; n < temp_net.get_size() && flag; n++){ // checking if this conjunction already exists in temp_net
				if ( temp_net.get_con(n).compare(temp) )
					flag = false;
			}

			if ( flag )
				temp_net.push_con(temp);

		}

	}

	*this = temp_net;

}


// NOT USED ------------------------------------------
void Constraint_Net::propagate(int i, vector<Variable> variable, int st){	// propagate constraint i. State: st

	int prev_sat = constraints[i].get_satisfied();
	int sat;

	if ( constraints[i].get_state() == -1 ) {
		
		constraints[i].propagate(variable, st);

		sat = constraints[i].get_satisfied();

		if ( sat == SAT ) {

			if ( prev_sat == UNDEF ){
				nb_sat++;
				nb_undef--;
			} 

		} else if ( sat == VIOL ) {

			if ( prev_sat == UNDEF ){
				nb_viol++;
				nb_undef--;
			}

		}

	}

}

void Constraint_Net::restore(int state){	// restore reified constraints from state ...

	int sat;

	for (int i = 0; i < size; i++){

		if ( constraints[ i ].get_state() == state ){

			sat = constraints[ i ].get_satisfied();

			if ( sat == SAT )
				nb_sat--;
			else if ( sat == VIOL )
				nb_viol--;

			constraints[ i ].set_satisfied(UNDEF);
			constraints[ i ].set_state(-1);

			nb_undef++;

		} 

	}

}

void Constraint_Net::clear(){
	constraints.clear();
	size = 0;
}

void Constraint_Net::remove_con(int i){	// remove the constraint i from network
	constraints.erase(constraints.begin() + i );
	size--;
}

bool Constraint_Net::remove_con(Constraint c){	// remove the constraint c from network

	int i;

	Constraint temp;

	for (i = 0; i < size; i++){	// for every constraint in the current constraint network

		if ( constraints[i].compare( c ) ){		// if the constraint already exists in cl

			remove_con(i);
			i--;

			return true;
		}
	}

	return false;

}

void Constraint_Net::remove_scope(Scope scope){	// remove all the constraints with Scope scope

	for (int i = 0; i < size; i++){

		if (constraints[i].compare_scope(scope) ){
			remove_con(i);
			i--;
		}

	}

}


void Constraint_Net::remove_scope(vector<short int> scope){	// remove all the constraints with Scope scope

	Scope s(scope);

	remove_scope(s);

}

void Constraint_Net::initialize_bias(int var){

	Constraint *c;

	if ( benchmark.compare(0,7,"golomb_") == 0){

		for (int i = 1; i < VAR_NUM; i++){
			for (int j = 0; j < i; j++){
				for (int x = i; x < VAR_NUM; x++){
					for (int y = 0; y < x; y++){

						if ( (y != i && x != j && x != i && y != j) || (y != i && x != j && x == i && y != j) || (y != i && x != j && x != i && y == j)  ){

							// for the quaternary distance constraints, types 9, 10
							for (int t = 9; t < 11; t++){		

								c = new Constraint(t);

								c->push_var(i);
								c->push_var(j);
								c->push_var(x);
								c->push_var(y);

								constraints.push_back(*c);

							}

							// for the quaternary distance constraints, types 14, 15
							for (int t = 14; t < 16; t++){		

								c = new Constraint(t);

								c->push_var(i);
								c->push_var(j);
								c->push_var(x);
								c->push_var(y);

								constraints.push_back(*c);

							}

						}

					}
				}

			}
		}


		Constraint c1, c2;

		for (int i = 0; i < constraints.size() - 1; i++ ){

			c1 = constraints[i];

			for (int j = i + 1; j < constraints.size(); j++ ){

				c2 = constraints[j];

				if ( c1.compare(c2) ){

					remove_con(j);
					j--;
				}

			}

		}



	}


	for (int i = 0; i < var; i++){

				if ( benchmark.compare("zebra") == 0){

					for (int t = 18; t < 24; t++){		// for the unary constraints, types 16 - 21
						for (int x = 0; x < VAL_NUM; x++){
						
							c = new Constraint(t);

							c->push_var(i);

							c->set_value(condition_values[x]);

							constraints.push_back(*c);

						}
					}

				}

		for (int j = 0; j < var; j++){

			if ( i < j ) {

//				if ( benchmark.compare("rlfap") != 0 ){
//--
					for (int t = 0; t < 6; t++){		// 4 : !=, =, >, < or 6: !=, =, >, <, >=, <=

						c = new Constraint(t);

						c->push_var(i);
						c->push_var(j);

						constraints.push_back(*c);

					}

//				}

				if ( benchmark.compare("zebra") == 0 ){

					for (int t = 6; t < 9; t++){		// for the distance constraints, types 6, 7, 8

						c = new Constraint(t);

						c->push_var(i);
						c->push_var(j);

						constraints.push_back(*c);

					}

				}

				if ( benchmark.compare("rlfap") == 0 || benchmark.compare(0,11,"timetabling") == 0 ){

					for (int t = 11; t < 13; t++){		// for the distance constraints, types 11 - 12
						for (int x = 0; x < VAL_NUM; x++){
						
							c = new Constraint(t);

							c->push_var(i);
							c->push_var(j);

							c->set_value(condition_values[x]);

							constraints.push_back(*c);

						}
					}

				}
				
				if ( benchmark.compare(0,11,"timetabling") == 0 ){

					for (int t = 16; t < 18; t++){		// for the distance constraints, types 16 - 17
						for (int x = 0; x < VAL_NUM; x++){

							c = new Constraint(t);

							c->push_var(i);
							c->push_var(j);

							c->set_value(condition_values[x]);

							constraints.push_back(*c);

						}
					}				

				}

			}
		}

	}

	size = constraints.size();

	cout << "\n\nbias size = " << size << endl;

	for (int i = 0; i < size; i++){

		if (constraints[i].get_scope().get_size() == 1) {

			constraints[i].println();

		}

	}

	nb_sat = 0;
	nb_undef = size;
	nb_viol = 0;

}

void Constraint_Net::print(){

	cout << "Constraint_Net with size " << size << " : " << endl << endl;

	for (int i = 0; i < constraints.size(); i++){
		constraints[i].print();
		cout << endl;
	}

	cout << endl;
}

int Constraint_Net::count_rejects(Query query){	// count how many constraints reject a query

	int counter = 0;

	int var1, var2, var3, var4;

	for (int i = 0; i < size; i++){	// for each constraint in the constraint network

 		if ( constraints[i].get_scope_size() == 2 ) {

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong ) {

				if ( !constraints[i].satisfy( query ) )
					counter++;

			}

		} else if ( constraints[i].get_scope_size() == 4 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);
			var4 = constraints[i].get_var(3);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong && query.get_var(var4) != wrong ) {

				if ( !constraints[i].satisfy(  query ) )
					counter++;

			}

		} else if ( constraints[i].get_scope_size() == 3 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong ) {

				if ( !constraints[i].satisfy( query ) )
					counter++;

			}		

		} else if  ( constraints[i].get_scope_size() == 1 ){

                        var1 = constraints[i].get_var(0);

                        if ( query.get_var(var1) != wrong ) {

                                if ( !constraints[i].satisfy( query ) ){
                                        counter++;
                                }

                        }
                }


	} 

	return counter;

} // end of Constraint_Net::count_rejects()

int Constraint_Net::count_rejects(vector<Variable> variable){	// count how many constraints reject a query

	Query query;

	query.set_state(1);

	for (int i = 0; i < variable.size(); i++)
		query.set_var(variable[i].get_value(), i);

	return count_rejects(query);

}

bool Constraint_Net::rejects(Query query){	// true if at least one constraint reject query

	bool rejects = false;

	int var1, var2, var3, var4;

	int i;

	for (i = 0; i < size && !rejects; i++){	// for each constraint in the constraint network

		if ( constraints[i].get_scope_size() == 2 ) {

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong ) {

				if ( !constraints[i].satisfy( query.get_var(var1), query.get_var(var2) ) )
					rejects = true;

			}

		} else if ( constraints[i].get_scope_size() == 4 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);
			var4 = constraints[i].get_var(3);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong && query.get_var(var4) != wrong ) {

				if ( !constraints[i].satisfy( query.get_vars() ) ) {
					rejects = true;
				
				}
			}
		
		} else if ( constraints[i].get_scope_size() == 3 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong ) {

				if ( !constraints[i].satisfy( query.get_var(var1), query.get_var(var2), query.get_var(var3) ) )
					rejects = true;

			}		

		} else if  ( constraints[i].get_scope_size() == 1 ){

                        var1 = constraints[i].get_var(0);

                        if ( query.get_var(var1) != wrong ) {

                                if ( !constraints[i].satisfy( query.get_var(var1) ) ){
                                        rejects = true;
                                }

                        }
                }

	}

	return rejects;

}

void Constraint_Net::remove_rejects(Query query){ // remove the constraints that reject a query

	int var1, var2, var3, var4;

	for (int i = 0; i < size; i++){	// for each constraint in the constraint network

		if ( constraints[i].get_scope_size() == 2 ) {	// constraint types 0 - 9, 11 - 12

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong ) {

				if ( !constraints[i].satisfy( query.get_var(var1), query.get_var(var2) ) ){
					remove_con(i);
					i--;
				}
			}

		} else if ( constraints[i].get_scope_size() == 4 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);
			var4 = constraints[i].get_var(3);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong && query.get_var(var4) != wrong ) {

				if ( !constraints[i].satisfy( query ) ){
					remove_con(i);
					i--;
				}

			}

		} else if ( constraints[i].get_scope_size() == 3 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong ) {

				if ( !constraints[i].satisfy( query.get_var(var1), query.get_var(var2), query.get_var(var3) ) ){
					remove_con(i);
					i--;
				}

			}		

		} else if  ( constraints[i].get_scope_size() == 1 ){

                        var1 = constraints[i].get_var(0);

                        if ( query.get_var(var1) != wrong ) {

                                if ( !constraints[i].satisfy( query.get_var(var1) ) ){
                                        remove_con(i);
                                        i--;
                                }

                        }
		}

	} 

}

void Constraint_Net::remove_accepts(Query query){ // remove the constraints that accept a query

	int var1, var2, var3, var4;

	for (int i = 0; i < size; i++){	// for each constraint in the constraint network

		if ( constraints[i].get_scope_size() == 2 ) {	// constraint types 0 - 9, 11 - 12

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong ) {

				if ( constraints[i].satisfy( query.get_var(var1), query.get_var(var2) ) ){
					remove_con(i);
					i--;
				}
			}

		} else if ( constraints[i].get_scope_size() == 4 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);
			var4 = constraints[i].get_var(3);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong && query.get_var(var4) != wrong ) {

				if ( constraints[i].satisfy( query ) ){
					remove_con(i);
					i--;
				}

			}

		} else if ( constraints[i].get_scope_size() == 3 ){

			var1 = constraints[i].get_var(0);
			var2 = constraints[i].get_var(1);
			var3 = constraints[i].get_var(2);

			if ( query.get_var(var1) != wrong && query.get_var(var2) != wrong && query.get_var(var3) != wrong ) {

				if ( constraints[i].satisfy( query.get_var(var1), query.get_var(var2), query.get_var(var3) ) ){
					remove_con(i);
					i--;
				}
				
			}		

		} else if  ( constraints[i].get_scope_size() == 1 ){

                        var1 = constraints[i].get_var(0);

                        if ( query.get_var(var1) != wrong ) {

                                if ( constraints[i].satisfy( query.get_var(var1) ) ){
                                        remove_con(i);
                                        i--;
                                }

                        }
                }


	}

}

// remove the constraints that exists to Cn
void Constraint_Net::remove_existing(Constraint_Net Cn){

	int i, j;

	bool flag;

	for (i = 0; i < size; i++){	// for every constraint in the current constraint network

		flag = false;

		for (j = 0; j < Cn.get_size() && !flag; j++){	// for every constraint in cl

			if ( constraints[i].compare( Cn.get_con(j) ) ){		// if the constraint already exists in cl

				remove_con(i);
				flag = true;
				i--;

			}

		}

	}

}

// remove the constraints with scope 'scope' that already exists to cl
void Constraint_Net::remove_existing(Constraint_Net cl, vector<short int> scope){

	int i, j;

	Constraint temp;

	temp.set_scope(scope);

	for (i = 0; i < size; i++){	// for every constraint in the current constraint network

		if ( constraints[i].compare_scope(temp) ) {	// check only constraint with scope 'scope'

			for (j = 0; j < cl.get_size(); j++){	// for every constraint in cl

				if ( constraints[i].compare( cl.get_con(j) ) ){		// if the constraint already exists in cl

					remove_con(i);
					i--;

				}

			}

		}

	}

}


	// read - write ---------------------------------------------------------------

bool Constraint_Net::read(){		// read from a file given by user

	string filename;

	cout << "Please give the filename of the constraint network: " << endl;
	cin >> filename;

	return read(filename);
}


bool Constraint_Net::read(string filename){	// read from file 'filename'

	ifstream fin(filename.c_str());
	string line;

	Constraint c;

	int i;

	int tmp;

	int con;

	string tmp2;

	// save constraints
	while ( getline(fin,line) ) {

		if ( line.find(" ") != string::npos ){

			// take the type of the constraint
			con = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int

			if ( con < 0 || con > 21 ){
				clear();
				return false;
			} else if ( con < 9 ){

				for (i = 0; i < 2; i++){

					// remove from the line anything until ' '
					line = line.substr(line.find(" ")+1);

					// take variable i
					tmp = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int

					if ( tmp >= VAR_NUM ){
						clear();
						cout << "Error reading the constraints, variable does not exist\n";
						exit(-1);
					}

					c.push_var(tmp);

				}

			} else if ( con < 11 ){

				for (i = 0; i < 4; i++){

					// remove from the line anything until ' '
					line = line.substr(line.find(" ")+1);

					// take variable i
					tmp = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int
					c.push_var(tmp);

					if ( tmp >= VAR_NUM ){
						clear();
						cout << "Error reading the constraints, variable does not exist\n";
						exit(-1);
					}

				}

			} else if ( con < 13 || con == 16 || con == 17 ){

				for (i = 0; i < 2; i++){

					// remove from the line anything until ' '
					line = line.substr(line.find(" ")+1);

					// take variable i
					tmp = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int

					if ( tmp >= VAR_NUM ){
						clear();
						cout << "Error reading the constraints, variable does not exist\n";
						exit(-1);
					}

					c.push_var(tmp);

				}

				// get the value of the condition -------------------------------

				// remove from the line anything until ' '
				line = line.substr(line.find(" ")+1);

				// take variable i
				tmp = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int				

				c.set_value(tmp);

				// --------------------------------------------------------------

			} else if ( con == 13 ){

				for (i = 0; i < 3; i++){

					// remove from the line anything until ' '
					line = line.substr(line.find(" ")+1);

					// take variable i
					tmp = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int

					if ( tmp >= VAR_NUM ){
						clear();
						cout << "Error reading the constraints, variable does not exist\n";
						exit(-1);
					}

					c.push_var(tmp);

				}

			} else if ( con < 16 ){

				for (i = 0; i < 4; i++){

					// remove from the line anything until ' '
					line = line.substr(line.find(" ")+1);

					// take variable i
					tmp = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int
					c.push_var(tmp);

					if ( tmp >= VAR_NUM ){
						clear();
						cout << "Error reading the constraints, variable does not exist\n";
						exit(-1);
					}

				}

			} else if ( con < 24 ){

				// remove from the line anything until ' '
				line = line.substr(line.find(" ")+1);

				// take variable i
				tmp = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int

				if ( tmp >= VAR_NUM ){
					clear();
					cout << "Error reading the constraints, variable does not exist\n";
					exit(-1);
				}

				c.push_var(tmp);


				// get the value of the condition -------------------------------

				// remove from the line anything until ' '
				line = line.substr(line.find(" ")+1);

				// take variable i
				tmp = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int				

				c.set_value(tmp);

				// --------------------------------------------------------------



			}
			
			c.set_type(con);

			push_con(c);

			c.clear();
		}
	}

	fin.close();

	return true;
}


void Constraint_Net::write(){	// write to file given by user

	string filename;

	cout << "Please give the filename of the constraint network: " << endl;
	cin >> filename;

	write(filename);

}


void Constraint_Net::write(string filename){	// write to file 'filename'

	ofstream fout(filename.c_str());

	fout << "Constraint_Net with size " << size << " : " << endl << endl;

	for (int i = 0; i < constraints.size(); i++){
		constraints[i].write(&fout);
		fout << endl;
	}

	fout << endl;

}



