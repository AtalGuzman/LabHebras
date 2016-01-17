#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <time.h>
#define CLOCK_PER_SECOND 100000000.0
/*
 Esta es la estructura para poder pasar todos los datos cuando se realiza el
 llamado a la función
 */
typedef struct thread_data{
	int id_equipo;
	int id_hebra;
	int inicio;
	int fin;
	int cantidadDeHebras;
	int *listaGeneral;
	int *interseccionLocal;
	int *lista;
	int posResultados;
} t_data;

typedef struct thread_data_equipo{
 int id_equipo;
 int *lista;
 int *interseccion;
 int cantidadHebras;
} t_data_equipo;

pthread_mutex_t lock;
pthread_mutex_t lock2;
pthread_mutex_t lock3;
pthread_barrier_t *barreras1;
pthread_barrier_t *barreras2;

int ** listas;
int cantListas;
int **ListasGenerales;
int **listasTemporales;
int* ult;
float **resultadosTiempo;
float **resultadosTiempoHebras;
float **resultadosPromedioHebras;
int **intersecciones;
int ei, hi, c;
/*
 Función que es la llamada al crear cada hebra
 la cual será encargada de poder recorrer las listas.
 Entrada: una estructura que nos indica el id de la hebra
          el id del equipo al cual pertenece y por ultimo
          le pasa las listas con las cuales debe trabajar.
 */
void *equipos(void *t_data_equipo);
void *concursantes(void *tdata);

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

void quickSortMatFloat (float **a, int n);
//Función que permite intersetar dos arreglos
//A: arreglo 1
//B: arreglo 2
//Retorna el arreglo resultado de la intersección
int* intersectar(int* A, int* B, int inicio, int final);

//Función que permite realizar la búsqueda binaria
//Recibe el puntero del arreglo, el final y el inicio del arreglo, y
//El valor que será buscados
int BusquedaBinaria(int *k, int kInicial, int kFinal, int buscar);

