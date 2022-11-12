
#include <cstdlib>

#include "mpi.h"

int main(int argc,char* argv[])
{
  MPI_Init(&argc,&argv);
  const int numproc = MPI::COMM_WORLD.Get_size();
  const int myid = MPI::COMM_WORLD.Get_rank();

  // What is THIS processor name (hostname)?
  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Get_processor_name(processor_name, &namelen);
  std::cout << "Processor ID " << myid << " " 
	    << processor_name << " " <<  numproc << std::endl;;

  // Do this if master
  if (myid == 0) {

    const long N = std::atoi(argv[1]);

    // Master sends N to all the slave processes
    for  (int i = 1; i < numproc;i++) {
      MPI::COMM_WORLD.Send(&N, 1, MPI::LONG, i,0);
    }	

    // Master does its own partial sum
    long sum0 = 0;
    for(int i = 1; i <= N/numproc; i++){
      sum0 += i;
    }
    long result = sum0;
    for (int i = 1; i < numproc;i++) {
      long sum1;
      MPI::COMM_WORLD.Recv(&sum1, 1, MPI::LONG, i,0);
      result += sum1;
      std::cout << "Node: " << i << " " << result << " " << sum1 << std::endl;
    }
    std::cout << "The sum is " << result << std::endl;
  } 

  else {
    long N;
    MPI::COMM_WORLD.Recv(&N, 1, MPI::LONG, 0, 0);
    long sum1 = 0;
    for(int i = (N/numproc*myid)+1; i <= (N/numproc*(myid+1)); i++) {
      sum1 += i;
    }	
    MPI::COMM_WORLD.Send(&sum1, 1, MPI::LONG, 0, 0);
  }

  MPI::Finalize();
}
