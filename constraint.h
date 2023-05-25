#include <vector>
#include <string>

#define SAT 1
#define VIOL 0
#define UNDEF -1

using namespace std;

// Classes...........................................................................................................
class Variable;

class Query;

class Scope {

	private:

	vector<short int> variables;

	vector<short int> distinct_variables;

	public:

	// Constructor --------------------------

	Scope();
	Scope(vector<short int> var);


	// set - push ---------------------------

	void set_scope(vector<short int>  var);

	void push_var(short int v);

	void set_distinct_variables();


	// get ----------------------------------

	vector<short int> get_scope();

	vector<short int> get_variables();	// same as get_scope

	vector<short int> get_distinct_variables();	// get individual variables

	short int get_var(short int i);

	int get_size();

	int get_real_size();


	// other methods ------------------------

	bool compare(Scope scope);	// compare with Scope scope

	bool compare(vector<short int> var);	// compare with variables var

	bool subset_of(Scope scope);	//check if this Scope is a subset of scope

	bool subset_of(vector<short int> var);	//  check if this Scope is a subset of variables var

	void print();

	void println();

	void clear();	// clear 

};

class Constraint {

	private:

	short int type;			// 0 : != , 1 : = , 2 : > , 3 : < , 4 : >= , 5 : <= , 		<- generic binary constraints
							
							// 6 : x1 - x2 = 1, 7 : x2 - x1 = 1, 8 : |x1 - x2| = 1		<- for zebra problem
							
							// 9 : |x1 - x2| != |x3 - x4|								<- golomb rulers problem
							// 10 : |x1 - x2| = |x3 - x4|								<- golomb rulers problem
							
							// 11 : |x1 - x2| > arg3									<- rlfap problem
							// 12 : |x1 - x2| = arg3									<- rlfap problem
							
							// 13 : x1 - x2 = x3										<- CostasArray problem

							// 14 : |x1 - x2| > |x3 - x4|								<- golomb rulers problem
							// 15 : |x1 - x2| < |x3 - x4|								<- golomb rulers problem

							// 16 : |x1 - x2| < arg3									<- time-tabling problem
							// 17 : |floor(x1/3) - floor(x2/3)| > arg3					<- time-tabling problem

							// 18 - 23: !=, =, >, <, >=, <=								<- unary constraints

							// reading of all cons : type, var1, var2, ...
							// types 1 - 8: binary constraints
							// types 9 > non-binary constraints (in this case quaternary and maybe ternary)

							// Type CONJUNCTION is for conjunction of constraints

							// Type EXTENSIONAL is for extensional constraints (Not implemented yet)


	Scope scope;	// scope of the constraint. 
	string con;			

	vector<Constraint> conjunction;	// if it is a conjunction constraint ...

//	vector<int> values;	// for extentional constraints

	int value;	// for constraints of type 11 and 12 and 16 - 23

	int weight;			// the weight for the wdeg heuristic of the solver

	int inst_variables;	// the number of instantiated variables (the variables that already have been assigned a value)

// NOT USED ---------------------------------------------------------------------------

	bool reified;		// if constraint is reified. 0: no (hard constraint), 1: yes

	int satisfied;		// if constraintt is satisfied in current model solving
				// SAT (1) : satisfied,	VIOL (0) : not satisfied, UNDEF (-1) : undefined

	int state;		// the state (variable assign) that lead to satisfied value
				// -1: satisfied = UNDEF

// ------------------------------------------------------------------------------------

	public:

	// Constructor --------------------------

	Constraint();	
	Constraint(short int t);										// only the type
	Constraint(short int t, int v);									// type and value
	Constraint(short int t, bool r);								// type and if reified (NOT USED)
	Constraint(short int type, vector<short int>  v);				// type and variables (in vector)
	Constraint(short int type, vector<short int>  v, bool r);		// type, variables and if reified (NOT USED)
	Constraint(short int type, Scope s);							// type and Scope
	Constraint(short int type, Scope s, bool r);					// type, Scope and if reified (NOT USED)


	// set - push ---------------------------

	void set_type(short int t);

	void set_scope(vector<short int>  v);

	void set_scope(Scope s);

	void push_var(short int v);

	void set_weight(int w);

	void set_value(int v);

	void set_reified(bool r);

	void set_satisfied(int s);	// satisfied = s;

	void set_state(int s);	// state = s;

	void reify();			// reified = true

	void unreify();			// reified = false

	void increase_weight();		// weight = weight + 1

	void increase_inst_vars();	// increase instantiated variables by 1

	void decrease_inst_vars();	// decrease instantiated variables by 1

	void set_inst_vars(int n);	// set instantiated variables to n

	void push_conjunction_constraint(Constraint con);

	// get ----------------------------------

	short int get_type();

	vector<short int> get_scope_vars();

	Scope get_scope();

	int get_scope_size();

	short int get_var(short int i);

	string get_con();

	int get_weight();

	int get_value();

	bool get_reified();

	int get_satisfied();

	int get_state();

