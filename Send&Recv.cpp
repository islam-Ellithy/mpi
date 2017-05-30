#include<iostream>
#include <mpi.h>

using namespace std;
int MAX_SIZE = 10;
int *x = NULL;
int *y = NULL ; 
int *temp = NULL , *tempMaster = NULL;
int i, j;
int r, c , r2 , c2;

void print(int *arr, int size)
{
	for (i = 0; i < size; i++)
	{
		cout << arr[i] << "\n";
	}
}



int sum = 0;

int index2D(int r, int c)
{
	return 2 * r + c;
}
void print2D(int *arr, int r, int c)
{
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			cout << arr[index2D(i,j)]<< " " ;
		}cout << endl;
	}
	cout << endl;
}
/*
int* newArray(int size)
{
	return malloc(sizeof(int)*size);
}*/

void fillMatrix(int *x , int r , int c)
{
	int cnt = 1; 
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			x[index2D(i, j)] = cnt++ ;
		}
	}
}

int valuesPerProcess ;
MPI_Status status;
int len;
void main(int argc, char *argv[]) {

	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	cout << "Process # " << rank << "\n";

	if (rank == 0)
	{
		cin >> len;
		x = new int[len];
		for (int i = 0; i < len; i++)
			cin >> x[i];

		for (int i = 0; i < len; i++)
			cout << x[i] << " ";
		cout << '\n';

		for (int i = 1; i < size; i++)
		{
			MPI_Send(&len, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(x, len, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		cout << '\n';
	}
	else
	{
		MPI_Recv(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		x = new int[len];

		MPI_Recv(x, len, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);


		cout << "Length = " << len << "\n";
		print(x, len);
	}
	
	MPI_Finalize();

}