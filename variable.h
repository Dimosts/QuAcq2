#include <vector>
#include <string>

using namespace std;

// Classes...........................................................................................................

class Domain {


	private:

	vector<int> value;	// the domain values

	vector<int> state;	// the state of the domain values { -1: ok, 1, 2, ... : for Solver, deleted from var i }

	int size;

	public:

	// constructors ----------------------------------

	Domain();		// default: size of domains : DOM_SIZE, value[i] = i+1 e.g. value[0] = 1
				// default : state = -1

	Domain(vector<int> v);		// default : state[i] = -1

	Domain(vector<int> v, vector<int> s);

	void initialize();

	void initialize(int v); // Initialize Domain of variable v

	void initialize(vector<int> v);

	void initialize(vector<int> v, vector<int> s);

	void clear();

	// set -------------------------------------------

	void set_value(int v, int i);	// v: value, i : indicator

	void set_state(int s, int i);	// s: state, i : indicator

	void set_values(vector<int> v);	// set all values

	void set_states(vector<int> s);	// set all states


	// get -------------------------------------------

	int get_value(int i);	// return domain value i

	int get_state(int i);	// return state of domain value i

	int get_size();		// return the current size of the domain

	vector<int> get_values();	// return all values

	vector<int> get_states();	// return all states

	// others ---------------------------------------

	void print();	//print Domain 

};

class Variable {


	private:


	int value;		// the value of the variable

	Domain domain;		// the domain value of the variable

	vector<int> constraint;	// constraints from cl that have this variable in their scope

	vector<int> bias_constraint;	// constraints from bias that have this variable in their scope

	int constraint_size;	// the number of involved constraints from cl ( the size of vector constraint )
							// deg

	int bias_constraint_size;	// the number of involved constraints from bias ( the size of vector bias_constraint )
								// bdeg

	bool in_ac3_queue;	// if the var is already in ac3 queue

	int wdeg;		// for wdeg heuristic for variable ordering in Solver

	double dom_wdeg;	// for dom/wdeg heuristic for variable ordering in Solver

	int bwdeg;			// bias wdeg heuristic for variable ordering in the generation of a query


	public:


	// constructors ----------------------------------

	Variable();		// default: size of domains : DOM_SIZE, domain[i] = i + 1
				
	Variable(int v);	// v: variable indicator

	Variable(int v, Domain d);

	void initialize();

	void initialize(int v);	// v: variable indicator

	void initialize(int v, Domain d);

	void clear();

	// set - push ------------------------------------

	void set_value(int v);

	void push_con(int c);		// push constraint from cl with the variable in its scope

	void push_bias_con(int c);	// push constraint from bias with the variable in its scope

	void set_domain(Domain d);

	void set_domain_value(int v, int i); 	// v: domain value, i : indicator

	void set_domain_state(int s, int i);	// s: state, i : indicator

	void set_domain_values(vector<int> v); 	// v: all domain values

	void set_domain_states(vector<int> s);		// s: all states

	void set_in_queue(bool q);	

	void set_wdeg(int w);

	void calc_dom_wdeg();		// calculate dom/wdeg

	void set_bwdeg(int bw);

	void increase_bwdeg();

	void decrease_bwdeg();

	// get -------------------------------------------

	int get_value();		// return variable value

	int get_con(int i);		// return constraint[i]				cl

	int get_bias_con(int i);	// return bias_constraint[i]			bias

	int get_con_size();		// return the size of constraint vector		cl

	int get_bias_con_size();	// return the size of bias_constraint vector	bias
	
	Domain get_domain();		// return variable's domain

	int get_domain_value(int i); 	// return domain value i

	int get_domain_state(int i);	// return state of domain value i

	int get_domain_size();		// return the current size of the domain of the variable

	vector<int> get_domain_values(); 	// return all domain values

	vector<int> get_domain_states();	// return all states of domain values

	bool get_in_queue();			// return true if variable is in ac3 queue, false otherwise

	int get_wdeg();				// return wdeg of the variable

	double get_dom_wdeg();			// return dom/wdeg of the variable

	int get_bwdeg();			// return the bias wdeg of the variable

	void print();

};

extern int **Domains;
extern int *DomainsSize;

extern int *DomainOfVar;

extern int DOM_SIZE;
