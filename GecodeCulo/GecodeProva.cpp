// GecodeProva.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h" 
#include "tsp.cpp"


int _tmain(int argc, char* argv[])
{
			SizeOptions opt("TSP");
			opt.solutions(0);
			opt.icl(ICL_VAL);
			opt.parse(argc, argv);

			if (opt.size() >= ps_n) {
				std::cerr << "Error: size must be between 0 and "
					<< ps_n - 1 << std::endl;
				return 1;
			}

			IntMinimizeScript::run<TSP,DFS,SizeOptions>(opt);
			int n = 0;
			scanf_s("%d",n);


}

