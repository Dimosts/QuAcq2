#include "constraint.h"

#define wrong -999
#define CONJUNCTION -10
#define EXTENSIONAL -100


using namespace std;

// arguments ---------------------------------------------------------------

extern int var_heuristic;	// 0: lexico, 1: dom, 2: dom / wdeg

extern int domain_heuristic;	// 0: lexico, 1: random, 2: max_bias_rej

extern bool user;

extern bool max_b;	// use of heuristic max_B for partial queries!!

extern bool min_opt;	// use of minimization of kappa_B heuristic for generating queries!!

extern bool max_opt;	// use of maximization of kappa_B heuristic for generating queries!!

extern bool sol_opt;    // use of sol heuristic for generating queries!!

extern bool solp_opt;    // use of sol partial heuristic for generating queries!!

extern bool fs2;		// use of findScope-2 or not

extern bool fs3;		// use of findScope-2 or not

extern int fc;		// use of the defined findC function

extern bool ge;          // Define the query generation process followed

extern string benchmark;

extern string algorithm_string;

extern int algorithm; 

extern bool check_collapse; // check collapse or not

extern bool time_limit;

extern bool con_time_cut_off;
extern bool con_num_cut_off;

// ------------------------------------------------------------------------

// Reading from the files ------------------------------------------------- 

extern int **Domains;
extern int *DomainsSize;

extern int *DomainOfVar;

extern int DOM_SIZE;

extern int VAR_NUM;
extern int DOM_NUM;

extern int *condition_values;
extern int VAL_NUM;

extern Constraint_Net Sol;

extern Constraint_Net CL_init;

// -------------------------------------------------------------------------

// other globals -----------------------------------------------------------

extern Constraint_Net *CL;

extern Constraint_Net *B;

extern double cutoff;

extern double time_bound;

extern double background_percentage;       // percentage od c_t in background knowledge

// -------------------------------------------------------------------------

// for the metrics ---------------------------------------------------------

extern int counter_queries;

extern clock_t start_time, end_time;	// to measure time per query

extern double max_query_time;

extern double f_time;

extern double total_time;
extern double total_generating_time;

extern int complete_queries;
extern int queries_size_sum;

extern int counter_cutoff_1;
extern int counter_cutoff_2;

extern bool pos_query;

extern double time_only_pos;

extern clock_t start_time_pos;

extern double counter_queries_conv;

extern Constraint_Net final_B;

extern int remove_counter1;

extern int remove_counter2;

extern int counter_findc;

// -------------------------------------------------------------------------

// for multiacq ---------------

extern clock_t start_cut, end_cut;	// to measure time per query

// ----------------------------
