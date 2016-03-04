#include <omp.h>
#include <stdbool.h>
#include "../gtmp.h"

/*
    From the MCS Paper: A sense-reversing centralized barrier

    shared count : integer := P
    shared sense : Boolean := true
    processor private local_sense : Boolean := true

    procedure central_barrier
        local_sense := not local_sense // each processor toggles its own sense
	if fetch_and_decrement (&count) = 1
	    count := P
	    sense := local_sense // last processor toggles global sense
        else
           repeat until sense = local_sense
*/

void gtmp_init(int num_threads){
	count = num_threads;
	sense = true;
	local_sense = true;
}

void gtmp_barrier(){
	local_sense = !local_sense;

	#pragma omp atomic
	count -= 1;

	if (count == 0) {
		count = omp_get_thread_num();
		sense = local_sense;
	}
	else {
		while (sense != local_sense) {}
	}
}

void gtmp_finalize(){}
