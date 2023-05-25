#include <iostream>
#include <vector>

#include "variable.h" 
#include "globals.h" 

using namespace std;

// Class Domain methods -------------------------------------------------------------------------------

	// constructors ----------------------------------

Domain::Domain(){		// default: size of domains : DOM_SIZE, value[i] = i+1
				// default : state = -1

	for (int i = 0; i < DOM_SIZE; i++){

		value.push_back(i+1);
		state.push_back(-1);

	}

	size = DOM_SIZE;

}

Domain::Domain(vector<int> v){		// default : state = -1

	value = v;

	size = value.size();

	for (int i = 0; i < size; i++)
		state.push_back(-1);	

}

Domain::Domain(vector<int> v, vector<int> s){

	value = v;
	state = s;

	size = value.size();
}

void Domain::initialize(){

	if ( value.size() > 0 ) {

		for (int i = 0; i < value.size(); i++){

			state[i] = -1;

		}

		size = value.size();

	} else{

		value.clear();

		state.clear();

		for (int i = 0; i < DOM_SIZE; i++){

			value.push_back(i+1);
			state.push_back(-1);

		}

		size = DOM_SIZE;

	}

}

void Domain::initialize(int v){ // Initialize Domain of variable v

	value.clear();
	state.clear();

	size = DomainsSize[ DomainOfVar[v] ];

	for (int i = 0; i < size; i++){
		value.push_back(Domains[ DomainOfVar[v] ][ i ]);
		state.push_back(-1);
	}

}

void Domain::initialize(vector<int> v){

	value = v;

	size = value.size();

	state.clear();

	for (int i = 0; i < size; i++)
		state.push_back(-1);	

}

void Domain::initialize(vector<int> v, vector<int> s){

	value = v;
	state = s;

	size = value.size();

}

void Domain::clear(){
	initialize();
}


	// set -------------------------------------------

void Domain::set_value(int v, int i){	// v: value, i : indicator
	value[i] = v;
}

void Domain::set_state(int s, int i){	// s: state, i : indicator

	if ( s == -1 && state[i] != -1){
		size++;
	}

	if (s != -1 && state[i] == -1 ){
		size--;
	}

	state[i] = s;

}

void Domain::set_values(vector<int> v){	// set all values

	value = v;

	size = value.size();

	for (int i = 0; i < size; i++)
		state.push_back(-1);	

}

void Domain::set_states(vector<int> s){	// set all states

	size = state.size();

	for (int i = 0; i < size; i++){

		set_state(s[i], i);

	}
}


	// get -------------------------------------------

int Domain::get_value(int i){	// return value i
	return value[i];
}

int Domain::get_state(int i){	// return state i
	return state[i];
}

int Domain::get_size(){	// return the size of the domain
	return size;
}

vector<int> Domain::get_values(){	// return all values
	return value;
}

vector<int> Domain::get_states(){	// return all states
	return state;
}

// others ----------------------------------------

void Domain::print(){	// print Domain

	cout << "Domain size: " << value.size() << ", Domain current size: " << size << endl;
	cout << "Domain | Value | state" << endl;

	for (int i = 0; i < value.size(); i++){
		cout << i << "| " << value[i] << "| " << state[i] << endl;
	}

	cout << endl;
}


// Class Variable methods -----------------------------------------------------------------------------

	// constructors ----------------------------------

Variable::Variable(){		// default: size of domains : DOM_SIZE, domain[i] = i

	value = wrong;

	constraint_size = 0;
	bias_constraint_size = 0;

	in_ac3_queue = false;

	wdeg = 0;

	bwdeg = 0;

}
		
Variable::Variable(int v){	// v: variable indicator

	value = wrong;

	domain.initialize(v);

	constraint_size = 0;
	bias_constraint_size = 0;

	in_ac3_queue = false;

	wdeg = 0;

	dom_wdeg = 0;

	bwdeg = 0;

}

Variable::Variable(int v, Domain d){

	value = v;
	domain = d;

	constraint_size = 0;
	bias_constraint_size = 0;

	in_ac3_queue = false;

	wdeg = 0;

	dom_wdeg = 0;

	bwdeg = 0;

}

