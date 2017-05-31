#include<iostream>
#include <mpi.h>
#include<time.h>

using namespace std;
int MAX_SIZE = 10;
int *x = NULL;
int *y = NULL;
int *z = NULL;
int *temp = NULL, *tempMaster = NULL;
int i, j;
int r, r1, c, c1;

void print(int *arr, int size)
{
	for (i = 0; i < size; i++)
	{
		cout << arr[i] << "\n";
	}
}



int sum = 0;

int index2D(int r, int c, int col)
{
	return col * r + c;
}

void print2D(int *arr, int r, int c)
{
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			cout << arr[index2D(i, j, c)] << " ";
		}cout << endl;
	}
	cout << endl;
}

/*
int* newArray(int size)
{
	return malloc(sizeof(int)*size);
}*/

int* alloc(int r, int c)
{
	return new int[r*c];
}
void fillMatrix(int *x, int r, int c)
{
	int cnt = 1;
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			x[index2D(i, j, c)] = cnt++;
		}
	}
}



int valuesPerProcess;
MPI_Status status;
int len;
int portion, rem;

int * dot(int *MxA, int r, int c, int *MxB, int r1, int c1, int portion)
{
	cout << "Inside Dot\n";
	//print(z, portion);
	//print(y, r1*c1);
	int *MxC = NULL;
	MxC = alloc(r, c1);
	memset(MxC, 0, r*c1);
	cout << "len = " << r*c1 << '\n';
	cout << r << '\t' << c << '\t' << r1 << '\t' << c1 << '\n';
	int k = 0;
	if (c == r1)
	{
		for (k = 0; k < r; k++)
		{
			for (j = 0; j < c1; j++)
			{

				for (i = 0; i < r; i++)
				{
					MxC[k] += MxA[i] * MxB[i];
					cout << MxC[k] << "\n";
				}
			}
		}
	}
	print2D(MxC, r, c1);

	return MxC;
}


void main(int argc, char *argv[]) {

	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	cout << "Process # " << rank << "\n";

	if (rank == 0)
	{
		cin >> r >> c;
		x = alloc(r, c);
		fillMatrix(x, r, c);
		cin >> r1 >> c1;
		y = alloc(r1, c1);
		fillMatrix(y, r1, c1);
	//	print2D(x, r, c);

//		print2D(y,)
		portion = (r/ size)*c;
		rem = (r)% size;
	}

	MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&c, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&r1, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&c1, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&portion, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank != 0)
		y = new int[r1*c1];

	z = new int[portion];

	MPI_Bcast(y, r1*c1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Scatter(x, portion, MPI_INT, z, portion, MPI_INT, 0, MPI_COMM_WORLD);


	r = portion / c;
	int *Mxc = NULL;// dot(z, portion / c, c, y, r1, c1, portion);
	Mxc = new int[r*c1];
	
	print2D(z,r,c);
	print2D(y, r1,c1);

	int k = 0, m = 0;
	
	if (c == r1)
	{
		for (i = 0; i < r; i++)
		{
			for (j = 0; j < c1; j++)
			{
				Mxc[index2D(i,j,c1)] = 0;
				for (k = 0; k < c; k++)
				{
					Mxc[index2D(i, j, c1)] += z[index2D(i, k, c)] * y[index2D(k, j, c1)];
				}
			}
		}
	}
	//	print(Mxc, r*c1);

	if (rank == 0)
	{
		z = alloc(r, c1);
	}

	MPI_Gather(Mxc, r*c1, MPI_INT, z, r*c1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		cout << "Gather collection\n";
		print2D(z, r*c, c1);
	}

	MPI_Finalize();
}