#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
     //char *m = (char) *(argv[1]);

     if (argc != 3) {
	printf("Usage: %s <number_of_processors> <instance_filename>\n", argv[0]);
        exit(0);
     }

     int i, m = 0, j=1, n;
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
	if ( (fscanf(fp_in, "%d", &P[i])) > 0) {    // Load of each task
	    PS[i+1] = PS[i] + P[i];                     // Fixed sum array
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
     int Lheur = heuristic_pipelining(P, PS, L, m, n);

     printf("The max load using heuristic pipelining is %d\n\n", Lheur);
     printf("The load split on different processors is:\n");
     for (i=0; i<m; i++) {
	printf("L%d = %d\n", i, L[i]);
     }

}

int heuristic_pipelining(int *P, int *PS, int *L, int m, int n) {
     int Lavg = (PS[n]/m);
     int Lmax = 0;
     int low = 0, hi = 1, j = 0;
     printf("Lavg = %d\n\n", Lavg);
     while (j < m-1) {
	if ( abs((PS[hi] - PS[low]) - Lavg) > abs((PS[hi-1] - PS[low]) - Lavg) ) {
	   L[j] = PS[hi-1] - PS[low];
	   if (L[j]>Lmax) {
		Lmax = L[j];
	   }
           //printf("L%d = %d\n", j, L[j]);
           //printf("low = %d hi = %d\n", low, hi);
           low = hi - 1;
	   j++;
	} else {
	   hi++;
	}
     }
     L[m-1] = PS[n] - PS[low];
     if (L[m-1]>Lmax) {
	Lmax = L[m-1];
     }
     //printf ("L%d = %d\n\n", (m-1), L[m-1]);
     //printf ("Lmax = %d\n", Lmax);
     //printf("low = %d hi = %d\n\n", low, hi);
     //printf("PS[m] = %d, PS[low] = %d\n\n", PS[n], PS[low]);
     return Lmax;
}
