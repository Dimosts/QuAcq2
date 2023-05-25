#include <vector>

#include "solver.h" 

using namespace std;

Constraint_Net quacq();		// the algorithm QuAcq (Quick Acquisition)

// find the scope of the constraint that needs to be inserted in cl
vector<short int> find_scope(Query query, vector<short int> R, vector<short int> Y, bool flag, Constraint_Net &bias);	

vector<short int> find_scope2(Query query, vector<short int> R, vector<short int> Y, bool flag, Constraint_Net &bias);	

// aij_version
vector<short int> find_scope3(Query query, vector<short int> R, vector<short int> Y, bool flag, Constraint_Net &bias);	

// ------------------------------------------------------------------------------------------------------------------------------------------------------

// findc ijcai version
Constraint find_con(Query query, vector<short int> Y, Constraint_Net &bias, Constraint_Net cl);	

// findC, aij version
bool findC(Query query, vector<short int> Y, Constraint_Net &bias, Constraint_Net &cl);

// Generate query
Query generate_query(Constraint_Net &bias, Constraint_Net &cl);

Query generate_example(Constraint_Net &bias, Constraint_Net &cl);