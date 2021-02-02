#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

// Индексация внутри блока
#define _i(i, j) (((j) + 1) * (n + 2) + (i) + 1)
#define _ix(id) (((id) % (n + 2)) - 1)
#define _iy(id) (((id) / (n + 2)) - 1)

// Индексация по блокам (процессам)
#define _ib(i, j) ((j) * nb + (i))
#define _ibx(id) ((id) % nb)
#define _iby(id) ((id) / nb)

int main(int argc, char *argv[]) {
	int id, ib, jb, nb, n;
	int i, j, it;
	int numproc, proc_name_len;
	char proc_name[MPI_MAX_PROCESSOR_NAME];
	double lx, ly, hx, hy, bc_down, bc_up, bc_left, bc_right;
	double *data, *temp, *next, *buff;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Get_processor_name(proc_name, &proc_name_len);	
	
	fprintf(stderr, "proc %d(%d) on %s\n", id, numproc, proc_name);
	fflush(stderr);

	MPI_Barrier(MPI_COMM_WORLD);
	
	if (id == 0) {					// Инициализация параметров расчета
		n = 4;						// Размер блока по одному измерению
		nb = 4;						// Размер сетки блоков (процессов) по одному измерению	
		lx = ly = 1.0;
		bc_down = 1.0;
		bc_up = 2.0;
		bc_left = 3.0;
		bc_right = 4.0;
	}

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);			// Передача параметров расчета всем процессам
	MPI_Bcast(&nb, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&lx, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&ly, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&bc_down, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&bc_up, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&bc_left, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&bc_right, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	ib = _ibx(id);		// Переход к 2-мерной индексации процессов 
	jb = _iby(id);
	
	hx = lx / (n * nb);	
	hy = ly / (n * nb);

	data = (double *)malloc(sizeof(double) * (n + 2) * (n + 2));	
	next = (double *)malloc(sizeof(double) * (n + 2) * (n + 2));
	buff = (double *)malloc(sizeof(double) * (n + 2));


	int buffer_size = 4 * (n + 2) * sizeof(double) + MPI_BSEND_OVERHEAD;	// Выделение буфера для буферезированной отправки сообщений
	double *buffer = (double *)malloc(buffer_size);
	MPI_Buffer_attach(buffer, buffer_size);

	for(i = 0; i < n; i++)					// Инициализация блока
		for(j = 0; j < n; j++)
			data[_i(i, j)] = 1.0;

	for(it = 0; it < 500; it++) {
		MPI_Barrier(MPI_COMM_WORLD);
		
// Отправка данных

		if (ib + 1 < nb) {					
			for(j = 0; j < n; j++)
				buff[j] = data[_i(n - 1, j)];
			MPI_Bsend(buff, n, MPI_DOUBLE, _ib(ib + 1, jb), id, MPI_COMM_WORLD);
		}

		if (jb + 1 < nb) {
			for(i = 0; i < n; i++)
				buff[i] = data[_i(i, n - 1)];
			MPI_Bsend(buff, n, MPI_DOUBLE, _ib(ib, jb + 1), id, MPI_COMM_WORLD);
		}
	
		if (ib > 0) {
			for(j = 0; j < n; j++)
				buff[j] = data[_i(0, j)];
			MPI_Bsend(buff, n, MPI_DOUBLE, _ib(ib - 1, jb), id, MPI_COMM_WORLD);
		}
		
		if (jb > 0) {
			for(i = 0; i < n; i++)
				buff[i] = data[_i(i, 0)];
			MPI_Bsend(buff, n, MPI_DOUBLE, _ib(ib, jb - 1), id, MPI_COMM_WORLD);
		}

// Прием данных

		if (ib > 0) {
			MPI_Recv(buff, n, MPI_DOUBLE, _ib(ib - 1, jb), _ib(ib - 1, jb), MPI_COMM_WORLD, &status);
			for(j = 0; j < n; j++)
				data[_i(-1, j)] = buff[j];
		} else {
			for(j = 0; j < n; j++)
				data[_i(-1, j)] = bc_left;
		}

		if (jb > 0) {
			MPI_Recv(buff, n, MPI_DOUBLE, _ib(ib, jb - 1), _ib(ib, jb - 1), MPI_COMM_WORLD, &status);
			for(i = 0; i < n; i++)
				data[_i(i, -1)] = buff[i];
		} else {
			for(i = 0; i < n; i++)
				data[_i(i, -1)] = bc_down;
		}

		if (ib + 1 < nb) {
			MPI_Recv(buff, n, MPI_DOUBLE, _ib(ib + 1, jb), _ib(ib + 1, jb), MPI_COMM_WORLD, &status);
			for(j = 0; j < n; j++)
				data[_i(n, j)] = buff[j];
		} else {
			for(j = 0; j < n; j++)
				data[_i(n, j)] = bc_right;
		}

		if (jb + 1 < nb) {
			MPI_Recv(buff, n, MPI_DOUBLE, _ib(ib, jb + 1), _ib(ib, jb + 1), MPI_COMM_WORLD, &status);
			for(i = 0; i < n; i++)
				data[_i(i, n)] = buff[i];
		} else {
			for(i = 0; i < n; i++)
				data[_i(i, n)] = bc_up;
		}

		MPI_Barrier(MPI_COMM_WORLD);

//		Перевычисление значений температуры

		for(i = 0; i < n; i++)
			for(j = 0; j < n; j++)
				next[_i(i, j)] = 0.5 * ((data[_i(i + 1, j)] + data[_i(i - 1, j)]) / (hx * hx) +
										(data[_i(i, j + 1)] + data[_i(i, j - 1)]) / (hy * hy)) / 
											(1.0 / (hx * hx) + 1.0 / (hy * hy));
		temp = next;
		next = data;
		data = temp;		

		if (id == 0) {
			fprintf(stderr, "it = %d\n", it);
			fflush(stderr);			
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (id != 0) {	
		for(j = -1; j <= n; j++) {
			for(i = -1; i <= n; i++) 
				buff[i + 1] = data[_i(i, j)];
			MPI_Send(buff, n + 2, MPI_DOUBLE, 0, id, MPI_COMM_WORLD);
		}
	} else {
		for(jb = 0; jb < nb; jb++)
			for(j = -1; j <= n; j++)
				for(ib = 0; ib < nb; ib++) {
					if (_ib(ib, jb) == 0)
						for(i = -1; i <= n; i++)
							buff[i + 1] = data[_i(i, j)];
					else
						MPI_Recv(buff, n + 2, MPI_DOUBLE, _ib(ib, jb), _ib(ib, jb), MPI_COMM_WORLD, &status);
					for(i = -1; i <= n; i++)
						printf("%.2f ", buff[i + 1]);
					if(ib + 1 == nb) {
						printf("\n");
						if (j == n)
							printf("\n");
					} else 
						printf(" ");
				}
	}

	MPI_Buffer_detach(buffer, &buffer_size);
	MPI_Finalize();	

	free(buff);
	free(data);
	free(next);
	free(buffer);

	return 0;
}

