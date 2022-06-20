#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define BLANCO 0///PODADO
#define AZUL 1///TRATAMIENTO
#define NARANJA 2 ///ENFERMO SIN SINTOMAS
#define ROJO 3 ///ENFERMO CON SINTOMAS
#define VERDE 4 ///SANO

int ciclo=0,N;

typedef struct  {
    int estado; /// b= blanco, a=azul, r= rojo, n= naranja, v=verde
    int edad ;/// 1 joven, 2 adulto y 3 viejo;
    int herida;
    int tiempo;

} Arbol;

void reAsignarEdad(Arbol *mat, int posicion){
    int randoH;
    #pragma omp critical
    {
    if(mat[posicion].edad>= 52 && mat[posicion].edad <=156){
                randoH= rand()%100;
                if(randoH <= 23){
                    mat[posicion].herida= 1;/// Arbol con herida abierta
                }
                else{
                    mat[posicion].herida= 0; /// Arbol sin herida abierta
                }

    }
    else{
        if(mat[posicion].edad>=157 && mat[posicion].edad <=1820){
            randoH= rand()%100;
            if(randoH <= 8){
                mat[posicion].herida= 1;/// Arbol con herida abierta
            }
            else{
                mat[posicion].herida= 0; /// Arbol sin herida abierta
            }
        }
         else{
            if(mat[posicion].edad>= 1821){
                randoH= rand()%100;
                if(randoH <= 37){
                    mat[posicion].herida= 1;/// Arbol con herida abierta
                }
                else{
                    mat[posicion].herida= 0; /// Arbol sin herida abierta
                }
            }

        }
    }
    }

}

///Funciones de cambio de estado
void fArbolSano(Arbol *mat, int posicion, int arbolCuenta[], Arbol *matrizAux){
    int porcArbolRojo=0;
    int probContangio, susc;
    float randoP;
    #pragma omp critical
    {
    porcArbolRojo= arbolCuenta[0]/arbolCuenta[1];

    ///susceptibilidad
    if(mat[posicion].herida == 1){
        if(mat[posicion].edad>= 52 && mat[posicion].edad <=156){
            susc= 0.35+ 0.15;
        }
        else{
            if(mat[posicion].edad>=157 && mat[posicion].edad <=1820){
                susc= 0.17+ 0.15;
            }
            else{
                if(mat[posicion].edad>= 1821){
                    susc= 0.63 + 0.15;
                }
            }
        }
    }
    else{
        if(mat[posicion].edad>= 52 && mat[posicion].edad <=156){
            susc= 0.35;
        }
        else{
            if(mat[posicion].edad>=157 && mat[posicion].edad <=1820){
                susc= 0.17;
            }
            else{
                if(mat[posicion].edad>= 1821){
                    susc= 0.63;
                }
            }
        }
    }
    probContangio= ((porcArbolRojo * susc)*0,60)+0,7;

    randoP= ( rand( ) % 1001 ) / 1000.0f ;

    if( randoP<= probContangio){
        matrizAux[posicion].estado= NARANJA;
        matrizAux[posicion].edad= mat[posicion].edad;
        matrizAux[posicion].herida= mat[posicion].herida;
        matrizAux[posicion].tiempo= 0;
    }
    else{
        matrizAux[posicion].estado= VERDE;
        matrizAux[posicion].edad= mat[posicion].edad;
        matrizAux[posicion].herida= mat[posicion].herida;
        matrizAux[posicion].tiempo= mat[posicion].tiempo;
    }
    }

}

///------------------------------------------------------------
void sumaArbol(Arbol *mat, int i, int j, int arbolCuenta[]){
    #pragma omp critical
    {
    if(mat[j*N+i].estado == ROJO){
        arbolCuenta[0]= arbolCuenta[0]+1;
        arbolCuenta[1]= arbolCuenta[1]+1;
    }
    else{
        arbolCuenta[1]= arbolCuenta[1]+1;
    }
    }

}


