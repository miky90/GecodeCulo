// GecodeProva.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h" 
#include "tsp.cpp"


int _tmain(int argc, char* argv[])
{
	std::cout << "Wait..." << std::endl;
	SizeOptions opt("TSP");
	opt.solutions(0);
	opt.icl(ICL_VAL);
	opt.parse(argc, argv);
	//creo problema
	int maxNumber = 100;
	std::ofstream out("out.txt");
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf());
	for (int k = 4; k < 100; k++) {
		PA_n = k;
		PA_d = new int[PA_n * PA_n];
		for (int i = 0; i < PA_n; i++) {
			for (int j = 0; j < PA_n; j++) {
				if (i == j)
					PA_d[PA_n*i + j] = 0;
				else
					PA_d[PA_n*i + j] = rand() % maxNumber;
			}
		}
		if (PA != 0)
			delete PA;
		PA = new Problem(PA_n, PA_d);
		std::cout << "n_citta: " << k <<std::endl;
		IntMinimizeScript::run<TSP, BAB, SizeOptions>(opt);
	}
	
	std::cout << "Complete!" << std::endl;
	int n = 0;
	scanf_s("%d",n);
}

