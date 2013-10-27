//		Matei Razvan Madalin
//		313CA

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

unsigned char *arena; 
int NBG = 1;
// NGB = Numar Blocuri de Gestiune.
// Indexul de start este considerat bloc de gestiune.


void initialize(int N) {
	arena = calloc(N, sizeof(char));
} 


void finalize() {
	free(arena);
}


void dump(int N) {

	unsigned char *p=arena;
	int i, j;
	
	for(i=0; i<N; i+=16) {
		printf("%08X ", i);
		for(j=i; j < i+16 && j < N; j++) {
			if(j == i+8 ) {
				printf(" ");
			}
			printf("%02X ", *(p+j));
		}
		printf("\n");
	}
	printf("%08X\n", N);
	
}


void alloc(int N, int *urm, int *pre, int *dim, int size) {

	unsigned char *p = arena;
	int i=0, j;
	
	// Conditia ce verifica daca arena este goala.
	if(urm[0] == 0) {
	
		// 	Conditia ce verifica daca este loc suficient
		// in arena pentru a aloca un bloc.
		if( (N-4) >= (size+12) ) {
			
			NBG=2;
			
			urm[1]=0;
			pre[1]=0;
			dim[1]=size+12;
			
			urm[0]=4;
			
			// Consideram cunoscut sizeof(int) = 4.
			*(int *)(p)    = urm[0];
			*(int *)(p+4)  = urm[1];
			*(int *)(p+8)  = pre[1];
			*(int *)(p+12) = dim[1];
			
			printf("16\n");

			return;
		}
		
		else {
			printf("0\n");
			return;
		}
		
	}

	// Se cauta spatiu intre doua blocuri vecine.
	while(urm[i] != 0) { 
	
		// Conditia ce verifica daca este spatiu intre doua blocuri vecine.
		if( (urm[i] - (pre[i+1] + dim[i])) >= (size + 12) ) {

			NBG++;
			
			// 	Se decaleaza la dreapta valorile vectorilor urm, pre si dim de dupa
			// valorile corespunzatoare noului bloc.
			for(j = NBG-1; j >= i+2; j--) {
				urm[j]=urm[j-1];
				pre[j]=pre[j-1];
				dim[j]=dim[j-1];
			}
			
			urm[i+1] = urm[i];
			pre[i+1] = pre[i+2];
			dim[i+1] = size+12;
			
			urm[i]   = pre[i+1] + dim[i];
			pre[i+2] = pre[i+1] + dim[i];
			
			*(int *)(p+pre[i+2])   = urm[i+1];
			*(int *)(p+pre[i+2]+4) = pre[i+1];
			*(int *)(p+pre[i+2]+8) = dim[i+1];
			
			*(int *)(p+pre[i+1])   = urm[i];
			*(int *)(p+urm[i+1]+4) = pre[i+2];
			
			printf("%d\n", urm[i]+12);

			return;
		}
		
		i++;
		
	}

	// Se cauta spatiu dupa ultimul bloc.
	if(urm[NBG-1] == 0) {
	
		// Conditia ce verifica daca este spatiu suficient dupa ultimul bloc.
		if( (N - urm[NBG-2] - dim[NBG-1]) >= (size+12) ) {

			NBG++;
			
			urm[(NBG-1)] = 0;
			pre[(NBG-1)] = urm[(NBG-3)];
			dim[(NBG-1)] = size+12;
			
			urm[NBG-2] = (urm[NBG-3]+dim[NBG-2]);
			
			*(int *)(p+urm[NBG-2])   = urm[NBG-1];
			*(int *)(p+urm[NBG-2]+4) = pre[NBG-1];
			*(int *)(p+urm[NBG-2]+8) = dim[NBG-1];
			
			*(int *)(p+urm[NBG-3])   = urm[NBG-2]; 
		
			printf("%d\n", urm[NBG-2]+12);

			return;
		}
	}
	
	// Daca nu a fost gasit spatiu va fi afisata valoarea 0.
	printf("0\n");
	return;
	
}


void elib(int N, int *urm, int *pre, int *dim, int INDEX) {

	unsigned char *p = arena;
	int i=0, j;
	
	// Se cauta blocul ce urmeaza a fi eliberat.
	while(i <= NBG-1) {
	
		if( (urm[i]+12) == INDEX ) {
		
			// Urmeaza sa fie eliminat din arena blocul cu indexul i+1.
			
			urm[i] = urm[i+1];
			*(int *)(p+pre[i+1]) = urm[i+1];
			
			// 	Conditia ce verifica daca blocul ce se doreste 
			// a fi eliminat nu este chiar ultimul bloc din arena.
			if( i != (NBG-2) ) {
				pre[i+2] = pre[i+1];
				*(int *)(p+urm[i+1]+4) = pre[i+1];   // 4 = sizeof(int)
			
				// 	Odata eliminat un bloc din arena, NGB va fi mai mic cu 1.
				// De aceea vor fi decalate la stanga toate valorile vectorilor 
				// urm, pre si dim de dupa valorile corespunzatoare blocului eliminat.
				for(j=i+1; j <= (NBG-2); j++) {
					urm[j] = urm[j+1];
					pre[j] = pre[j+1];
					dim[j] = dim[j+1];
				}
			}
			
			NBG--;
			return;  
		}
		
		i++;
		
	}
	
}


