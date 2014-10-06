// GecodeProva.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h" 
#include "tsp.cpp"


int _tmain(int argc, char* argv[])
{
	int maxNumber;
	int firstFile;
	int lastFile;
	int firstMatrix;
	int lastMatrix;

	std::cout << "max distance:";
	std::cin >>maxNumber;
	std::cout << "first output file:";
	std::cin >> firstFile;
	std::cout << "last output file:";
	std::cin >> lastFile;
	std::cout << "first matrix:";
	std::cin >> firstMatrix;
	std::cout << "last matrix:";
	std::cin >> lastMatrix;

	std::cout << "Wait..." << std::endl;
	SizeOptions opt("TSP");
	opt.solutions(0);
	opt.icl(ICL_DOM);
	opt.parse(argc, argv);
	//creo problema
	
	//ciclo creazione file
	for (int n = firstFile; n < lastFile+1; n++){
		std::string nome_file = "out";
		nome_file.append(std::to_string(n));
		nome_file.append(".txt");
		std::ofstream out(nome_file);
		std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
		std::cout.rdbuf(out.rdbuf());
		//ciclo matrici
		for (int k = firstMatrix; k < lastMatrix+1; k++) {
			PA_n = k;
			PA_d = new int[PA_n * PA_n];
			for (int i = 0; i < PA_n; i++) {
				for (int j = 0; j < PA_n; j++) {

					if (i == j)
						PA_d[PA_n*i + j] = 0;
					else if (i>j) {
						PA_d[PA_n*i + j] = (rand() % maxNumber) + 1;
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
				if (PA_d[i] >= 10 & PA_d[i]<100)
					std::cout << " ";
				else if (PA_d[i] < 10)
					std::cout << "  ";
				std::cout << PA_d[i];
				if ((i + 1) % PA_n == 0 & i != 0)
					std::cout << std::endl << "         ";
				else
					std::cout << ",";
			}
			std::cout << std::endl;
			IntMinimizeScript::run<TSP, BAB, SizeOptions>(opt);
		}

		std::cout << "Complete!" << std::endl;
	}
}

