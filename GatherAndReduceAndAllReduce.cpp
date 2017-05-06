#include<iostream>
#include <mpi.h>

using namespace std;
int MAX_SIZE = 10;
int *x = new int[MAX_SIZE];
int *y = new int[MAX_SIZE]; 
int *avg = NULL;

void print(int *arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << "\n";
	}
}

int sum = 0;

int Sum(int *arr, int sz)
{
	int x = 0;
	for (int i = 0; i < sz; i++)
	{
		x += arr[i];
	}
	return x;
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
			x[i] = i + 1; 
		}
		cout << "X\n";
		print(x, MAX_SIZE);
		avg = new int[size];
	}

	int valuesPerProcess = MAX_SIZE/size;
	
	MPI_Scatter(x, valuesPerProcess, MPI_INT, y, valuesPerProcess, MPI_INT, 0, MPI_COMM_WORLD);
	

	cout << "Process # " << rank << "\t size is " << size << "\n";
	
	cout << "Y\n";
	print(y, valuesPerProcess);

	int s = Sum(y, valuesPerProcess);
	
	//MPI_Gather(&s,1,MPI_INT,avg,1,MPI_INT,0,MPI_COMM_WORLD);
	//MPI_Reduce(&s, &sum, 1, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);
	MPI_Allreduce(&s, &sum, 1, MPI_INT, MPI_SUM ,MPI_COMM_WORLD);


	cout << "partial sum = " << s << "\n";
	
		cout << "Final Total sum = " << sum << "\n";

	MPI_Finalize();

}