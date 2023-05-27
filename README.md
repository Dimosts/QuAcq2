# QuAcq2
QuAcq2 implementation from the paper "Learning Constraints through Partial Queries"

QuAcq2 is an active constraint acquisition algorithm. It learns constraint networks by asking the user partial queries. That is, it asks the user to classify assignments to subsets of the variables as positive or negative.

## Compilation

use compile.sh to compile. The executable produced is called "run"

## Usage

./run quacq <arguments>

### Arguments

#### Generic arguments

-nu    			if the oracle is a software system.

-u				If the oracle answering the queries is a human user (default)

-nc 				no collapse test (as in QuAcq.2, in contrast to QuAcq.1)

-i, --initial-cl		the system is expecting to find a "<benchmark>_cl" file in the "<benchmark>" folder, using it as background knowledge

-bp <num>			background knowledge percentage assumption, <num> is a float in ]0,1[


#### Select query generator and variable and value ordering heuristics

-ge				use of GenerateQuery.cutoff from [3] (default is the use of GenerateQuery.basic from [3]) 

-h 	<#>			determines the variable ordering heuristic. Available options:
				- 0     Use of lexicographic variable ordering 
				- 1     Use of dom variable ordering heuristic
				- 2     Use of dom/wdeg variable ordering heuristic
				- 3     Use of bdeg variable ordering heuristic [5]

-domh	<#>			determines the value ordering heuristic. Available options:
				- 0  Use of lexico heuristic, i.e. choose the values in a lexicographic order
				- 1   Use of random heuristic, i.e. choose the values in a random order
				- 2    Use of max_v heuristic, i.e. choose the value violating a maximum number of constraints from the bias [5].



#### Select query generation optimization heuristic -- One of them can be selected in each run -- the ones with objective function also use the cutoff used

-sol, --solution    default=true    use of sol heuristic for generating queries (i.e., no objective, return first complete solution found) [1]

-max, --maximize    default=false    use of maximization of kappa_B heuristic for generating queries, objective: max(kappa_B(e)) [1]

-min, --minimize    default=false    use of minimization of kappa_B heuristic for generating queries, objective: min(kappa_B(e))

-p, --partial    default=false    use of heuristic max_B for partial queries, i.e. genarate a !partial! query maximizing kappa_B, objective: max(kappa_B(e)) [5]

-solp, --solution-partial	generate partial query with maximum assigned variables, objective is: max(|Y|) [3]


#### Select FindScope and FindC versions

-fs	use of findScope [1]

-fs2	use of findScope-2 [4-5]

-fs3	use of findScope-3 [3]

-fc <num>	default=0	use of FindC, ijcai (=0) [1] or aij version (=1) [3]

#### Benchmark selection 

-f <benchmark>		Choice of the benchmark to use.



[1] Christian Bessiere et al. Constraint acquisition via partial queries. In IJCAI, volume 13, pages 475–481, 2013.

[2] Christian Bessiere et al. "Learning Constraints through Partial Queries". Artificial Intelligence 319 (2023): 103896.

[3] Dimosthenis C. Tsouros et al.. Efficient methods for constraint acquisition. In 24th International Conference on Principles and Practice of Constraint Programming, 2018

[4] Dimosthenis C Tsouros and Kostas Stergiou. Efficient multiple constraint acquisition. Constraints, 25(3):180–225, 2020.



##############################################################################################  Benchmark inputs ########################################

There must be a folder "benchmarks/" in the same path with the exe file and a folder with the same name as the benchmark chosen in it. The benchmark folder must contain:
a <benchmark>_var file defining the variables existing in the problem, 
a <benchmark>_dom file defining the domains, 
a <benchmark>_con file if -nu is used, in order to let the system answer the queries without the need of a human user, 
a <benchmark>_cl file defining the initial constraint network we want to give to the algorithm if -i is used.

###### Structure of benchmark files ###########################

Examples of benchmark files are given in the 
<benchmark>_con and <benchmark>_cl files:

<relation_identifier> <variables> <argument>

The numbers corresponding to each relation are the following:
  
0 : x1 != x2, 1 : x1 = x2, 2 : x1 > x2, 3 : x1 < x2, 4 : x1 >= x2, 5 : x1 <= x2, 

6 : x1 - x2 = 1, 7 : x2 - x1 = 1, 8 : |x1 - x2| = 1,
9 : |x1 - x2| != |x3 - x4|,

10 : |x1 - x2| = |x3 - x4|,

11 : |x1 - x2| > arg,

12 : |x1 - x2| = arg,

13 : x1 - x2 = x3,

14 : |x1 - x2| > |x3 - x4|,

15 : |x1 - x2| < |x3 - x4|,

16 : |x1 - x2| < arg,

17 : |floor(x1/3) - floor(x2/3)| > arg

18 - 23: x1 != arg, x1 = arg, x1 > arg, x1 < arg, x1 >= arg, x1 <= arg		<- Unary constraints	

<benchmark>_var:

<number_of_variables> <br>
<var_index> <domain_index> <br>
<var_index> <domain_index> <br>
<var_index> <domain_index> <br>
...


<benchmark>_dom: 

<number_of_domains> <br>
<domain_index> <domain_values><br>
..


