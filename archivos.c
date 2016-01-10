#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

/*
 Función para poder realizar la ejecución de las hebras.
 */

void *funcionThread(void *argumentos) {
	//char *argInterno = (char *)argumentos;
	int *argInt = (int *)argumentos;
	printf("soy una hebra, mi id %d y mi equipo es %d\n", argInt[1], argInt[0]);
	//varGlobal++;
	//pthread_mutex_lock(&m1);
	//Comienza sección crítica
	//varGlobal = varGlobal+1;
	//pthread_mutex_unlock(&m1); //fin sección crítica
	
	//sleep(1);
	pthread_exit(NULL);
}

/*
Función que permite obtener la matriz de listas
que es contenido en el archivo de entrada
entrada: nombre del archivo que debe ser leído, se asume que está
				en la misma carpeta que el ejecutable
*/
int ** leerListas(char* entrada);
/*
	Permite contar la cantidad de líneas que posee el archivo
	para así posteriormente leer las listas
	archivo = archivo al cual se le contarán las líneas
*/
int contarLineas(FILE* archivo);

/*
	Función que permite eleminar los elementos repetidas de las listas leídas
	para así cumplir con la definición de conjuntos (no poseen repetidos)
	A = lista de elementos del conjunto
*/
int*  eliminarRepetidos(int * A);

//Función obtenida de roseta code.
//Es utilizada para el ordenamiento de las listas
void quickSort (int *a, int n);

//Modificación de la función anterior, que permite crear el ordenamiento de la
//matriz según el largo de las listas ingresadas
void quickSortMat (int **a, int n);

//Función que permite intersetar dos arreglos
//A: arreglo 1
//B: arreglo 2
//Retorna el arreglo resultado de la intersección
int* intersectar(int* A, int* B);