void fill(int *urm, int *pre, int *dim, int INDEX, int SIZE, int VALUE) {

	unsigned char *p = arena+INDEX;
	int i=0;
	
	// Se da valoarea VALUE octetilor de la INDEX pana la INDEX + SIZE.
	do {
		*(p+i) = VALUE;
		i++;
	} while (i < SIZE);
	
	// 	In cazul in care au fost modificate blocurile de gestiune,
	// vectorii urm, pre, dim vor fi modificati corespunzator valorilor
	// din arena.
	*urm = *(int *)arena;
	if( *urm != 0 ) {
		NBG = 2;
	}
	for(i=1; i<NBG; i++) {
		*(urm+i) = *(int *)(arena+urm[i-1]);
		*(pre+i) = *(int *)(arena+urm[i-1]+4);
		*(dim+i) = *(int *)(arena+urm[i-1]+8);
		if( *(urm+i) != 0 ) {
			NBG++;
		}
	}	
	
}


// conditie: 1 - free
//           2 - usage
//           3 - allocations 
void showfree(int N, int *urm, int *pre, int *dim, int conditie) {

	int nrblocksfree=0, nrbytesfree=0, nrblocksused=0, nrbytesused=0, i;
	float eff, frag=0;
	
	if( *(int *)arena == 0 ) {
	
		nrbytesfree = N-4;
		nrbytesused = 0;
		nrblocksfree = 1;
		nrblocksused = 0;
		
	}
	
	else {

		for(i=1; i <= NBG-1; i++) {
			nrbytesused += (dim[i]-12);
		}
		nrblocksused = NBG-1;
	
		for(i=0; i <= NBG-2; i++) {
			if( ( urm[i] - (pre[i+1] + dim[i]) ) > 0 ) {
				nrblocksfree++;
				nrbytesfree += ( urm[i] - (pre[i+1] + dim[i]) );
			}
		}
	
		if( (N - (urm[NBG-2] + dim[NBG-1]) ) > 0 ) {
			nrblocksfree++;
			nrbytesfree += ( N - (urm[NBG-2] + dim[NBG-1]) ) ;
		}
		
	}
	
	if(conditie == 1) {
		printf("%d blocks (%d bytes) free\n", nrblocksfree, nrbytesfree);
	}
	
	if(conditie == 2) {
		printf("%d blocks (%d bytes) used\n", nrblocksused, nrbytesused);
		eff = ( (float)nrbytesused / (N - nrbytesfree)) * 100;
		printf("%d%% efficiency\n", (int)eff );
		if( nrblocksused != 0 ) {
			frag = ( (nrblocksfree-1) / (float)nrblocksused) * 100;
		}
		printf("%d%% fragmentation\n", (int)frag ); 
	}
	
	if(conditie == 3) {
		printf("OCCUPIED 4 bytes\n");
		
		if( *(int *)arena == 0 ) {
			printf("FREE %d bytes\n", nrbytesfree);
		}
		
		else {
		
			for(i=0; i <= NBG-2; i++) {
				if( (urm[i] - (pre[i+1] + dim[i])) > 0) {
					printf("FREE %d bytes\n", (urm[i] - (pre[i+1] + dim[i])) );
				}
				printf("OCCUPIED %d bytes\n", dim[i+1]);
			}		
			if( (N - (urm[NBG-2] + dim[NBG-1])) > 0 ) {
				printf("FREE %d bytes\n", (N - (urm[NBG-2] + dim[NBG-1])) );
			}
			if( *(int *)arena == 0 ) {
				printf("FREE %d bytes\n", nrbytesfree);
			}
			
		}
	}
	
}

	
int main() {

	int nrinit, nralloc, nrelib, nrINDEX, nrSIZE, nrVALUE;
	int *urm = malloc( 100 * sizeof(int));
	int *pre = malloc( 100 * sizeof(int));
	int *dim = malloc( 100 * sizeof(int));
	char *cmd     = malloc( 20 * sizeof(char));
	char *comanda = malloc( 20 * sizeof(char));
	
	urm[0] = 0;
	dim[0] = 4;
	
	fgets(comanda, 20, stdin);
	cmd = strtok(comanda, " ");
	cmd = strtok(NULL, "\n");
	nrinit = atoi(cmd);
	initialize(nrinit);
	
	do {
		fgets(comanda, 20, stdin);
		
		if( strcmp(comanda, "FINALIZE\n") == 0 ) {
			break;
		}
		
		if( strcmp(comanda, "DUMP\n") == 0 ) {
			dump(nrinit);
		}
		
		cmd = strtok(comanda, " ");
		
		if( strcmp(cmd, "ALLOC") == 0 ) {
			cmd = strtok(NULL, "\n");
			nralloc = atoi(cmd);
			alloc(nrinit, urm, pre, dim, nralloc);
		}
		
		if( strcmp(cmd, "FREE") == 0 ) {
			cmd = strtok(NULL, "\n");
			nrelib = atoi(cmd);
			elib(nrinit, urm, pre, dim, nrelib);
		}
		
		if( strcmp(cmd, "FILL") == 0 ) {
			cmd = strtok(NULL, " ");
			nrINDEX = atoi(cmd);
			cmd = strtok(NULL, " ");
			nrSIZE = atoi(cmd);
			cmd = strtok(NULL, "\n");
			nrVALUE = atoi(cmd);
			fill(urm, pre, dim, nrINDEX, nrSIZE, nrVALUE);
		}
		
		if( strcmp(cmd, "SHOW") == 0 ) {
			cmd = strtok(NULL, "\n");
			if( strcmp(cmd, "FREE") == 0 ) {
				showfree(nrinit, urm, pre, dim, 1);
			}
			if( strcmp(cmd, "USAGE") == 0 ) {
				showfree(nrinit, urm, pre, dim, 2);
			}
			if( strcmp(cmd, "ALLOCATIONS") == 0 ) {
				showfree(nrinit, urm, pre, dim, 3);
			}
		}
		
	} while (1);
	
	finalize();
	
	free(urm);
	free(pre);
	free(dim);
	return 0;
}