//Función que permite mostrar los resultados finales
//Recibe como parámetro de entradas una arreglo que permite
void escribirResultados(float **resultados,float **individual,float **promedio, int ** inter);

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


	char* inputFile = NULL;

  opterr = 0;
	if(argc < 7){
		fprintf(stderr, "Error: faltan parámetros de entrada \nUso: ./competencia -g cantidadEquipos -h cantidadHebras -i archivo de entrada\n");
		return 1;
	}
	//Los parámetros se pasan así ./salida -g entero -h entero -i string
	while ((c = getopt (argc, argv, ":g:h:i:")) != -1)
	    switch (c)
	      {
	      case 'g':
					ei = atoi(optarg);
	        break;
	      case 'h':
					hi = atoi(optarg);
	        break;
	      case 'i':
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

	if(ei==0 || hi==0){
		fprintf(stderr, "La cantidad de equipos y la cantidad de concursantes deben ser diferentes de 0\n");
		return 1;
	}
		///Prueba para el paso de parámetros
	printf("Se crearán %d equipos de hebras, de %d hebras cada uno y se leerá desde el archivo %s\n",ei,hi,inputFile);
  listas = leerListas(inputFile);
	int i,j,k;
	cantListas = contarLineas(fopen(inputFile,"r"));

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
				if(j==0) printf("\nLista %d, con repetidos eliminados.\nCantidad de elementos %d\n",i,listas[i][j]);
				printf("%d ",listas[i][j]);
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
				if(j==0) printf("\nLista %d, con repetidos eliminados.\nCantidad de elementos %d\n",i,listas[i][j]);
				printf("%d ",listas[i][j]);
			}
	}

	printf("\n");

	ult = malloc(sizeof(int)*ei);
	for(i=0;i<ei;i++){
		ult[i] = 1;
	}

	barreras1 = malloc(sizeof(pthread_barrier_t)*ei);
	barreras2 = malloc(sizeof(pthread_barrier_t)*ei);
	resultadosTiempo = malloc(sizeof(float*)*ei); //Resultados de cada equipo
	resultadosTiempoHebras = malloc(sizeof(float*)*ei*hi);
	resultadosPromedioHebras = malloc(sizeof(float*)*ei*hi); //Se les asigna espacio a los lugares de la cosa
	intersecciones = malloc(sizeof(int*)*ei);

	for(i = 0; i< ei*hi;i++) resultadosTiempoHebras[i] = malloc(sizeof(int)*3);
	for(i = 0; i< ei*hi;i++){
		resultadosPromedioHebras[i] = malloc(sizeof(int)*4);
		resultadosPromedioHebras[i][3] = 1;
	}
	for(i = 0; i < ei;i++){
		pthread_barrier_init(&barreras1[i],NULL,hi);
		pthread_barrier_init(&barreras2[i],NULL,hi);
		resultadosTiempo[i] = malloc(sizeof(float)*2);
		resultadosTiempo[i][1] = i;
	}



	int cant = ei*hi; //cantidad total de hebras
	//se crea un matriz con los id del equipo
	int *arrId = malloc(cant*sizeof(int));
	int cont = 0;
	for (i = 0; i < ei; i++) {
			arrId[i] = i; //s guard
		}

	//se crea un array de hebras
	pthread_t *arr_threads = (pthread_t *)malloc(ei*sizeof(pthread_t));

	//comenzar a crear las hebras por equipo
	t_data_equipo* tDataEquipo;
	cont = 0;
	for(i = 0;i < ei;i++){
			tDataEquipo = (t_data_equipo *) malloc(sizeof(t_data_equipo));
			tDataEquipo->id_equipo = arrId[cont];
			tDataEquipo->lista =
			intersecciones[i] = malloc(sizeof(int)*listas[0][0]+1);
			tDataEquipo->lista = intersecciones[i];
			for(j=0;j<=listas[0][0];j++){
				tDataEquipo->lista[j] = listas[0][j];
			}
			tDataEquipo->interseccion = malloc(sizeof(int)*(listas[cantListas-1][0]+1));
			tDataEquipo->cantidadHebras = hi;
			pthread_create(&arr_threads[cont], NULL, &equipos, (void *)tDataEquipo);
			cont++;
		}

	for(i=0;i<ei;i++){
		pthread_join(arr_threads[i], NULL);
	}

	printf("Se termino con todas las hebras\n");
	 //Se procede a escribir el archivo final
	 for(i=0;i<ei*hi;i++){
		 resultadosPromedioHebras[i][0] = resultadosPromedioHebras[i][0]/resultadosPromedioHebras[i][3];
	 }
	escribirResultados(resultadosTiempo,resultadosTiempoHebras,resultadosPromedioHebras,intersecciones);

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

void quickSortMatFloat (float **a, int n) {
    int i, j;
		float *p, *t;
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
    quickSortMatFloat(a, i);
    quickSortMatFloat(a + i, n - i);
}
//Inicio y final de B
int* intersectar(int* A, int* B, int inicio, int final){
	int *aux = malloc(sizeof(int)*(A[0]+1));
	aux[0] = 0;
	int i,j,k = 1;
	for(i=1;i<=A[0];i++){
		if(BusquedaBinaria(B,inicio,final,A[i]) == 1){
			aux[k] = A[i];
			k++;
			aux[0] = aux[0]+1;} }
	aux = realloc(aux,aux[0]+1);
	return aux;
}

int BusquedaBinaria(int *k, int kInicial, int kFinal, int buscar){
		int inicio = kInicial, final = kFinal;
		while(inicio <= final){
			int mitad = (inicio + final)/2;
				if (k[mitad] > buscar)
						final = mitad - 1;
				else if (k[mitad] < buscar)
						inicio = mitad + 1;
				else
						return 1;
		}
		return -1;
}