void Variable::initialize(){

	domain.initialize();

	constraint.clear();
	bias_constraint.clear();

	value = wrong;

	constraint_size = 0;
	bias_constraint_size = 0;

	in_ac3_queue = false;

	wdeg = 0;

	dom_wdeg = 0;

	bwdeg = 0;

}

void Variable::initialize(int v){ // v: variable indicator

	initialize();

	domain.initialize(v);

}

void Variable::initialize(int v, Domain d){

	constraint.clear();
	bias_constraint.clear();

	value = v;

	domain = d;

	constraint_size = 0;
	bias_constraint_size = 0;

	in_ac3_queue = false;

	wdeg = 0;

	dom_wdeg = 0;

	bwdeg = 0;


}

void Variable::clear(){

	domain.clear();

	constraint.clear();
	bias_constraint.clear();

	value = wrong;

	constraint_size = 0;
	bias_constraint_size = 0;

	in_ac3_queue = false;

	wdeg = 0;

	bwdeg = 0;

	dom_wdeg = 0;

}

	// set -------------------------------------------

void Variable::set_value(int v){
	value = v;
}

void Variable::push_con(int c){
	constraint.push_back(c);
	constraint_size++;
}

void Variable::push_bias_con(int c){
	bias_constraint.push_back(c);
	bias_constraint_size++;
}

void Variable::set_domain(Domain d){
	domain = d;
}

void Variable::set_domain_value(int v, int i){ 	// v: domain value, i : indicator
	domain.set_value(v, i);
}

void Variable::set_domain_state(int s, int i){	// s: state, i : indicator
	domain.set_state(s, i);
}

void Variable::set_domain_values(vector<int> v){ 	// v: all domain values
	domain.set_values(v);
}

void Variable::set_domain_states(vector<int> s){		// s: all states
	domain.set_states(s);
}

void Variable::set_in_queue(bool q){
	in_ac3_queue = q;
}

void Variable::set_wdeg(int w){
	wdeg = w;
}

void Variable::calc_dom_wdeg(){		// calculate dom/wdeg

	if ( wdeg > 0)
		dom_wdeg = (double) domain.get_size() / wdeg;
	else
		dom_wdeg = (double) domain.get_size();

}

void Variable::set_bwdeg(int bw){
	bwdeg = bw;
}

void Variable::increase_bwdeg(){
	bwdeg++;
}

void Variable::decrease_bwdeg(){
	bwdeg--;
}

	// get -------------------------------------------

int Variable::get_value(){		// return variable value
	return value;
}

int Variable::get_con(int i){	// return constraint[i]
	return constraint[i];
}

int Variable::get_bias_con(int i){	// return bias_constraint[i]
	return bias_constraint[i];
}

int Variable::get_con_size(){		// return the size of constraint vector
	return constraint_size;
}

int Variable::get_bias_con_size(){		// return the size of constraint vector
	return bias_constraint_size;
}

Domain Variable::get_domain(){		// return variable's domain
	return domain;
}

int Variable::get_domain_value(int i){ 	// return domain value i
	return domain.get_value(i);
}

int Variable::get_domain_state(int i){	// return state of domain value i
	return domain.get_state(i);
}

int  Variable::get_domain_size(){		// return the current size of the domain of the variable
	return domain.get_size();
}

vector<int> Variable::get_domain_values(){ 	// return all domain values
	return domain.get_values();
}

vector<int> Variable::get_domain_states(){	// return all states of domain values
	return domain.get_states();
}

bool Variable::get_in_queue(){
	return in_ac3_queue;
}

int Variable::get_wdeg(){				// return wdeg of the variable
	return wdeg;
}

double Variable::get_dom_wdeg(){			// return dom/wdeg of the variable
	return dom_wdeg;
}

int Variable::get_bwdeg(){				// return wdeg of the variable
	return bwdeg;
}

void Variable::print(){

	cout << "Value: " << value << endl;

	cout << "Domain: " << endl;

	domain.print();

	cout << "Cl constraints involved: " << constraint_size << endl;

	cout << "Bias constraints involved: " << bias_constraint_size << endl;

}