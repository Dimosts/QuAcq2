#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>

#include "quacq.h" 

//Problem : Sudoku
// 16 Variables
// Domain: 4

using namespace std;

string path = "/benchmarks/";

// functions ------------------------------------------------------------------

bool read_args(int argc, char** argv);

bool read_domains(string filename);

bool read_vars(string filename);

bool read_values(string filename);

// ----------------------------------------------------------------------------

int main(int argc, char** argv){

	int i,j;

	srand(time(0)); // use current time as seed for random generator

	bool ans_flag;	// if user's answer is valid

	cout << endl;

	if ( !read_args(argc, argv) ) {
		exit(-1);
	}

	cout << "Sol size: " << Sol.get_size() << endl;
//	Sol.print();

//	for (int x = 0; x < VAL_NUM; x++)
//		cout << "condition_values[" << x <<"] = " << condition_values[x] << endl;

	if ( algorithm == 2 ) 
		quacq();

	cout << "Queries: " << counter_queries << endl;

	cout << endl;				// final new line 

	cout << endl;				// final new line 

	return 0;

}

bool read_args(int argc, char** argv){

	bool init_flag = false;  // if we want to start with a predefined initial netowrk in C_L

	int heur_sum = 0;	// count for heuristics input

	if ( strcmp(argv[1], "quacq") == 0 ){
		algorithm = 2;
		algorithm_string = "quacq";
	} else {
		cout << "Wrong algorithm choice, cannot recognize '" << argv[1] << "'" << endl;
		return false;
	}

	for (int i = 2; i < argc; i++){

		if ( strcmp(argv[i], "-nu") == 0 )		// no user, system answers
			user = false;
		else if ( strcmp(argv[i], "-nc") == 0 )	// no checking collapse
			check_collapse = false;			
		else if ( strcmp(argv[i], "-u") == 0 )	// user answers
			user = true;
		 else if ( strcmp(argv[i], "-h") == 0 ){	// -h which variable ordering var_heuristic to choose

			i++;

			if ( strcmp(argv[i], "0") == 0 )
				var_heuristic = 0;
			else if ( strcmp(argv[i], "1") == 0 )
				var_heuristic = 1;
			else if ( strcmp(argv[i], "2") == 0 )
				var_heuristic = 2;
			else if ( strcmp(argv[i], "3") == 0 )
				var_heuristic = 3;
			else if ( strcmp(argv[i], "4") == 0 )
				var_heuristic = 4;				
			else {
				cout << "Wrong variable heuristic choice, cannot recognize '" << argv[i] << "'" << endl;
				return false;
			}

		} else if ( strcmp(argv[i], "-domh") == 0 ){	// which value ordering heuristic

			i++;

			if ( strcmp(argv[i], "0") == 0 )
				domain_heuristic = 0;
			else if ( strcmp(argv[i], "1") == 0 )
				domain_heuristic = 1;
			else if ( strcmp(argv[i], "2") == 0 )
				domain_heuristic = 2;
			else {
				cout << "Wrong heuristic choice, cannot recognize '" << argv[i] << "'" << endl;
				return false;
			}

		} else if ( strcmp(argv[i], "-ge") == 0 || strcmp(argv[i], "--generate-example") == 0 )	// which value ordering heuristic
			ge = true;
		else if ( strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--initial-cl") == 0 ){

			init_flag = true;

		} else if ( strcmp(argv[i], "-bp") == 0 ){       // -bp <percentage: backgroung knowledge percentage

			i++;

			stringstream temp(argv[i]);

			temp >> background_percentage;
			cout << background_percentage;

//			do {
//
//				cout << "background percentage should be inside the range (0,1)\n Please give the value again: ";
//				cin >> background_percentage;

//			} while ( background_percentage < 0 || background_percentage > 1 );
		


		} else if ( strcmp(argv[i], "-f") == 0 ){	// -f <benchmark> : which benchmark to run

			i++;

			benchmark = argv[i];		// get filename from command line

			string var_file = path + benchmark + "/" + benchmark + "_var";

			string dom_file = path + benchmark + "/" + benchmark + "_dom";

			string con_file = path + benchmark + "/" + benchmark + "_con";

			string val_file = path + benchmark + "/" + benchmark + "_val";

//			string bias_file = benchmark + "_bias";

			read_domains(dom_file);

			read_vars(var_file);

			if ( benchmark.compare("rlfap") == 0 || benchmark.compare("zebra") == 0 || benchmark.compare(0,11,"timetabling") == 0)
				read_values(val_file);

			Sol.read(con_file);

		} else if ( strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--partial") == 0 || strcmp(argv[i], "-maxb") == 0 ){	// -p use of max_b heuristic for the generation of the queries
			max_b = true;
			heur_sum++;
		} else if ( strcmp(argv[i], "-max") == 0 || strcmp(argv[i], "--maximize") == 0 ){	// use of max heuristic for the generation of the queries
			max_opt = true;
			heur_sum++;
		} else if ( strcmp(argv[i], "-min") == 0 || strcmp(argv[i], "--minimize") == 0 ){	// use of min heuristic for the generation of the queries
			min_opt = true;
			heur_sum++;
		} else if ( strcmp(argv[i], "-sol") == 0 || strcmp(argv[i], "--solution") == 0 ){	// use of sol heuristic for the generation of the queries
			sol_opt = true;
			heur_sum++;
		} else if ( strcmp(argv[i], "-solp") == 0 || strcmp(argv[i], "--solution-partial") == 0 ){	// use of partial sol (solp) heuristic for the generation of the queries
			solp_opt = true;
			heur_sum++;
		} else if ( strcmp(argv[i], "-fs2") == 0 || strcmp(argv[i], "--findscope2") == 0 ){	// use of function findscope-2 instead of function findscope
			fs2 = true;
			fs3 = false;
		} else if ( strcmp(argv[i], "-fs3") == 0 || strcmp(argv[i], "--findscope3") == 0 ){	// use of function findscope (aij version) instead of function findscope
			fs3 = true;			
			fs2 = false;
		} else if ( strcmp(argv[i], "-fs") == 0 || strcmp(argv[i], "--findscope") == 0 ){ // use of function findscope
			fs2 = false;
			fs3 = false;
		} else if ( strcmp(argv[i], "-fc") == 0 || strcmp(argv[i], "--findc") == 0 ){

			i++;

			if ( strcmp(argv[i], "0") == 0 )
				fc = 0;	// use of simple findc (ijcai version)
			else if ( strcmp(argv[i], "1") == 0 )
				fc = 1;	// use of findc (aij version)
			else {
				cout << "Wrong findc choice, cannot recognize '" << argv[i] << "'" << endl;
				return false;
			}

		} else {
			cout << "Wrong parameters, cannot recognize '" << argv[i] << "'" << endl;
			return false;
		}

	}

	if ( Sol.get_size() == 0 ){
		
		cout << "Benchmark not found or not defined. Please use argument -f <benchmark> to define the benchmark" << endl;
		return false;		

	}

	if ( init_flag ) {

		string cl_file = path + benchmark + "/" + benchmark + "_cl";
		string ans;

		CL_init.read(cl_file);

		if ( CL_init.get_size() < 1 ) {
			
			CL_init.print();

			do {

				cout << "There is no initial network file (_cl) for this benchmark. Continue without it?? (y, n)" << endl;
				cin >> ans;

				if ( ans.compare("n") == 0 )
					return false;
				else if ( ans.compare("y") != 0 )
					cout << "Please type y or n: ";

			} while ( ans.compare("y") == 0 );

		}
	
	}	


	if ( heur_sum == 0 )
		max_opt = true;
	else if ( heur_sum > 1 ){
		cout << "Error: More than one heuristic is chosen "<< endl;
		return false;
	}

	cout << "Vars number = " << VAR_NUM << endl;

	cout << "Variable Heuristic = " << var_heuristic << endl;
	cout << "Domain Heuristic = " << domain_heuristic << endl;
	
	cout << "Generator heuristic = ";

	if ( max_b )
		cout << "max_B" << endl;
	else if ( max_opt )
		cout << "max" << endl;
	else if ( min_opt )
		cout << "min" << endl;
	else if ( sol_opt )
		cout << "sol" << endl;
	else if ( solp_opt )
		cout << "solp" << endl;
		
	cout << "User answer : " ;

	if ( user )
		cout << "Yes" << endl;
	else
		cout << "No" << endl;

	cout << endl;

	return true;

}

