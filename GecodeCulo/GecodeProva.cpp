// GecodeProva.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h" 
#include "tsp.cpp"


int _tmain(int argc, char* argv[])
{
	std::cout << "Wait..." << std::endl;
	SizeOptions opt("TSP");
	opt.solutions(0);
	opt.icl(ICL_DOM);
	opt.parse(argc, argv);
	//creo problema
	int maxNumber = 100;
	std::ofstream out("out.txt");
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf());
	for (int k = 4; k < 100; ) {
		PA_n = k;
		PA_d = new int[PA_n * PA_n];
		for (int i = 0; i < PA_n; i++) {
			for (int j = 0; j < PA_n; j++) {

				if (i == j)
					PA_d[PA_n*i + j] = 0;
				else if (i>j) {
					PA_d[PA_n*i + j] = rand() % maxNumber;
					PA_d[PA_n*j + i] = PA_d[PA_n*i + j];
				}
			}
		}
		if (PA != 0)
			delete PA;
		PA = new Problem(PA_n, PA_d);
		std::cout << "n_citta: " << k << std::endl;
		std::cout << "Matrice: ";
		for (int i = 0; i < PA_n*PA_n; i++){
			if (PA_d[i] < 10)
				std::cout << " ";
			std::cout << PA_d[i];
			if ((i+1)%PA_n==0 & i!=0)
				std::cout << std::endl << "         ";
			else
				std::cout << ",";
		}
		std::cout << std::endl;
		IntMinimizeScript::run<TSP, BAB, SizeOptions>(opt);

		if (k < 25)
			k++;
		else if (k < 50)
			k += 5;
		else
			k += 10;
	}
	
	std::cout << "Complete!" << std::endl;
	int n = 0;
	scanf_s("%d",n);
}

