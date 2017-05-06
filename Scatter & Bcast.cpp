#include<iostream>
#include <mpi.h>

using namespace std;
int MAX_SIZE = 5;
int *x = new int[MAX_SIZE];
int *y = new int[MAX_SIZE];

void print(int *arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << "\n";
	}
}



void main(int argc, char *argv[]) {

	int rank , size ;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		//fill x with values
		for (int i = 0; i < MAX_SIZE; i++)
		{
			x[i] = i + 10; 
		}
	}

	int valuesPerProcess = size / MAX_SIZE;
	
	//MPI_Bcast(x, MAX_SIZE, MPI_INT , 0 , MPI_COMM_WORLD);

	MPI_Scatter(x, valuesPerProcess, MPI_INT, y, valuesPerProcess, MPI_INT, 0, MPI_COMM_WORLD);
	
	cout << "Process # " << rank << "\t size is " << size << "\n";
	
	if (rank == 0)
	{
		cout << "X\n";
		print(x, MAX_SIZE);
	}
	
	
	cout << "Y\n";
	print(y, valuesPerProcess);



	MPI_Finalize();

}