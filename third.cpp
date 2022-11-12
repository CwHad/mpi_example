#include <cstdlib>

#include "mpi.h"

typedef unsigned long ULONG;


// Evaluate (ax+c) mod m
ULONG modlin(ULONG a, ULONG x, ULONG c, ULONG m)
{
	return (a * x + c) % m;
}

double rescale(ULONG N, ULONG n, double x1, double x2)
{
	double f = static_cast<double>(n) / static_cast<double>(N);
	return x1 + f * (x2 - x1);
}

int main(int argc, char* argv[])
{

	const ULONG a = 1664525;
	const ULONG c = 1013904223;
	const ULONG m = 4294967296;

	const ULONG sidelen = 65536; // sqrt of m

	const ULONG numtrials = 1000000;

	ULONG i_prev = 12345; // Seed value

	MPI_Init(&argc, &argv);

	const int numproc = MPI::COMM_WORLD.Get_size();
	const int myid = MPI::COMM_WORLD.Get_rank();


	int namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Get_processor_name(processor_name, &namelen);
	std::cout << "Processor ID " << myid << " "
		<< processor_name << " " << numproc << std::endl;;

	if (myid == 0)
	{

		// Master sends N to all the slave processes
		for (int i = 0; i < numtrials; i++)
		{
			const long N = modlin(a, i, c, m);
			// 发送的内容是N，每次发送1个，发送的类型是MPI::LONG, 目标进程是i，标志为0(当前编号)
			MPI::COMM_WORLD.Send(&N, 1, MPI::LONG, i, 0);
		}
		// Master does its own partial sum
		long sum0 = 0;
		for (int i = 1; i <= N / numproc; i++) {
			sum0 += i;
		}
		long result = sum0;
	
	}
	else
	{
		long N;
		MPI::COMM_WORLD.Recv(&N, 1, MPI::LONG, 0, 0);
		long sum1 = 0;
	}

}