#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
	int i, rank, nprocs, n = 16;
	int *data = (int *)malloc(sizeof(int) * n);
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	fprintf(stderr, "%d(%d)\n", rank, nprocs);

/*	for(i = 0; i < n; i++)
		data[i] = rank * nprocs + i; */

	MPI_File fp;
	MPI_Datatype arraytype;

	MPI_Type_contiguous(n, MPI_INT, &arraytype);
	MPI_Type_commit(&arraytype);

//	MPI_File_delete("data", MPI_INFO_NULL);
//	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_RDONLY, MPI_INFO_NULL, &fp);
	MPI_File_set_view(fp, rank * n * sizeof(int), MPI_INT, arraytype, "native", MPI_INFO_NULL);

//	MPI_File_write(fp, data, n, MPI_INT, MPI_STATUS_IGNORE);
	MPI_File_read(fp, data, n, MPI_INT, MPI_STATUS_IGNORE);

	printf("%d: ", rank);
	for(i = 0; i < n; i++)
		printf("%d ", data[i]);
	printf("\n");

	MPI_File_close(&fp);

	MPI_Finalize();
	free(data);

	return 0;
}
