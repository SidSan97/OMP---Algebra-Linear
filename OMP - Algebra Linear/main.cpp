#include <iostream>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <omp.h>
#include <stdlib.h>
#pragma warning(disable:4996)

using namespace std;

static void SERIAL()
{
	setlocale(LC_ALL, "Portuguese");
	clock_t c1, c2, c3, c4;
	float tempo, tempo1, tempo2;
	int matrizA[10][10], matriz_B[10][10], matriz_C[10][10], nLido, auxi = 0;

	FILE* matriz_A;  //DANDO NOME AO PONTEIRO QUE IRÁ LER O ARQUIVO COM A MATRIZ

	cout << "Abaixo está a matriz A lida de um arquivo de texto: " << endl<<endl;

	c1 = clock();
	matriz_A = fopen("matriz_A.txt", "r"); //LENDO O ARQUIVO .TXT QUE CONTEM A MATRIZ
	if (matriz_A == NULL)
	{
		cout << "Não foi possível abrir arquivo matriz_A.txt" << endl;
		exit(1);
	}

	for (int i = 0; i < 10; i++) 
	{
		for (int j = 0; j < 10; j++) 
		{
			nLido = fscanf(matriz_A, "%d", &matrizA[i][j]);
			cout << matrizA[i][j] << " ";
		}
		cout << endl;
	}
	fclose(matriz_A);

	cout << endl;

	cout << "Abaixo temos a matriz B gerada de forma randômica: \n" << endl;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			matriz_B[i][j] = rand() % 10;
			cout << matriz_B[i][j] << " ";
		}
		cout << endl;
	}

	//PARTE DO CÓDIGO QUE ESCREVE A MATRIZ B EM UM ARQUIVO DE TEXTO 'matriz_B.txt'

	FILE* MatrizB;
	MatrizB = fopen("Matriz_B.txt", "w");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			fprintf(MatrizB, "%d ", matriz_B[i][j]);
		}
		fprintf(MatrizB, "\n");
	}

	//PARTE QUE FAZ A MULTIPLICAÇÃO ENTRE AS MATRIZES A e B
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			matriz_C[i][j] = 0;
			for (int k = 0; k < 10; k++)
			{
				auxi += matrizA[i][k] * matriz_B[k][j];
			}
			matriz_C[i][j] = auxi;
			auxi = 0;
		}
	}

	cout << endl;

	//IMPRIME O PRODUTO DA MATRIZ NA TELA
	cout << "Produto das matrizes é: " << endl;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cout << matriz_C[i][j] << " ";
		}
		cout << endl;
	}

	//ESCREVE A MATRIZ C EM UM ARQUIVO DE TEXTO
	FILE* MatrizC;
	MatrizC = fopen("Matriz_C.txt", "w");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			fprintf(MatrizC, "%d ", matriz_C[i][j]);
		}
		fprintf(MatrizC, "\n");
	}
	c2 = clock();

	tempo = c2 - c1;

	cout << endl;
	cout << "Tempo de execução foi de: " << 1000 * (double)tempo / (double)(CLOCKS_PER_SEC) <<" milisegundos."<< endl;
	cout << endl;

	system("pause");

	MENU();
}

static void OPENMP(void)
{
	setlocale(LC_ALL, "Portuguese");
	clock_t c1, c2, c3, c4;
	float tempo, tempo1, tempo2;
	int tid, nthreads, chunk;
	int matrizA[10][10], matriz_B[10][10], matriz_C[10][10], nLido, auxi = 0;

	FILE* matriz_A;  //DANDO NOME AO PONTEIRO QUE IRÁ LER O ARQUIVO COM A MATRIZ

	cout << "Abaixo está a matriz A lida de um arquivo de texto: " << endl << endl;

	c1 = clock();

	#pragma omp parallel
	{
		tid = omp_get_thread_num();
		if (tid == 0) {
			nthreads = omp_get_num_threads();  //INICIALIZA AS THREADS
			printf("Iniciando multiplicacao de matrizes com %d threads\n", nthreads);
		}

		#pragma omp for schedule(static, chunk)
		matriz_A = fopen("matriz_A.txt", "r"); //LENDO O ARQUIVO .TXT QUE CONTEM A MATRIZ
		if (matriz_A == NULL)
		{
			cout << "Não foi possível abrir arquivo matriz_A.txt" << endl;
			exit(1);
		}

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				nLido = fscanf(matriz_A, "%d", &matrizA[i][j]);
				cout << matrizA[i][j] << " ";
			}
			cout << endl;
		}
		fclose(matriz_A);

		cout << endl;

		cout << "Abaixo temos a matriz B gerada de forma randômica: \n" << endl;

		#pragma omp num_threads(4)
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					matriz_B[i][j] = rand() % 10;
					cout << matriz_B[i][j] << " ";
				}
				cout << endl;
			}

			//PARTE DO CÓDIGO QUE ESCREVE A MATRIZ B EM UM ARQUIVO DE TEXTO 'Matriz_B PARALELO.txt'

			FILE* MatrizB;
			MatrizB = fopen("Matriz_B PARALELO.txt", "w");

			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					fprintf(MatrizB, "%d ", matriz_B[i][j]);
				}
				fprintf(MatrizB, "\n");
			}
		}

		//PARTE QUE FAZ A MULTIPLICAÇÃO ENTRE AS MATRIZES A e B

		#pragma omp num_threads(4)
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					matriz_C[i][j] = 0;
					for (int k = 0; k < 10; k++)
					{
						auxi += matrizA[i][k] * matriz_B[k][j];
					}
					matriz_C[i][j] = auxi;
					auxi = 0;
				}
			}
		}

		cout << endl;

		//IMPRIME O PRODUTO DA MATRIZ NA TELA
		cout << "Produto das matrizes é: " << endl;

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				cout << matriz_C[i][j] << " ";
			}
			cout << endl;
		}

		//IMPRIME A MATRIZ C EM UM ARQUIVO DE TEXTO
		FILE* MatrizC;
		MatrizC = fopen("Matriz_C PARALELO.txt", "w");

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				fprintf(MatrizC, "%d ", matriz_C[i][j]);
			}
			fprintf(MatrizC, "\n");
		}
	}
	c2 = clock();

	tempo = c2 - c1;

	cout << endl;
	cout << "Tempo de execução foi de: " << 1000 * (double)tempo / (double)(CLOCKS_PER_SEC) << " milisegundos." << endl;
	cout << endl;

	system("pause");

	MENU();
}

void MENU(void)
{
	setlocale(LC_ALL, "Portuguese");

	int op;

	cout << "Qual opção você deseja executar? " << endl;
	cout << "(1) SERIAL" << endl << "(2) PARALELO" << endl;

	cin >> op;

	if (op == 1)
		SERIAL();
	else if (op == 2)
		OPENMP();

	return;
}

int main()
{
	setlocale(LC_ALL, "Portuguese");

	int op;

	cout << "Qual opção você deseja executar? " << endl;
	cout << "(1) SERIAL" << endl << "(2) PARALELO" << endl;

	cin >> op;

	if (op == 1)
		SERIAL();
	else if (op == 2)
		OPENMP();
}