	int get_inst_vars();	// get the number of instantiated variables

	Constraint get_conjunction_constraint(int i);
	
	vector<Constraint> get_conjunction_constraints();

	int get_conjunction_size();

	// other methods ------------------------

	bool satisfy(vector<int> e);	// if an example e satisfies the constraint
	
	bool satisfy(Query query);	// if the Query query satisfies the constraint

	bool satisfy(int var1, int var2);	// for binary constraints !! if the two vars satisfy the constraint

	bool satisfy(int var1);	// for unary constraints !!

	bool satisfy(int var1, int var2, int var3, int var4);	// for constraints of type 9, 10 !! (ternary and quaternary)
															// if the four vars assignation satisfy the constraint

	bool satisfy(int var1, int var2, int var3);			// For Constraints of type 13 (ternary) !

	bool satisfy(int v[]);	// if these values (v[]) for the variables of its scope, satisfies the constraint

	bool propagate(vector<Variable> variable, int st);	// if the two vars of constraints scope can satisfy the constraint

	void restore(int state);	// restore reified constraint from state ...

	bool compare(Constraint con);	// compare with constraint con

	bool compare_scope(Constraint con);	// compare scope with constraint con

	bool compare_scope(Scope sc);	// compare scope with constraint con

	Constraint complement();	// return the complementary of this constraint ( only for = and != until now)

	void to_conjunction();	// make the constraint a conjunction constraint

	void print();	// print the constraint to user friendly

	void println();	// print the constraint to user friendly

	void write(ofstream *fout);

	void clear();	// clear 

};

class Constraint_Net {

	private:

	vector<Constraint> constraints;
	int size;

	int nb_sat;
	int nb_undef;
	int nb_viol;

	public:


	// Constructor --------------------------

	Constraint_Net();
	Constraint_Net(int var);


	// set - push ---------------------------

	void set_network(vector<Constraint> c);		// set the network

	void push_con(Constraint c);		// push constraint c to the network

	void push_con(Constraint_Net Cn);		// push constraint network Cn to the network

	void increase_con_weight(int c);	// constraints[c].weight++

	void increase_inst_vars(int c);	// increase instantiated variables by 1, to con c

	void decrease_inst_vars(int c);	// decrease instantiated variables by 1, to con c

	void check_cons();		// update nb_sat, nb_undef, nb_viol

	// get ----------------------------------

	vector<Constraint> get_network();	// return all the constraints

	vector<Constraint> get_constraints(); // return all the constraints
	
	vector<short int> get_variables(); // return the variables that included in the scope of at least one constraint
	
	Constraint get_con(int i);	// return the constraint i

	Constraint_Net get_scope(Scope scope);	// return all the constraints with Scope scope

	Constraint_Net get_scope(vector<short int> scope);	// return all the constraints with Scope scope

	Constraint_Net get_subnetwork(Scope scope);	// return all the constraints with a scope var(c) \in scope

	Constraint_Net get_subnetwork(vector<short int> scope); // return all the constraints with a scope var(c) \in scope
	
	int get_size();			// return the size of the constraint network

	int get_max_conjunction_size();	// return the size of the largest conjunction constraint

	int get_min_conjunction_size();	// return the size of the smallest conjunction constraint

	int get_con_weight(int c);		// return con c weight

	int get_nb_sat();	// get number of constraints being satisfied

	int get_nb_undef();	// get number of constraints undefined if satisfied

	int get_nb_viol();	// get number of constraints violated

	Constraint_Net get_rejects(Query query);	// return k_B(query)

	Constraint_Net get_accepts(Query query);	

	// other methods ------------------------

	bool contains(Constraint con);	// if thiw constraint network contains the constraint ``con''

	void joint(Constraint_Net net);	// joint with constraint network net

	void propagate(int i, vector<Variable> variable, int st);	// propagate constraint i. State: st

	void restore(int state);	// restore reified constraints from state ...

	void clear();

	void remove_con(int i);	// remove the constraint i from network

	bool remove_con(Constraint c);	// remove the constraint c from network

	void remove_scope(Scope scope);	// remove all the constraints with Scope scope

	void remove_scope(vector<short int> scope);	// remove all the constraints with Scope scope

	void initialize_bias(int var);

	void print();

	int count_rejects(Query query);	// count how many constraints reject a query

	int count_rejects(vector<Variable> variable);	// count how many constraints reject a query

	bool rejects(Query query);	// true if at least one constraint reject query

	void remove_rejects(Query query); // remove the constraints that reject a query

	void remove_accepts(Query query); // remove the constraints that accept a query

	void remove_existing(Constraint_Net Cn);

	// remove the constraints with scope 'scope' that already exists to cl
	void remove_existing(Constraint_Net cl, vector<short int> scope);	

	// read - write ---------------------------------------------------------------

	bool read();		// read from a file given by user

	bool read(string filename);	// read from file 'filename'

	void write();	// write to file given by user

	void write(string filename);	// write to file 'filename'


};

extern int VAL_NUM;

extern int *condition_values;
