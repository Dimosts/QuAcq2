#include <vector>
#include <string>

#include "variable.h"
#include "query.h"
 
using namespace std;

// Classes...........................................................................................................

class Solver {

	private:

	vector<Variable> variable;

	Constraint_Net cl;

	Constraint_Net bias;

	vector<int> vars_queue;		// Solver variables ordered queue

	vector<int> used_vars_queue;	// queue for used vars ( to backtrack )

	// for the max-reject problem for bias

	vector<int> bias_rejects;	// for each variable assignation the number of constraints from bias rejecting the query

	vector<int> bias_checked;	// for each variable assignation the number of constraints not possible to be violated

	vector<int> bias_remain;	// for max-rejecet from bias check. 0 if no var of constraint i of the bias is assigned
								// 1 if one var of its scope is assigned ... . 
								// if bias_remain[i] = bias.get_con(i).get_scope.get_size() then con i checked ...

	bool **StatesDomains;		// VAR_NUM x VAR_NUM array, state i exists in domain of variable j ...


	public:

	// constructors ----------------------------------

	Solver();	// default: size of variable array : VAR_NUM 
		// default: size of domains : DOM_SIZE, domain[i] = i + 1

	Solver(vector<Variable> v); 	

	Solver(Constraint_Net c, Constraint_Net b);	// default: size of variable array : VAR_NUM 
							// default: size of domains : DOM_SIZE, domain[i] = i

	Solver(vector<Variable> v, Constraint_Net c, Constraint_Net b);

	void initialize();

	void initialize(vector<Variable> v);

	void initialize(Constraint_Net c, Constraint_Net b);


	// set -------------------------------------------


	void set_variables(vector<Variable> v);	// set all variables

	void set_variable(Variable v, int i);	// set variable i

	void set_variable_value(int v, int i);	// set value v to variable i

	void set_variable_domain(Domain d, int i);	// set domain d to variable i

	void set_variable_domain_state(int state, int var, int dom);	// set variable's var, domain value dom, to state state

	void set_cl(Constraint_Net c);

	void set_bias(Constraint_Net b);

	void set_constraints(Constraint_Net c, Constraint_Net b);

	void increase_con_weight(int c);	// cl[c].weight++


	// get -------------------------------------------


	vector<Variable> get_variables();		// return all variables

	Variable get_variable(int i);		// return variable i

	int get_variable_value(int i);		// return variable i value

	Domain get_variable_domain(int i);	// return value of domain value dom, of variable var

	int get_variable_domain_state(int var, int dom);	// return state of domain value dom, of variable var

	int get_variable_domain_value(int var, int dom);	// return value of domain value dom, of variable var

	Constraint_Net get_cl();			// return cl

	Constraint_Net get_bias();			// return bias

	int get_con_weight(int c);		// return con c weight

	// other methods ---------------------------------

	void print_variables();

	bool gac(vector<short int> scope);		// gac algorithm for arc consistency,
							// scope: which variables to check for arc consistency

	bool gac(int state, vector<short int> scope);		// gac algorithm for arc consistency,
					// state: which var has been assigned a value in mac
					// scope: which variables to check for arc consistency

	bool revise(Constraint con, int var_pos,int state);	// revise function for gac algorithm
						// var_ind: if var to check is 1st or second in Constraint con ( values: 0 or 1 )
					// state: which var has been assigned a value in mac

	Query generate_query();

	Query generate_query(Query query);

	Query generate_query(Query query, double bound);

	Query generate_query(vector<short int> scope);	// generate query with only with variables in scope
	
	Query generate_query(vector<short int> scope, double bound);	// generate query with only with variables in scope in maximum time = bound

	Query generate_findc_query(vector<short int> scope, Constraint_Net Delta_p);	// generate query with only with variables in scope

	Query mac();	// mac algorithm

	Query mac(vector<short int> scope); 	// mac algorithm in vars in vector scope

	int pick_domain_value(int var_index);		// pick domain value for var var_index. 
							// returns position of domain value in variable domain

	void restore_domain(int state);	// restore domain values with state = state

	int next_var();	// returns next variable by wdeg ordering

	int next_var(int h);	// returns next variable by wdeg ordering

	int prev_var();	// returns previous variable

	void calc_wdeg(short int v);		// calculate wdeg of the variable, v: the index of current var

	int count_bias_rejects(int var);		// count rejects after Variable var assignation

	int check_bias_rejects(int var);		// check rejects for a domain value of a Variable

	void bias_remain_restore(int var);	// restore bias_remain array after backtrack 
};