/*Función principal*/
int main(int argc,char*argv[]){

	printf("      ___           ___           ___           ___           ___\n");
	printf("     /\\__\\         /\\  \\         /\\  \\         /\\  \\         /\\  \\\n");
	printf("    /:/  /        /::\\  \\       /::\\  \\       /::\\  \\       /::\\  \\\n");
	printf("   /:/__/        /:/\\:\\  \\     /:/\\:\\  \\     /:/\\:\\  \\     /:/\\:\\  \\\n");
	printf("  /::\\  \\ ___   /::\\~\\:\\  \\   /::\\~\\:\\__\\   /::\\~\\:\\  \\   /::\\~\\:\\  \\\n");
	printf(" /:/\\:\\  /\\__\\ /:/\\:\\ \\:\\__\\ /:/\\:\\ \\:|__| /:/\\:\\ \\:\\__\\ /:/\\:\\ \\:\\__\\\n");
	printf(" \\/__\\:\\/:/  / \\:\\~\\:\\ \\/__/ \\:\\~\\:\\/:/  / \\/_|::\\/:/  / \\/__\\:\\/:/  /\n");
	printf("      \\::/  /   \\:\\ \\:\\__\\    \\:\\ \\::/  /     |:|::/  /       \\::/  /\n");
	printf("      /:/  /     \\:\\ \\/__/     \\:\\/:/  /      |:|\\/__/        /:/  /\n");
	printf("     /:/  /       \\:\\__\\        \\::/__/       |:|  |         /:/  /\n");
	printf("     \\/__/         \\/__/         ~~            \\|__|         \\/__/\n\n");
	printf("      ___       ___           ___           ___                       ___\n");
	printf("     /\\__\\     /\\  \\         /\\__\\         /\\  \\          ___        /\\  \\\n");
	printf("    /:/  /    /::\\  \\       /::|  |       /::\\  \\        /\\  \\      /::\\  \\\n");
	printf("   /:/  /    /:/\\:\\  \\     /:|:|  |      /:/\\:\\  \\       \\:\\  \\    /:/\\:\\  \\\n");
	printf("  /:/  /    /::\\~\\:\\  \\   /:/|:|  |__   /:/  \\:\\__\\      /::\\__\\  /::\\~\\:\\  \\\n");
	printf(" /:/__/    /:/\\:\\ \\:\\__\\ /:/ |:| /\\__\\ /:/__/ \\:|__|  __/:/\\/__/ /:/\\:\\ \\:\\__\\\n");
	printf(" \\:\\  \\    \\/__\\:\\/:/  / \\/__|:|/:/  / \\:\\  \\ /:/  / /\\/:/  /    \\/__\\:\\/:/  /\n");
	printf("  \\:\\  \\        \\::/  /      |:/:/  /   \\:\\  /:/  /  \\::/__/          \\::/  /\n");
	printf("   \\:\\  \\       /:/  /       |::/  /     \\:\\/:/  /    \\:\\__\\          /:/  /\n");
	printf("    \\:\\__\\     /:/  /        /:/  /       \\::/__/      \\/__/         /:/  /\n");
	printf("     \\/__/     \\/__/         \\/__/         ~~                        \\/__/\n");


	int Ei, Hi, c;
	char* inputFile = NULL;

  opterr = 0;
	while ((c = getopt (argc, argv, ":a:b:c:")) != -1)
	    switch (c)
	      {
	      case 'a':
					Ei = atoi(optarg);
	        break;
	      case 'b':
					Hi = atoi(optarg);
	        break;
	      case 'c':
	        inputFile = optarg;
	        break;
				case ':':
				fprintf(stderr, "%s: option '-%c' requires an argument\n",
                argv[0], optopt);
				return 1;
        break;
	      case '?':
	      	if (isprint (optopt))
	          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	        else
	          fprintf (stderr,
	                   "Unknown option character `\\x%c'.\n",
	                   optopt);
	        return 1;
	      default:
	        abort ();
	      }

<<<<<<< HEAD
  int ** listas = leerListas("prueba");
	int i,j,k;
=======
	//printf("Serán creados %d equipos, %d hebras y se abrirá el archivo %s\n",Ei,Hi,inputFile);

	int ** listas = leerListas("prueba");
	int i,j;
>>>>>>> 9e1c5dbe04668429e754a064fd34dee57d505080
	int cantListas = contarLineas(fopen("prueba","r"));
	printf("Cantidad de listas leídas %d\n",cantListas);

	//Se eliminan los elementos repetidos de cada una de las listas leídas
	printf("\n");
	printf("\n ****************************************\n");
	printf("\n *********LISTAS SIN REPETIDOS***********\n");
	printf("\n *************Y ORDENADOS****************\n");
	printf("\n ****************************************\n");
	printf("\n");
	for(i=0;i<cantListas;i++){
		listas[i] =eliminarRepetidos(listas[i]);
		quickSort(listas[i]+1,listas[i][0]);
			for(j=0;j<=listas[i][0];j++){
				if(j==0) printf("Lista %d, con repetidos eliminados.\nCantidad de elementos %d\n",i,listas[i][j]);
				printf("%d \n",listas[i][j]);
			}
	}

	//Se ordenan las listas según sus largos
	quickSortMat(listas,cantListas);
	printf("\n");
	printf("\n ****************************************\n");
	printf("\n ******LISTAS ORDENADAS SEGÚN LARGO******\n");
	printf("\n ****************************************\n");
	printf("\n");
	for(i=0;i<cantListas;i++){
			for(j=0;j<=listas[i][0];j++){
				if(j==0) printf("Lista %d, con repetidos eliminados.\nCantidad de elementos %d\n",i,listas[i][j]);
				printf("%d \n",listas[i][j]);
			}
	}
<<<<<<< HEAD
	printf("No entiendo que está pasando\n");
	printf("Se procede a intersectar la lista\n");
	for(j=0;j<=listas[4][0];j++){
		if(j==0) printf("Lista %d, con repetidos eliminados.\nCantidad de elementos %d\n",4,listas[4][j]);
		printf("%d ",listas[4][j]);
	}
	printf("\n");

	for(j=0;j<=listas[5][0];j++){
		if(j==0) printf("Lista %d, con repetidos eliminados.\nCantidad de elementos %d\n",5,listas[5][j]);
		printf("%d ",listas[5][j]);
	}
	printf("\n");

	int *S = listas[0];
	int * aux;
	for(i = 1;i<cantListas;i++){
		aux = intersectar(S,listas[i]);

			printf("Elementos aux[0] = %d\n",aux[0]);

		free(aux);
	}

	printf("\n\nCantidad de elementos de la interesección %d\n",aux[0]);
	printf("\n\nCantidad de listas %d\n",cantListas);

	/*for(i=1;i<=S[0];i++){
		printf("Elementos S[%d] = %d\n",i,S[i]);
	}*/
=======
	
	//comenzar a crear equipos con hebras
	
	int ei = 3; //cantidad de equipos
	int hi = 4; //cantidad de hebras por equipos
	
	//int i,j;
	int cant = ei*hi; //cantidad total de hebras
	//se crea un matriz con los id del equipo y de la hebra
	int **arrId = malloc(cant*sizeof(int*));
	int cont = 0;
	for (i = 0; i < ei; i++) {
		for(j = 0;j<hi;j++){
			arrId[cont] = malloc(2*sizeof(int));
			arrId[cont][0] = i; //s guardara el equipo
			arrId[cont][1] = j; //s guardara el numero de hebra del equipo
			cont++; 
		}
	}
	
	//se crea un array de hebras
	pthread_t *arr_threads = (pthread_t *)malloc(cant*sizeof(pthread_t));
	//comenzar a crear las hebras por equipo
	cont = 0; 
	for(i = 0;i < ei;i++){
		for(j=0;j < hi;j++){
			pthread_create(&arr_threads[cont], NULL, &funcionThread, arrId[cont]);
			cont++;
		}
	}
	
	for (i = 0; i < cant; i++) {
		pthread_join(arr_threads[i], NULL); //espera a que la hebra termine
	}
	
	
	printf("Se termino con todas las hebras\n");
	
>>>>>>> 9e1c5dbe04668429e754a064fd34dee57d505080
  return 0;
}

