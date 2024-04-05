original: fliphash.c only
	gcc -fopenmp -o original fliphash.c
only: onlyfliphash.c
	gcc -o onlyfliphash onlyfliphash.c