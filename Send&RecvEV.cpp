#include<iostream>
#include "mpi.h"

using namespace std;

int numOfProc, id, array_size, portion;
int *arr = NULL;
MPI_Status status;
const static int tag = 1;


int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	cout << "Hello from Process # " << id << '\n';

	if (id == 0)//master
	{
		cin >> array_size;
		arr = new int[array_size];

		for (int i = 0; i < array_size; i++)
		{
			arr[i] = i + 1;
		}

		 portion = array_size / numOfProc;

		for (int p = 1; p < numOfProc; p++)
		{

			MPI_Send(&portion, 1, MPI_INT, p, tag, MPI_COMM_WORLD);
			MPI_Send(&arr[(p-1)*portion], portion, MPI_INT, p, tag , MPI_COMM_WORLD);
		}
	}
	else // slaves 
	{

		MPI_Recv(&portion, 1, MPI_INT, 0, tag, MPI_COMM_WORLD,&status);
		
		arr = new int[portion]; 

		MPI_Recv(arr, portion, MPI_INT, 0, tag , MPI_COMM_WORLD,&status);

		for (int i = 0; i < portion; i++)
		{
			cout << arr[i] << " "; 
		}

	}

	MPI_Finalize();
}