int ** leerListas(char* entrada){
	FILE* archivoEntrada;
	if((archivoEntrada = fopen(entrada,"r"))==NULL){
		fprintf(stderr, "Error: Verifica la existencia del archivo. \n" );
	} else{
		int lineas = contarLineas(archivoEntrada);
		fopen(entrada,"r");
		int ** conjuntoDeListas = malloc(sizeof(int*)*lineas);
    int j=0;
		while(j<lineas){
			int cantidad;
			fscanf(archivoEntrada,"%d",&cantidad);
			int i;
			int* lista = malloc(sizeof(int)*(cantidad+1));
			lista[0]=cantidad;
			for(i=1; i<=cantidad;i++){
				fscanf(archivoEntrada,"%d",&lista[i]);
				conjuntoDeListas[j] = lista;
			}
			j++;
		}
		return conjuntoDeListas;
  }
}


int contarLineas(FILE* archivo){
  int i; //Cantidad de líneas
  char temporal;
  while((temporal=fgetc(archivo))!=EOF){
    if(temporal=='\n'){
      i++;
      }
  }
	i++;
  fclose(archivo);
  return i;
}

int*  eliminarRepetidos(int * A){
	int * B = malloc(sizeof(int)*(A[0]+1)); //lista sin repetidos
	int i;
	int k;
	int flag = 0;
	B[0] = 0;
	for(i=1;i<=A[0];i++){
		for(k=1;k<=B[0];k++){
			if(B[k]==A[i]){
				flag = 0;
				break;
			} else{
				flag = 1;
			}
		}
		if(i==1){
			B[1] = A[1]; //Se agrega un elementos
			B[0] =B[0]+1; //Se aumenta el largo de la lista
		}
		if(flag == 1){
			B[k] = A[i]; //Si no se encontró un elemento repetido, el nuevo se agrega
			B[0] = B[0]+1; //a la lista sin repetidos;
			flag = 0;
		}
	}
	free(A); //Se elimina la lista de entrada
	A = NULL; //Se deja apuntando a null
	B = realloc(B,B[0]+1); //Se realoca 1313 woooooh!(disculpas me emocioné) la memoria,
													//para la lista nueva
	return B;
}

void quickSort (int *a, int n) {
    int i, j, p, t;
    if (n < 2)
        return;
    p = a[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i] < p)
            i++;
        while (p < a[j])
            j--;
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    quickSort(a, i);
    quickSort(a + i, n - i);
}

void quickSortMat (int **a, int n) {
    int i, j, *p, *t;
    if (n < 2)
        return;
    p = a[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i][0] < p[0])
            i++;
        while (p[0] < a[j][0])
            j--;
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    quickSortMat(a, i);
    quickSortMat(a + i, n - i);
}

int* intersectar(int* A, int* B){
	int *aux = malloc(sizeof(int)*(A[0]+B[0]+1));
	aux[0] = 0;
	int i,j,k = 1;
	for(i=1;i<=A[0];i++){
		for(j=1;j<=B[0];j++){
			printf("A[%d] = %d y B[%d] = %d\n",i,A[i],j,B[j]);
			if(A[i]==B[j]){
				printf("Encontré elementos iguales  en A[%d] = %d y B[%d] = %d\n",i,A[i],j,B[j]);
				aux[k] = A[i];
				k++;
				aux[0] = aux[0]+1;
				break;
			}
		}
	}
	aux = realloc(aux,aux[0]+1);
	return aux;
}
