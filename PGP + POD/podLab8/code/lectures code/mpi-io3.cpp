#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
	int i, j, rank, nprocs, nx = 4, ny = 5, n_size = 8;
	int *data = (int *)malloc(sizeof(int) * nx * ny);
	char *buff = (char *)malloc(sizeof(char) * nx * ny * n_size);	


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	fprintf(stderr, "%d(%d)\n", rank, nprocs);

	for(j = 0; j < ny; j++)
		for(i = 0; i < nx; i++)
			data[j * nx + i] = j * (nx * nprocs) + rank * nx + i; 

	memset(buff, ' ', nx * ny * n_size * sizeof(char));
	for(j = 0; j < ny; j++) {
		for(i = 0; i < nx; i++)
			sprintf(buff + (j * nx + i) * n_size, "%d(%d) ", data[j * nx + i], rank);
		if (rank + 1 == nprocs)
			buff[(j + 1) * nx * n_size - 1] = '\n';
	}
	for(i = 0; i < nx * ny * n_size; i++)
		if (buff[i] == '\0')
			buff[i] = ' ';


	MPI_File fp;
	MPI_Datatype filetype;

	MPI_Type_vector(ny, nx * n_size, nx * n_size * nprocs, MPI_CHAR, &filetype);
	MPI_Type_commit(&filetype);

	MPI_File_delete("data", MPI_INFO_NULL);
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);


	MPI_File_set_view(fp, nx * n_size * rank * sizeof(char), MPI_CHAR, filetype, "native", MPI_INFO_NULL);

	MPI_File_write_all(fp, buff, nx * ny * n_size, MPI_CHAR, MPI_STATUS_IGNORE);

	
	MPI_File_close(&fp);

	MPI_Finalize();
	free(data);
	free(buff);
	return 0;
}
