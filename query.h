#include <vector>
#include <math.h>

#include "globals.h" 

using namespace std;

class Query {

	private:

	vector<int> var;	// default var[i] = -1

	short int state;	// 0: initialized 1 : Query ok. 2 : collapse, 3 : converge, 
				// 4: for find_con, if is rejected from some cons and accepted from other
				// 5: Prematurely Converged

	public:

	Query();

	// set --------------------------------

	void set_state(short int s);

	void set_var(int v, int i); // v: variable value, i: variable indicator


	// get -------------------------------

	short int get_state() const;

	int get_var(int i) const;

	int get_size();
	
	vector<int> get_vars() const;


	// other ----------------------------

	void print();

	void write(ofstream *fout);

	void initialize();

	bool ask();	// ask a query to the user

	Query partial(vector<short int> vars);	// return partial query with variable vars

	bool subqueryof(Query query); 	// return true if this query is a sub-query of the query given. Return false otherwise
	
	bool operator !=(Query const &query);

	bool operator ==(Query const &query);
};

