//#include "constraint.h"
#include "query.h"


using namespace std;

// arguments ---------------------------------------------------------------

int var_heuristic = 1;	//	0: none, 
					//	1: dom,
					//	2: dom / wdeg
					//	3: bdeg
					//	4: bwdeg

int domain_heuristic = 1;	// 0: lexico, 1: random, 2: max_bias_rej

bool user = true;		// user or machine answering

bool max_b = false;	// use of heuristic max_B for partial queries!!

bool min_opt = false;	// use of minimization of kappa_B heuristic for generating queries!!

bool max_opt = false;	// use of maximization of kappa_B heuristic for generating queries!!

bool sol_opt = false;    // use of sol heuristic for generating queries!!

bool solp_opt = false;	 // use of sol partial heuristic for generating queries!!

bool fs2 = true;		// use of findScope-2 or not

bool fs3 = true;		// use of findScope-3 or not

int fc = 0;				// use of FindC, find_subscope (findc from icon) or FindC2-3-4-5 (0,1,2,3,4,5)
						// default: use FindC2

bool ge = false;          // Define the query generation process followed

string benchmark;

string algorithm_string;

int algorithm = 1; // which algorithm to use : 1: mquacq, 2: quacq, 3: multiacq, 4: MQuAcq-2
					// default mquacq

bool check_collapse = true; // check collapse or not

bool time_limit = true;

	// 1 of the 2 heuristics for con cutoff can be used (for MQuAcq)
bool con_time_cut_off = false;
bool con_num_cut_off = false;

// ------------------------------------------------------------------------

// Reading from the files ------------------------------------------------- 

int **Domains;
int *DomainsSize;

int *DomainOfVar;

int DOM_SIZE = 9;

int VAR_NUM = 81;
int DOM_NUM = 1;

int *condition_values;
int VAL_NUM = 0;

Constraint_Net Sol;

Constraint_Net CL_init;

// -------------------------------------------------------------------------

// other globals -----------------------------------------------------------

Constraint_Net *CL;

Constraint_Net *B;

double cutoff = 1;			// 1 sec

double time_bound = 900;	// 900 secs = 15 min

double background_percentage = 0;       // percentage of c_t in background knowledge

// -------------------------------------------------------------------------

// for the metrics ---------------------------------------------------------

int counter_queries = 0;

clock_t start_time, end_time;	// to measure time per query

double max_query_time = 0;

double f_time;

double total_time;
double total_generating_time = 0;

int complete_queries = 0;
int queries_size_sum = 0;

int counter_cutoff_1 = 0;
int counter_cutoff_2 = 0;

bool pos_query = false;

double time_only_pos = 0;

clock_t start_time_pos;

double counter_queries_conv = 0;

Constraint_Net final_B;

int remove_counter1 = 0;

int remove_counter2 = 0;

int counter_findc = 0;

// -------------------------------------------------------------------------

// for multiacq ---------------

clock_t start_cut, end_cut;	// to measure time per query

// ----------------------------