void imprimirMatriz(Arbol *mat){
    int i,j;
    for(j=0;j<N;j++){
        #pragma omp parallel for schedule(dynamic,25) private (i) 
        for(i=0;i<N;i++){
            if(i==0){
              printf("[");
            }


            printf("%d  ",mat[j*N+i].estado);
            if(i==N-1){
                printf("]\n");
            }
            else{
                printf(" ");
            }
         }
    }

}
void main(){

    int i,j,randoE,randoEdad, ejecuciones=1000,posicion;
    clock_t  tiempo_inicio, tiempo_final, tiempoTotal;
    double segundos;
    int randoH;
    int cantEjecuciones;
    N=1500;
    Arbol *mat= NULL;
    mat= (Arbol *)malloc(N*N*sizeof(Arbol));
    Arbol *matrizAux= NULL;
    Arbol *aux= NULL;
    matrizAux= (Arbol *)malloc(N*N*sizeof(Arbol));
    int arbolCuenta[2];/// Pos 0= blanco, 1= azul, 2= rojo, 3= naranja, 4= verde, 5= tiene el total de vecinos
    int probContangio;
    //printf("Mostrar matriz:\n");
    for(cantEjecuciones=0; cantEjecuciones<5; cantEjecuciones++){
    tiempo_inicio = clock();
    // asigno valores
    for(j=0;j<N;j++){
        #pragma omp parallel for schedule(dynamic,25) private (i,randoE,randoEdad,randoH,posicion) shared(mat)
        for(i=0;i<N;i++){
            /*if(i==0){
              printf("[");
            }*/
            #pragma omp critical
            {
                posicion= j*N+i;
                //printf("posicion %d\n",posicion);
                randoE= rand() % 100;
                //asignarE(mat,i,j, N,randoE);
                ///Asigno estado
                if(randoE<= 65){
                    mat[posicion].estado= VERDE;
                    mat[posicion].tiempo=0;
                }
                else{
                    if(randoE>65 && randoE<=70){
                        mat[posicion].estado=ROJO;
                    }
                     else{
                        if(randoE>70 && randoE<=80){

                            mat[posicion].estado=NARANJA;
                        }
                        else{
                            if(randoE>80 && randoE<=100){

                                mat[posicion].estado=AZUL;
                            }
                        }
                    }
                }
                randoEdad= rand() % 100;
                //asignarEdad(mat, i, j, N, randoEdad);
                ///Asigno edad y herida
                if(randoEdad<= 30){
                            mat[posicion].edad= 52;/// Arbol Joven de un ano
                            randoH= rand()%100;
                            if(randoH <= 23){
                                mat[posicion].herida= 1;/// Arbol con herida abierta
                            }
                            else{
                                mat[posicion].herida= 0; /// Arbol sin herida abierta
                            }

                }
                else{
                    if(randoEdad>30 && randoEdad<=80){
                        mat[posicion].edad= 157;/// Arbol Adulto de 3 ano y 1 semana
                        randoH= rand()%100;
                        if(randoH <= 8){
                            mat[posicion].herida= 1;/// Arbol con herida abierta
                        }
                        else{
                            mat[posicion].herida= 0; /// Arbol sin herida abierta
                        }
                    }
                     else{
                        if(randoEdad>80 && randoEdad<=100){
                            mat[posicion].edad= 1821; ///Arbol Viejo con 35 anos y 1 semana
                            randoH= rand()%100;
                            if(randoH <= 37){
                                mat[posicion].herida= 1;/// Arbol con herida abierta
                            }
                            else{
                                mat[posicion].herida= 0; /// Arbol sin herida abierta
                            }
                        }

                    }
                }
            }
            /*printf("%d  ",mat[posicion].estado);
            if(i==N-1){
                printf("]\n");
            }
            else{
                printf(" ");
            }*/
         }

    }
    //-------------------------------------------------------------------------------------------------------------------------
    ///Ejecucion de reglas
    /*printf("Ingresa la cantidad de semana para simular (puede ser entre 240 hasta 4320)\n");
    scanf("%d",&ejecuciones);*/


    for(ciclo=0; ciclo<ejecuciones; ciclo++){

        //checkMatriz(mat);

        //int arbolCuenta[6];
        arbolCuenta[0]= 0;
        arbolCuenta[1]= 0;


        for(j=0;j<N;j++){
            #pragma omp  for schedule(dynamic,25) private ( arbolCuenta, i, posicion, randoE) 
            for(i=0;i<N;i++){
                #pragma omp critical
                {
                posicion= j*N+i;
                }
               ///calculos vecinos
               ///casos de eje horizontal
                if(!((i-2)<0)){
                    sumaArbol(mat,i-2,j, arbolCuenta);
                }

                if(!((i+2)>=N)){
                    sumaArbol(mat,i+2,j, arbolCuenta);
                }

                if(!((i-1)<0)){
                    sumaArbol(mat,i-1,j, arbolCuenta);
                }

                if(!((i+1)>=N)){
                    sumaArbol(mat,i+1,j, arbolCuenta);
                }
                ///////////////////////////////////////////
                ///Casos de los eje vertical
                if(!((j-2)<0)){
                    sumaArbol(mat,i,j-2, arbolCuenta);
                }

                if(!((j+2)>=N)){
                    sumaArbol(mat,i,j+2, arbolCuenta);
                }

                if(!((j-1)<0) ){
                    sumaArbol(mat,i,j-1, arbolCuenta);
                }

                if(!(j+1>=N)){
                    sumaArbol(mat,i,j+1, arbolCuenta);
                }
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ///Casos de los costados
                if( (i-1 >= 0 && j-1 >= 0)){
                    sumaArbol(mat,i-1,j-1, arbolCuenta);
                }

                if( (i+1<N && j-1 >=0)){
                    sumaArbol(mat,i+1,j-1, arbolCuenta);
                }

                if( (i-1 >=0 && j+1 < N)){
                    sumaArbol(mat,i-1,j+1, arbolCuenta);
                }

                if( (i+1 < N && j+1 < N)){
                    sumaArbol(mat,i+1,j+1, arbolCuenta);
                }
                ///fin de calcular vecinos/////////////////////////////////////////////////////////////////////////////////////
                ///Comienzo de consulta de estado
               if(mat[posicion].estado==VERDE){
                    //printf("aqui0\n");
                    reAsignarEdad(mat,posicion);
                    fArbolSano(mat,posicion, arbolCuenta,matrizAux);
               }
               else{
                    #pragma omp critical
                    {
                    if(mat[posicion].estado== NARANJA){
                        //printf("aqui1\n");
                        ///funcion de arbol enfermo sin sintomas
                        //fArbolEnfermoS(mat,i,j,arbolCuenta,matrizAux);
                        if(mat[posicion].tiempo == 3){
                            matrizAux[posicion].estado=ROJO;
                            matrizAux[posicion].edad= mat[posicion].edad;
                            matrizAux[posicion].herida= matrizAux[posicion].herida;
                            matrizAux[posicion].tiempo= mat[posicion].tiempo;
                        }
                        else{
                            matrizAux[posicion].estado=NARANJA;
                            matrizAux[posicion].edad= mat[posicion].edad;
                            matrizAux[posicion].herida= mat[posicion].herida;
                            matrizAux[posicion].tiempo= mat[posicion].tiempo+1;
                        }
                    }
                    else{
                        if(mat[posicion].estado==ROJO){
                            ///printf("aqui2\n");
                            ///funcion de arbol enfermo con sintomas
                            //fArbolEnfermo(mat,i,j,arbolCuenta,matrizAux);

                            randoE= rand() % 100;
                            if(randoE <= 85){
                                 matrizAux[posicion].estado=AZUL;
                                 matrizAux[posicion].edad= mat[posicion].edad;
                                 matrizAux[posicion].herida= mat[posicion].herida;
                                 matrizAux[posicion].tiempo= 0;
                                 //matrizAux[i*N+j].flagTiempo= 1;
                            }
                            else{

                                matrizAux[posicion].estado=mat[posicion].estado;
                                matrizAux[posicion].edad= mat[posicion].edad;
                                matrizAux[posicion].herida= mat[posicion].herida;
                                matrizAux[posicion].tiempo= mat[posicion].tiempo + 1;
                            }
                        }
                        else{
                            if(mat[posicion].estado==AZUL){
                                ///funcion de arbol con tratamiento
                                if(mat[posicion].tiempo >= 5){
                                    randoE= rand()%100;
                                    //printf("aca tiempo dentro de 8\n");
                                    if((mat[posicion].edad >= 52 && mat[posicion].edad <=156)){
                                        if(randoE<= 3){
                                            ///aca iria recuperado
                                            matrizAux[posicion].estado=VERDE;
                                            matrizAux[posicion].edad= mat[posicion].edad;
                                            matrizAux[posicion].herida= 0;
                                            matrizAux[posicion].tiempo= 0;
                                        }
                                        else{
                                            ///aca iria podado
                                            matrizAux[posicion].estado==BLANCO;
                                            matrizAux[posicion].edad= mat[posicion].edad;
                                            matrizAux[posicion].herida= 0;
                                            matrizAux[posicion].tiempo= 0;
                                        }

                                    }



                                    if((mat[posicion].edad >= 157 && mat[posicion].edad <=1820)){/// arbol adulto
                                        if(randoE <= 15){
                                            ///aca iria recuperado
                                            matrizAux[posicion].estado=VERDE;
                                            matrizAux[posicion].edad= mat[posicion].edad;
                                            matrizAux[posicion].herida= 0;
                                            matrizAux[posicion].tiempo= 0;
                                        }
                                        else{
                                            ///aca iria podado
                                            matrizAux[posicion].estado=BLANCO;
                                            matrizAux[posicion].edad= mat[posicion].edad;
                                            matrizAux[posicion].herida= 0;
                                            matrizAux[posicion].tiempo= 0;
                                        }

                                    }


                                    if(mat[posicion].edad >=1821){ ///arbol viejo
                                        if(randoE <= 53){
                                            ///aca iria recuperado
                                            matrizAux[posicion].estado= VERDE;
                                            matrizAux[posicion].edad= mat[posicion].edad;
                                            matrizAux[posicion].herida= 0;
                                            matrizAux[posicion].tiempo= 0;
                                        }
                                        else{
                                            ///aca iria reemplazado por un arbol joven
                                            matrizAux[posicion].estado= VERDE;
                                            matrizAux[posicion].edad= 1;
                                            matrizAux[posicion].herida= 0;
                                            matrizAux[posicion].tiempo= 0;
                                        }

                                    }

                                }
                                else{

                                    matrizAux[posicion].estado=mat[posicion].estado;
                                    matrizAux[posicion].edad= mat[posicion].edad;
                                    matrizAux[posicion].herida= mat[posicion].herida;
                                    matrizAux[posicion].tiempo= mat[posicion].tiempo + 1;
                                }
                            }
                            else{
                                if(mat[posicion].estado== BLANCO){
                                    //printf("aqui4\n");
                                    ///funcion de arbol podado
                                    //fArbolPodado(mat,i,j,arbolCuenta,matrizAux);
                                    if(mat[posicion].tiempo == 12){

                                        matrizAux[posicion].estado= VERDE;
                                        matrizAux[posicion].edad= mat[posicion].edad;
                                        matrizAux[posicion].herida= 0;
                                        matrizAux[posicion].tiempo= 0;
                                    }
                                    else{

                                        matrizAux[posicion].estado= BLANCO;
                                        matrizAux[posicion].edad= mat[posicion].edad;
                                        matrizAux[posicion].herida= mat[posicion].herida;
                                        matrizAux[posicion].tiempo= mat[posicion].tiempo+1;
                                    }
                                }
                            }
                        }
                    }
                    }///fin de critical
               }///fin de else
               #pragma omp critical
               matrizAux[posicion].edad= matrizAux[posicion].edad+1;
                //}
             }
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///actualizar matriz
        for(j=0;j<N;j++){
            #pragma omp  for schedule(dynamic,25)  private (i,posicion) 
            for(i=0;i<N;i++){
                #pragma omp critical
                {
                posicion= j*N+i;
                mat[posicion].estado= matrizAux[posicion].estado;
                mat[posicion].edad= matrizAux[posicion].edad;
                mat[posicion].herida= matrizAux[posicion].herida;
                mat[posicion].tiempo= matrizAux[posicion].tiempo;
                }
            }
        }
        //printf("Muestro matriz en semana %d\n",ciclo+1);
        //imprimirMatriz(mat);
    }
    tiempo_final = clock();

    segundos = (double)(tiempo_final - tiempo_inicio) / CLOCKS_PER_SEC; /*según que estes midiendo el tiempo en segundos es demasiado grande*/

    printf("Tiempo de ejecucion de una vuelta: %f\n",segundos);
    tiempoTotal += (tiempo_final-tiempo_inicio);

    }
    printf("el tiempo promedio de las ejecuciones fue: %f\n",(double)(tiempoTotal/5)/CLOCKS_PER_SEC);
    printf("Con una matriz de %dx%d\n",N,N);
    printf("Con semana: %d\n",ejecuciones);
    printf("Y 5 ejecuciones\n");
    free(mat);
    free(matrizAux);

}
