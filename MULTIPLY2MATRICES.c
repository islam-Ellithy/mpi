#include<stdlib.h>
#include <mpi.h>

int MAX_SIZE = 10;
int *x ;
int *y ;
int *temp = NULL , *tempMaster = NULL;
int i, j;
int r, c;

void print(int *arr, int size)
{
	for (i = 0; i < size; i++)
	{
		printf("%d\n",arr[i]);
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
            printf("%d ",arr[index2D(i,j)]) ;
		}printf("\n");
	}
    printf("\n");
}

int* newArray(int size)
{
	return (int*)malloc(sizeof(int)*size);
}

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

void main(int argc, char *argv[]) {

	int rank , size ;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("Process # %d\n",rank);

	if (rank == 0)
	{
		//fill x with values

		r = 2;
		c = 2;

		x = newArray(r*c);
		y = newArray(r*c);

		fillMatrix(x, r, c);
		fillMatrix(y, r, c);

		print2D(x, r, c);
		print2D(y, r, c);

		valuesPerProcess = (r*c) / size;
		int p = 1;
		for (p; p < size; p++)
		{
			MPI_Send(&valuesPerProcess,1, MPI_INT, p, 0, MPI_COMM_WORLD);
			MPI_Send(&x[p*valuesPerProcess], valuesPerProcess, MPI_INT, p,0,MPI_COMM_WORLD);
			MPI_Send(&y[p*valuesPerProcess], valuesPerProcess, MPI_INT, p, 0, MPI_COMM_WORLD);
		}

		i = rank*valuesPerProcess;


		temp =  newArray(valuesPerProcess);
		tempMaster = newArray(r*c);

		for (i; i < rank*valuesPerProcess + valuesPerProcess; i++)
		{
			temp[i] = x[i] + y[i];
		}

	}
	else
	{
		valuesPerProcess = 0;
		MPI_Recv(&valuesPerProcess, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		x = newArray(valuesPerProcess);
		y = newArray(valuesPerProcess);
		temp = newArray(valuesPerProcess);

		MPI_Recv(x, valuesPerProcess, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		MPI_Recv(y, valuesPerProcess, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		print(x, valuesPerProcess);
		print(y, valuesPerProcess);

		i = 0;
		for (i; i < valuesPerProcess; i++)
		{
			temp[i] = x[i] + y[i];
		}
		print(temp, valuesPerProcess);
	}



	MPI_Gather(temp, valuesPerProcess, MPI_INT, tempMaster, valuesPerProcess, MPI_INT, 0, MPI_COMM_WORLD);


	if (rank == 0)
	{

		print2D(tempMaster, r, c);

	}


	MPI_Finalize();

}
