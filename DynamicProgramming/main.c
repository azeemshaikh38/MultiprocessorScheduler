#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

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

     int *L;
     L = malloc(sizeof(int)*m*(n+1));

     int Ldynamic = dynamic_pipelining(P, PS, L, m, n);

     printf("The max load using dynamic pipelining is %d\n\n", Ldynamic);
     //printf("The load split on different processors is:\n");
     //for (i=0; i<m; i++) {
     //	printf("L%d = %d\n", i, L[i]);
     //}
}

int dynamic_pipelining(int *P, int *PS, int *L, int m, int n) {
     int i,j,k;
     for (i = 0; i<=n; i++) {
	L[i] = PS[i];
     }
    
     int val, min;
     for (j=1; j<m; j++) {
        for (i=1; i<=(n - (m-j-1)); i++) {
	    min = PS[n] + 1;
	    for (k=0; k<=i; k++) {
		val = MAX((PS[i] - PS[i-k]), L[(n+1)*(j-1)+(i-k)]);
		if (val < min) {
		   min = val;
		}
	    }
	    L[(n+1)*j + i] = min;
	}
     }

     return L[(m-1)*(n+1) + n]; 
}