void *equipos(void *tDataEquipo) {
	int start = clock();
	t_data_equipo *dataEquipo = (t_data_equipo*) tDataEquipo;
			int i;
			int *S = (int *)dataEquipo->lista;
			int *S2 = (int*)dataEquipo->interseccion;
			S2[0] = S[0]; //Se agrega la cantidad de elementos igual que el primero
											//en el peor de los casos el tamaño será igual
			int CH = (int)dataEquipo->cantidadHebras;

			int *resultadosIndex = malloc(sizeof(int)*ei*hi);
			for(i = 0; i< ei*hi;i++){
				resultadosTiempoHebras[i] = malloc(sizeof(int)*3);
				resultadosIndex[i] = i;
			}

			int* id = malloc(sizeof(int)*CH);
			pthread_t *arr_Concursantes = (pthread_t *)malloc(CH*sizeof(pthread_t));
			printf("La cantidad de hebras del equipo %d es de %d\n",dataEquipo->id_equipo,CH);
			for(i = 0;i<CH;i++) id[i] = i;
			for(i=0;i<CH;i++){
				t_data* concursante = malloc(sizeof(t_data));
				concursante->id_equipo = dataEquipo->id_equipo;
				concursante->id_hebra = id[i];
				concursante->lista = NULL;
				concursante->inicio = 0;
				concursante->fin = 0;
				concursante->listaGeneral = S;
				concursante->interseccionLocal = S2;
				concursante->cantidadDeHebras = dataEquipo->cantidadHebras;
				concursante->posResultados = concursante->id_equipo*hi+concursante->id_hebra;
				pthread_create(&arr_Concursantes[i],NULL,&concursantes,(void *)concursante);
			}

	for(i=0;i<CH;i++){
		pthread_join(arr_Concursantes[i], NULL);
	}
	int fin = clock();
	resultadosTiempo[dataEquipo->id_equipo][0] = (fin-start)/CLOCK_PER_SECOND;
	pthread_exit(NULL);
	}

void *concursantes(void* tData){

	t_data *concursante = (t_data*) tData;
	//pthread_mutex_lock(&lock2);
	int i,j;
			float start = clock();
	for(i=1;i<cantListas;i++){
			int cantidadElementos = listas[i][0];
			int	companeros = concursante->cantidadDeHebras;
			//Se maneja el caso de lista no divisible
			int porcion = cantidadElementos/companeros;
			int resto = cantidadElementos%companeros;
			int inicio = 0;
			int final = 0;
			if(resto != 0 && concursante->id_hebra == companeros-1){
				inicio = (concursante->id_hebra)*porcion+1;
				final = (concursante->id_hebra+1)*porcion+resto;
			} else{
				inicio = (concursante->id_hebra)*porcion+1;
				final = (concursante->id_hebra+1)*porcion;
			}
			pthread_mutex_lock(&lock3);
					int * aux = intersectar(concursante->listaGeneral,listas[i],inicio,final);
					if(aux[0]!=0){
						for(j = 0; j<=aux[0];j++){
							if(j!=0){
								concursante->interseccionLocal[ult[concursante->id_equipo]] = aux[j];
								ult[concursante->id_equipo] = ult[concursante->id_equipo]+1;
								}
						}

						quickSort(concursante->interseccionLocal+1,ult[concursante->id_equipo]-1);
						concursante->interseccionLocal[0] = ult[concursante->id_equipo]-1;
						concursante->interseccionLocal = realloc(concursante->interseccionLocal,concursante->interseccionLocal[0]+1);
					}
					pthread_mutex_unlock(&lock3);

					pthread_barrier_wait(&barreras1[concursante->id_equipo]);

					if(concursante->id_hebra==0){
						printf("Soy la hebra 0 del equipo %d y mostraré los resutados de la iteración %d \n",concursante->id_equipo,i);
						for(j=0;j<=concursante->interseccionLocal[0];j++) concursante->listaGeneral[j] = concursante->interseccionLocal[j];
						concursante->listaGeneral = realloc(concursante->listaGeneral,concursante->listaGeneral[0]+1);
						ult[concursante->id_equipo] = 1;
						for(j=0;j<=concursante->interseccionLocal[0];j++){
							printf("S[%d] = %d\n",j,concursante->listaGeneral[j] );
						}
					}
					pthread_barrier_wait(&barreras2[concursante->id_equipo]);

					float end = clock();
					float total = (end-start)/CLOCK_PER_SECOND;
					resultadosTiempoHebras[concursante->posResultados][1] = concursante->id_hebra;
					resultadosTiempoHebras[concursante->posResultados][2]  = concursante->id_equipo;
					resultadosPromedioHebras[concursante->posResultados][1] = concursante->id_hebra;
					resultadosPromedioHebras[concursante->posResultados][2]  = concursante->id_equipo;
					resultadosPromedioHebras[concursante->posResultados][0] = resultadosPromedioHebras[concursante->posResultados][0]+total;
					resultadosPromedioHebras[concursante->posResultados][3]++;

					if(resultadosTiempoHebras[concursante->posResultados][0]!=0){
						if(resultadosTiempoHebras[concursante->posResultados][0]>total) resultadosTiempoHebras[concursante->posResultados][0]=total;
					}else{
						resultadosTiempoHebras[concursante->posResultados][0]=total;
					}
	}
	//pthread_mutex_unlock(&lock2);
	pthread_exit(NULL);
}