bool read_domains(string filename){

	bool falg = true; 

	ifstream fin(filename.c_str());
	string line;

	int pos, size, val, i;

	// read first line ( number of domains ) ---
	getline(fin,line);
	DOM_NUM = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int

	Domains = new int*[DOM_NUM];
	DomainsSize = new int[DOM_NUM];

	// read file line by line ---------
	while ( getline(fin,line) ) {

		if ( line.find(" ") != string::npos ){

			// take dom pos ---------------------------------------------------

			pos = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int
			
			// remove from the line anything until ' '
			line = line.substr(line.find(" ")+1);


			// take dom size ---------------------------------------------------

			size = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int
			
			// remove from the line anything until ' '
			line = line.substr(line.find(" ")+1);

			Domains[pos] = new int[size];
			DomainsSize[pos] = size;

			// take dom values ---------------------------------------------------

			for (i = 0; i < size; i++){

				val = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int
				
				Domains[pos][i] = val;

				// remove from the line anything until ' '
				line = line.substr(line.find(" ")+1);

			}

		}

	}

	cout << "Domains read completed" << endl;

	return true;

}

bool read_vars(string filename){

	bool falg = true; 

	ifstream fin(filename.c_str());
	string line;

	int var, dom;

	// read first line ( number of variables ) ---
	getline(fin,line);
	VAR_NUM = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int

	DomainOfVar = new int[VAR_NUM];

	// read file line by line ---------
	while ( getline(fin,line) ) {

		if ( line.find(" ") != string::npos ){

			// take var indicator ---------------------------------------------------
			var = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int
			
			// remove from the line anything until ' '
			line = line.substr(line.find(" ")+1);


			// take var's dom ---------------------------------------------------
			dom = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int
			

			DomainOfVar[var] = dom;

		}

	}

	cout << "Vars read completed" << endl;

	return true;

}

bool read_values(string filename){

	bool falg = true; 

	ifstream fin(filename.c_str());
	string line;

	int value, counter = 0;

	// read first line ( number of condition values ) ---
	getline(fin,line);
	VAL_NUM = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int

	condition_values = new int[VAL_NUM];

	// read file line by line ---------
	while ( getline(fin,line) ) {

		// take value in line of number -> counter ---------------------------------------------------
		value = atoi(line.substr(0, line.find(" ")).c_str() ); // convert to int

		condition_values[counter] = value;

		counter++;

	}

	cout << "Condition values read completed" << endl;

	return true;

}



