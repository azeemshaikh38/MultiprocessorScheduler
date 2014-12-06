#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
     //char *m = (char) *(argv[1]);
     int i, m = 0, j=1, n;
     if (argc != 3) {
	printf("Usage: %s <number_of_processors> <instance_filename>\n", argv[0]);
        exit(0);
     }

     /*************************Read number of Processors************************/
     for (i = strlen(argv[1]); i > 0; i--) {
         m += ((*(argv[1] + i - 1)) - '0') * j;
 	 //printf("%d %d %d\n", i, ((*(argv[1] + i - 1)) - '0'), j );
         j = j*10;
     }
     //printf("%d\n\n", m);
     
     /********************Start Reading Instance File**************************/
     FILE *fp_in;
     fp_in  = fopen(argv[2],"r");
     
     if (fp_in != NULL) {
	fscanf(fp_in, "%d", &n);        //Number of tasks in the instance file
	//printf("%d\n", n);
     } else {
	printf("Cant open file %s\n", argv[2]);
        exit(0);
     }
     
     int P[n],PS[n+1];
     PS[0] = 0;

     for (i=0; i<n; i++) {
	if ( (fscanf(fp_in, "%d", &P[i])) > 0) {    // Load of each task in P[i] & check if line exists
	    PS[i+1] = PS[i] + P[i];                     // Fixed sum array in PS[i]
            //printf("P%d = %d PS%d = %d\n", i, P[i], (i+1), PS[i+1]);
	} else {
	    printf("Not enough arguments in Instance file\n");
	    exit(0);
	}
     }

     close(fp_in);

     printf("\nNumber of tasks: %d\n", n);
     printf("Number of processors: %d\n", m);     
     printf("Ltotal = %d\n", PS[n]);
     int L[m];

     clock_t tic = clock();
     int Lparametric = parametric_pipelining(P, PS, L, m, n);
     clock_t toc = clock() - tic;

     printf("The max load using parametric pipelining is %d\n\n", Lparametric);
     printf("The load split on different processors is:\n");
     for (i=0; i<m; i++) {
	printf("L%d = %d\n", i, L[i]);
     }
     printf("Time taken is: %d\n", (toc));
}

int parametric_pipelining(int *P, int *PS, int *L, int m, int n) {
     int Lmin = (PS[n]/m);
     int Lmax = PS[n];
     int Lopt = (Lmin + Lmax)/2;
     int optimal_found = 0;

     //int debug_count = 0;
     while (optimal_found != 1) {
        //debug_count++;
	optimal_found = find_load(P, PS, L, m, n, Lopt);
        //printf("Iteration %d: Lmin = %d Lmax = %d Lopt = %d optimal_found = %d\n\n", debug_count, Lmin, Lmax, Lopt, optimal_found); 
	if (optimal_found==0) {
	   Lmin = Lopt + 1;
	   Lopt = (Lmin + Lmax)/2;
	} else if ( (find_load(P, PS, L, m, n, (Lopt - 1))) == 1 ) {
	      Lmax = Lopt - 1;
	      optimal_found = 0;
	      Lopt = (Lmin + Lmax)/2;
	} else {
	      find_load(P, PS, L, m, n, Lopt);
	} 

     }

    return Lopt; 
}

int find_load(int *P, int *PS, int *L, int m, int n, int Lopt) {
    int j = 0, low = 0, hi = 0;

    while (j < m-1) {
        printf ("Entered\n");
	if(PS[hi] - PS[low] > Lopt) {
	    L[j] = PS[hi-1] - PS[low];
            //printf("L%d = %d low = %d PS[low] = %d hi = %d PS[hi] = %d\n", j, L[j], low, PS[low], hi-1, PS[hi-1]);
	    low = hi - 1;
	    j++;
	} else {
	    if (hi < n) {
               hi++;
            } else {
	       L[j] = PS[hi] - PS[low];
	       //printf("L%d = %d low = %d PS[low] = %d hi = %d PS[hi] = %d\n", j, L[j], low, PS[low], hi, PS[hi]);
               low = hi;
	       break;
	    }
	}
    }

    L[m-1] = PS[n] - PS[low];
    //printf("L%d = %d low = %d PS[low] = %d hi = %d PS[hi] = %d\n", m-1, L[m-1], low, PS[low], hi-1, PS[hi-1]);
    return (L[m-1] <= Lopt);
}