void escribirResultados(float **resultados,float **individual,float **promedio, int ** inter){
	FILE* resultado = fopen("resultado.txt","w");
	if(cantListas!=1){
		if(ei >=3 ){
			quickSortMatFloat(resultados,ei);
			quickSortMatFloat(individual,ei*hi);
			quickSortMatFloat(promedio,ei*hi);
			fprintf(resultado, "Número del equipo que obtuvo el primer lugar: %g\n", resultados[0][1]);
			fprintf(resultado, "Tiempo del equipo que obtuvo el primer lugar: %f\n", resultados[0][0]);
			fprintf(resultado, "Número del equipo que obtuvo el segundo lugar: %g\n", resultados[1][1]);
			fprintf(resultado, "Tiempo del equipo que obtuvo el segundo lugar: %f\n", resultados[1][0]);
			fprintf(resultado, "Número del equipo que obtuvo el tercer lugar: %g\n", resultados[2][1]);
			fprintf(resultado, "Tiempo del equipo que obtuvo el tercer lugar: %f\n", resultados[2][0]);
		} else if(2<=ei && ei<3){
			fprintf(resultado, "Número del equipo que obtuvo el primer lugar: %g\n", resultados[0][1]);
			fprintf(resultado, "Tiempo del equipo que obtuvo el primer lugar: %f\n", resultados[0][0]);
			fprintf(resultado, "Número del equipo que obtuvo el segundo lugar: %g\n", resultados[1][1]);
			fprintf(resultado, "Tiempo del equipo que obtuvo el segundo lugar: %f\n", resultados[1][0]);
		} else if(ei<2){
			fprintf(resultado, "Número del equipo que obtuvo el primer lugar: %g\n", resultados[0][1]);
			fprintf(resultado, "Tiempo del equipo que obtuvo el primer lugar: %f\n", resultados[0][0]);
		}
		fprintf(resultado, "Hebra más eficiente: Hebra_%g-Equipo_%g\n",individual[0][1], individual[0][2]);
		int i,j;
		//for(i=0;i<ei*hi;i++)
		fprintf(resultado, "Hebra más eficiente en promedio: Hebra_%g-Equipo_%g\n",promedio[0][1], promedio[0][2]);

	 	fprintf(resultado,"Intersección de las listas:");
	 	for(i=0;i<ei;i++){
			fprintf(resultado, " Equipo %d->",i);

			for(j=1;j<=inter[i][0];j++){
				if(inter[i][0]==0)	printf("NO hay nada \n");
				fprintf(resultado, " %d",inter[i][j]);
			}
		}
	} else{
		printf("\n******************\nIntersección vacío. Ingrese más de un lista\n*********************\n");
	}
	fclose(resultado);
}
