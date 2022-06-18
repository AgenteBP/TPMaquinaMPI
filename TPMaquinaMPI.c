#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define BLANCO 0///PODADO
#define AZUL 1///TRATAMIENTO
#define NARANJA 2 ///ENFERMO SIN SINTOMAS
#define ROJO 3 ///ENFERMO CON SINTOMAS
#define VERDE 4 ///SANO
#define N 1500

int tamG;

typedef struct  {
    int estado; /// b= blanco, a=azul, r= rojo, n= naranja, v=verde
    int edad ;/// 1 joven, 2 adulto y 3 viejo;
    int herida;
    int tiempo;

} Arbol;

void reAsignarEdad(Arbol *matrizAux, int posicion){
    int randoH;
    if(matrizAux[posicion].edad>= 52 && matrizAux[posicion].edad <=156){
                randoH= rand()%100;
                if(randoH <= 23){
                    matrizAux[posicion].herida= 1;/// Arbol con herida abierta
                }
                else{
                    matrizAux[posicion].herida= 0; /// Arbol sin herida abierta
                }

    }
    else{
        if(matrizAux[posicion].edad>=157 && matrizAux[posicion].edad <=1820){
            randoH= rand()%100;
            if(randoH <= 8){
                matrizAux[posicion].herida= 1;/// Arbol con herida abierta
            }
            else{
                matrizAux[posicion].herida= 0; /// Arbol sin herida abierta
            }
        }
         else{
            if(matrizAux[posicion].edad>= 1821){
                randoH= rand()%100;
                if(randoH <= 37){
                    matrizAux[posicion].herida= 1;/// Arbol con herida abierta
                }
                else{
                    matrizAux[posicion].herida= 0; /// Arbol sin herida abierta
                }
            }

        }
    }

}

///Funciones de cambio de estado
void fArbolSano(Arbol *matrizAux, int posicion, int arbolCuenta[], Arbol *aux){
    int porcArbolRojo=0;
    int probContangio, susc;
    float randoP;

    porcArbolRojo= arbolCuenta[0]/arbolCuenta[1];

    ///susceptibilidad
    if(matrizAux[posicion].herida == 1){
        if(matrizAux[posicion].edad>= 52 && matrizAux[posicion].edad <=156){
            susc= 0.35+ 0.15;
        }
        else{
            if(matrizAux[posicion].edad>=157 && matrizAux[posicion].edad <=1820){
                susc= 0.17+ 0.15;
            }
            else{
                if(matrizAux[posicion].edad>= 1821){
                    susc= 0.63 + 0.15;
                }
            }
        }
    }
    else{
        if(matrizAux[posicion].edad>= 52 && matrizAux[posicion].edad <=156){
            susc= 0.35;
        }
        else{
            if(matrizAux[posicion].edad>=157 && matrizAux[posicion].edad <=1820){
                susc= 0.17;
            }
            else{
                if(matrizAux[posicion].edad>= 1821){
                    susc= 0.63;
                }
            }
        }
    }
    probContangio= ((porcArbolRojo * susc)*0,60)+0,7;

    randoP= ( rand( ) % 1001 ) / 1000.0f ;

    if( randoP<= probContangio){
        aux[posicion].estado= NARANJA;
        aux[posicion].edad= matrizAux[posicion].edad;
        aux[posicion].herida= matrizAux[posicion].herida;
        aux[posicion].tiempo= 0;
    }
    else{
        aux[posicion].estado= VERDE;
        aux[posicion].edad= matrizAux[posicion].edad;
        aux[posicion].herida= matrizAux[posicion].herida;
        aux[posicion].tiempo= matrizAux[posicion].tiempo;
    }

}

///------------------------------------------------------------
void sumaArbol(Arbol *matriz, int i, int j, int arbolCuenta[]){

    if(matriz[j*N+i].estado == ROJO){
        arbolCuenta[0]= arbolCuenta[0]+1;
        arbolCuenta[1]= arbolCuenta[1]+1;
    }
    else{
        arbolCuenta[1]= arbolCuenta[1]+1;
    }

}


void imprimirMatriz(Arbol *mat){
    int i,j;
    for(j=0;j<N;j++){
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
void vecinoProceso0(Arbol *matrizAux, Arbol *matrizAuxP, int arbolCuenta[], int i, int j){
    ///casos de eje horizontal
    if(!((i-2)<0)){
        sumaArbol(matrizAux,i-2,j, arbolCuenta);
    }

    if(!((i+2)>=N)){
        sumaArbol(matrizAux,i+2,j, arbolCuenta);
    }

    if(!((i-1)<0)){
        sumaArbol(matrizAux,i-1,j, arbolCuenta);
    }

    if(!((i+1)>=N)){
        sumaArbol(matrizAux,i+1,j, arbolCuenta);
    }
    ///////////////////////////////////////////
    ///Casos de los eje vertical
    if(!((j-2)<0)){
        sumaArbol(matrizAux,i,j-2, arbolCuenta);
    }

    if(((j+2)>=N)){
        //printf("la matriz en posicion i: %d y j+2: %d da %d\n",i,j,matrizAuxP[1*N+i].estado);
        if(j+1<N){
        sumaArbol(matrizAuxP,i,0, arbolCuenta);}///caso q tiene buscar al vecino
        else {sumaArbol(matrizAuxP,i,1, arbolCuenta);}///caso q tiene buscar al vecino
    }

    if(!((j-1)<0) ){
        sumaArbol(matrizAux,i,j-1, arbolCuenta);
    }

    if(!(j+1>=N)){
        sumaArbol(matrizAux,i,j+1, arbolCuenta);///caso q tiene buscar al vecino
    }
    else{
        //printf("la matriz en posicion i: %d y j+1: %d da %d\n",i,j,matrizAuxP[0*N+i].estado);
        sumaArbol(matrizAuxP,i,0, arbolCuenta);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///Casos de los costados
    if( (i-1 >= 0 && j-1 >= 0)){
        sumaArbol(matrizAux,i-1,j-1, arbolCuenta);
    }

    if( (i+1<N && j-1 >=0)){
        sumaArbol(matrizAux,i+1,j-1, arbolCuenta);
    }

    if( (i-1 >=0 && j+1 < N)){
        sumaArbol(matrizAux,i-1,j+1, arbolCuenta);
    }
    else{
        if((i-1 >=0 && j+1 >=N))///caso q tiene buscar al vecino
        {//printf("la matriz en posicion i-1: %d y j+1: %d da %d\n",i,j,matrizAuxP[0*N+(i-1)].estado);
        sumaArbol(matrizAuxP,i-1,0, arbolCuenta);}
    }

    if( (i+1 < N && j+1 < N)){
        sumaArbol(matrizAux,i+1,j+1, arbolCuenta);
    }
    else{
        if((i+1 < N  && j+1 >=N))///caso q tiene buscar al vecino
        {//printf("la matriz en posicion i+1: %d y j+1: %d da %d\n",i,j,matrizAuxP[0*N+(i+1)].estado);
        sumaArbol(matrizAuxP,i+1,0, arbolCuenta);}
    }
}
void vecinoNormal(Arbol *matrizAux, int arbolCuenta[], int i, int j){
    ///casos de eje horizontal
    if(!((i-2)<0)){
        sumaArbol(matrizAux,i-2,j, arbolCuenta);
    }

    if(!((i+2)>=N)){
        sumaArbol(matrizAux,i+2,j, arbolCuenta);
    }

    if(!((i-1)<0)){
        sumaArbol(matrizAux,i-1,j, arbolCuenta);
    }

    if(!((i+1)>=N)){
        sumaArbol(matrizAux,i+1,j, arbolCuenta);
    }
    ///////////////////////////////////////////
    ///Casos de los eje vertical
    if(!((j-2)<0)){
        sumaArbol(matrizAux,i,j-2, arbolCuenta);
    }

    if(!((j+2)>=N)){
        sumaArbol(matrizAux,i,j+2, arbolCuenta);
    }

    if(!((j-1)<0) ){
        sumaArbol(matrizAux,i,j-1, arbolCuenta);
    }

    if(!(j+1>=N)){
        sumaArbol(matrizAux,i,j+1, arbolCuenta);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///Casos de los costados
    if( (i-1 >= 0 && j-1 >= 0)){
        sumaArbol(matrizAux,i-1,j-1, arbolCuenta);
    }

    if( (i+1<N && j-1 >=0)){
        sumaArbol(matrizAux,i+1,j-1, arbolCuenta);
    }

    if( (i-1 >=0 && j+1 < N)){
        sumaArbol(matrizAux,i-1,j+1, arbolCuenta);
    }

    if( (i+1 < N && j+1 < N)){
        sumaArbol(matrizAux,i+1,j+1, arbolCuenta);
    }
}
void vecinoProcesoUlt(Arbol *matrizAux, Arbol *matrizAuxP, int arbolCuenta[], int i, int j){
    ///casos de eje horizontal
    if(!((i-2)<0)){
        sumaArbol(matrizAux,i-2,j, arbolCuenta);
    }

    if(!((i+2)>=N)){
        sumaArbol(matrizAux,i+2,j, arbolCuenta);
    }

    if(!((i-1)<0)){
        sumaArbol(matrizAux,i-1,j, arbolCuenta);
    }

    if(!((i+1)>=N)){
        sumaArbol(matrizAux,i+1,j, arbolCuenta);
    }
    ///////////////////////////////////////////
    ///Casos de los eje vertical
    if(((j-2)<0)){
        if(j-1>=0)
        sumaArbol(matrizAuxP,i,1, arbolCuenta);
        else{sumaArbol(matrizAuxP,i,0, arbolCuenta);}
    }

    if(!((j+2)>=N)){
        sumaArbol(matrizAux,i,j+2, arbolCuenta);
    }

    if(!((j-1)<0) ){
        sumaArbol(matrizAux,i,j-1, arbolCuenta);
    }
    else{
        sumaArbol(matrizAuxP,i,1, arbolCuenta);
    }

    if(!(j+1>=N)){
        sumaArbol(matrizAux,i,j+1, arbolCuenta);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///Casos de los costados
    if( (i-1 >= 0 && j-1 >= 0)){
        sumaArbol(matrizAux,i-1,j-1, arbolCuenta);
    }
    else{
        if((i-1 >= 0 && j-1 < 0))
            sumaArbol(matrizAuxP,i-1,1, arbolCuenta);
    }

    if( (i+1<N && j-1 >=0)){
        sumaArbol(matrizAux,i+1,j-1, arbolCuenta);
    }else{
        if((i+1<N  && j-1 < 0))
            sumaArbol(matrizAuxP,i+1,1, arbolCuenta);
    }

    if( (i-1 >=0 && j+1 < N)){
        sumaArbol(matrizAux,i-1,j+1, arbolCuenta);
    }

    if( (i+1 < N && j+1 < N)){
        sumaArbol(matrizAux,i+1,j+1, arbolCuenta);
    }
}

void vecinoProcesoGeneral(Arbol *matrizAux, Arbol *matrizAuxP, Arbol *matrizAuxP2, int arbolCuenta[], int i, int j){
    ///matrizAuxP es enviando por el proceso anterior y matrizAuxP2 por el posterior
    ///casos de eje horizontal
    if(!((i-2)<0)){
        sumaArbol(matrizAux,i-2,j, arbolCuenta);
    }

    if(!((i+2)>=N)){
        sumaArbol(matrizAux,i+2,j, arbolCuenta);
    }

    if(!((i-1)<0)){
        sumaArbol(matrizAux,i-1,j, arbolCuenta);
    }

    if(!((i+1)>=N)){
        sumaArbol(matrizAux,i+1,j, arbolCuenta);
    }
    ///////////////////////////////////////////
    ///Casos de los eje vertical
    if(((j-2)<0)){
        if(j-1>=0){
        sumaArbol(matrizAuxP,i,1, arbolCuenta);}
        else{sumaArbol(matrizAuxP,i,0, arbolCuenta);}
    }

    if(((j+2)>=N)){
        if(j+1<N){
        sumaArbol(matrizAuxP2,i,0, arbolCuenta);}///caso q tiene buscar al vecino
        else{sumaArbol(matrizAuxP2,i,1, arbolCuenta);}
    }

    if(!((j-1)<0) ){
        sumaArbol(matrizAux,i,j-1, arbolCuenta);
    }
    else{
        sumaArbol(matrizAuxP,i,1, arbolCuenta);
    }

    if(!(j+1>=N)){
        sumaArbol(matrizAux,i,j+1, arbolCuenta);///caso q tiene buscar al vecino
    }
    else{
        sumaArbol(matrizAuxP2,i,0, arbolCuenta);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///Casos de los costados
    if( (i-1 >= 0 && j-1 >= 0)){
        sumaArbol(matrizAux,i-1,j-1, arbolCuenta);
    }
    else{
        if((i-1 >= 0 && j-1 < 0))
            sumaArbol(matrizAuxP,i-1,1, arbolCuenta);
    }

    if( (i+1<N && j-1 >=0)){
        sumaArbol(matrizAux,i+1,j-1, arbolCuenta);
    }else{
        if((i+1<N  && j-1 < 0))
            sumaArbol(matrizAuxP,i+1,1, arbolCuenta);
    }
    //-------------------------------------------------------------------------------------------------

    if( (i-1 >=0 && j+1 < N)){
        sumaArbol(matrizAux,i-1,j+1, arbolCuenta);
    }
    else{
        if((i-1 >=0 && j+1 >=N))///caso q tiene buscar al vecino
        sumaArbol(matrizAuxP2,i-1,0, arbolCuenta);
    }

    if( (i+1 < N && j+1 < N)){
        sumaArbol(matrizAux,i+1,j+1, arbolCuenta);
    }
    else{
        if((i+1 < N  && j+1 >=N))///caso q tiene buscar al vecino
        sumaArbol(matrizAuxP2,i+1,0, arbolCuenta);
    }
}

void pasadorUltFilas(Arbol *matrizAux, Arbol *matrizAuxP){
    int i,j,k=0,pos, pos2;
    for(j=tamG-2;j<tamG;j++){
        for(i=0;i<N;i++){
            pos=j*N+i;
            pos2=k*N+i;
            matrizAuxP[pos2].estado=matrizAux[pos].estado;
            matrizAuxP[pos2].edad=matrizAux[pos].edad;
            matrizAuxP[pos2].herida=matrizAux[pos].herida;
            matrizAuxP[pos2].tiempo=matrizAux[pos].tiempo;
        }
        k++;
    }
   

}

void pasadorPrimerasFilas(Arbol *matrizAux, Arbol *matrizAuxP){
    int i,j,k=0,pos, pos2;
    for(j=0;j<2;j++){
        for(i=0;i<N;i++){
            pos=j*N+i;
            pos2=k*N+i;
            matrizAuxP[pos2].estado=matrizAux[pos].estado;
            matrizAuxP[pos2].edad=matrizAux[pos].edad;
            matrizAuxP[pos2].herida=matrizAux[pos].herida;
            matrizAuxP[pos2].tiempo=matrizAux[pos].tiempo;
        }
        k++;
    }

}

void main(int argc, char *argv[]){
    ///MPI
    float tiempofinal, tiempoTotal;
    double startwtime = 0.0, endwtime; // Variables de tiempo
    MPI_Init(&argc, &argv);
    MPI_Status status; //Para usar en el "recibir" del mensaje y en el test
    MPI_Request request; //Para usar en el MPI_Test
    int comm_sz;// Número de procesos
    int my_rank;// Número de este proceso
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);// Obtener la cantidad de procesos
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);// Obtener el número de proceso

    int i,j,randoE,randoEdad, ejecuciones=1000,posicion;
    double segundos;
    int randoH;
    int cantEjecuciones;
    int tam;
    int ciclo=0;//,N
    //N=16;
    tam= N/comm_sz;
    tamG= tam;
    Arbol *mat= NULL;
    Arbol *matrizAux= NULL;
    Arbol *matrizAuxP= NULL;
    Arbol *matrizAuxP2= NULL;
    Arbol *aux= NULL;
    matrizAux= (Arbol *)malloc(N*tam*sizeof(Arbol));
    matrizAuxP= (Arbol *)malloc(N*2*sizeof(Arbol));
    aux= (Arbol *)malloc(N*tam*sizeof(Arbol));
    
    matrizAuxP2= (Arbol *)malloc(N*2*sizeof(Arbol));
    
    int arbolCuenta[2];/// Pos 0= rojo, 1= tiene el total de vecinos
    int probContangio;
    
    for(cantEjecuciones=0; cantEjecuciones<5; cantEjecuciones++){
    startwtime = MPI_Wtime(); // Comienza a medir el tiempo
    // asigno valores
    if(my_rank== 0){
        mat= (Arbol *)malloc(N*N*sizeof(Arbol));
        //printf("Mostrar matriz:\n");
        for(j=0;j<N;j++){
            for(i=0;i<N;i++){
                // if(i==0){
                //   printf("[");
                // }
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
                // printf("%d  ",mat[posicion].estado);
                // if(i==N-1){
                //     printf("]\n");
                // }
                // else{
                //     printf(" ");
                // }
             }

        }

    }/// fin de proceso 0
    
    
    //-------------------------------------------------------------------------------------------------------------------------
    ///Ejecucion de reglas
    /*printf("Ingresa la cantidad de semana para simular (puede ser entre 240 hasta 4320)\n");
    scanf("%d",&ejecuciones);*/

    
    for(ciclo=0; ciclo<ejecuciones; ciclo++){



        MPI_Scatter(
            mat,
            sizeof(Arbol)*(N*tam),
            MPI_BYTE,
            matrizAux,
            sizeof(Arbol)*(N*tam),
            MPI_BYTE,
            0,
            MPI_COMM_WORLD);

        //int arbolCuenta[6];
        arbolCuenta[0]= 0;
        arbolCuenta[1]= 0;
        if(my_rank==0){
                    pasadorUltFilas(matrizAux, matrizAuxP);
                    MPI_Send(matrizAuxP, sizeof(Arbol)*(N*2), MPI_BYTE, my_rank+1, 0, MPI_COMM_WORLD);///envio al proceso 1
                    MPI_Recv(matrizAuxP, sizeof(Arbol)*(N*2), MPI_BYTE,my_rank+1, 0, MPI_COMM_WORLD, &status);
                    
                }
                else{
                    if(my_rank==comm_sz-1){
                        pasadorPrimerasFilas(matrizAux,matrizAuxP);
                        MPI_Send(matrizAuxP, sizeof(Arbol)*(N*2), MPI_BYTE, comm_sz-2, 0, MPI_COMM_WORLD);///envio al anteultimo proceso
                        MPI_Recv(matrizAuxP, sizeof(Arbol)*(N*2), MPI_BYTE, comm_sz-2, 0, MPI_COMM_WORLD, &status);///recibo del proceso anteultimo 
                    }
                    else{
                        
                        ///vecino anterior le envio 
                        pasadorPrimerasFilas(matrizAux,matrizAuxP);
                        MPI_Send(matrizAuxP, sizeof(Arbol)*(N*2), MPI_BYTE, my_rank-1, 0, MPI_COMM_WORLD);
                        ///vecino posterior le envio 
                        pasadorUltFilas(matrizAux, matrizAuxP);
                    
                        
                        MPI_Send(matrizAuxP, sizeof(Arbol)*(N*2), MPI_BYTE, my_rank+1, 0, MPI_COMM_WORLD);
                        ///vecino anterior lo recibo
                        MPI_Recv(matrizAuxP, sizeof(Arbol)*(N*2), MPI_BYTE, my_rank-1, 0, MPI_COMM_WORLD, &status);
                        ///vecino posterior lo recibo
                        MPI_Recv(matrizAuxP2, sizeof(Arbol)*(N*2), MPI_BYTE, my_rank+1, 0, MPI_COMM_WORLD, &status);
                                         
                    }
                }

        for(j=0;j<tam;j++){
            for(i=0;i<N;i++){
                posicion= j*N+i;
               ///calculos vecinos
                if(my_rank==0){
                    if(j>=tam-2 && j<tam){
                        vecinoProceso0(matrizAux,matrizAuxP,arbolCuenta,i,j);
                    }
                    else{
                        vecinoNormal(matrizAux,arbolCuenta,i,j);
                    }
                
                }
                if(my_rank==comm_sz-1){
                    if(j>=0 && j<2){
                        vecinoProcesoUlt(matrizAux,matrizAuxP,arbolCuenta,i,j);
                    }
                    else{
                        vecinoNormal(matrizAux,arbolCuenta,i,j);
                    }
                    
                }
                if(my_rank!=0 && my_rank!=comm_sz-1){
                    if(j>=0 && j<2){
                        vecinoProcesoUlt(matrizAux,matrizAuxP,arbolCuenta,i,j);
                    }
                    if(j>=tam-2 && j<tam){
                        vecinoProceso0(matrizAux,matrizAuxP2,arbolCuenta,i,j);
                    }
                    if(j>=2 && j<tam-2){
                        vecinoNormal(matrizAux,arbolCuenta,i,j);
                    }

                    //vecinoProcesoGeneral(matrizAux,matrizAuxP,matrizAuxP2,arbolCuenta,i,j);          
                }
               
                ///fin de calcular vecinos/////////////////////////////////////////////////////////////////////////////////////
                ///Comienzo de consulta de estado
               if(matrizAux[posicion].estado==VERDE){
                    //printf("aqui0\n");
                    reAsignarEdad(matrizAux,posicion);
                    fArbolSano(matrizAux,posicion, arbolCuenta,aux);
               }
               else{
                    if(matrizAux[posicion].estado== NARANJA){
                        if(matrizAux[posicion].tiempo == 3){
                            aux[posicion].estado=ROJO;
                            aux[posicion].edad= matrizAux[posicion].edad;
                            aux[posicion].herida= matrizAux[posicion].herida;
                            aux[posicion].tiempo= matrizAux[posicion].tiempo;
                        }
                        else{
                            aux[posicion].estado=NARANJA;
                            aux[posicion].edad= matrizAux[posicion].edad;
                            aux[posicion].herida= matrizAux[posicion].herida;
                            aux[posicion].tiempo= matrizAux[posicion].tiempo+1;
                        }
                    }
                    else{
                        if(matrizAux[posicion].estado==ROJO){
                            ///printf("aqui2\n");
                            ///funcion de arbol enfermo con sintomas

                            randoE= rand() % 100;
                            if(randoE <= 85){
                                 aux[posicion].estado=AZUL;
                                 aux[posicion].edad= matrizAux[posicion].edad;
                                 aux[posicion].herida= matrizAux[posicion].herida;
                                 aux[posicion].tiempo= 0;
                            }
                            else{

                                aux[posicion].estado=matrizAux[posicion].estado;
                                aux[posicion].edad= matrizAux[posicion].edad;
                                aux[posicion].herida= matrizAux[posicion].herida;
                                aux[posicion].tiempo= matrizAux[posicion].tiempo + 1;
                            }
                        }
                        else{
                            if(matrizAux[posicion].estado==AZUL){
                                ///printf("aqui3\n");
                                ///funcion de arbol con tratamiento
                                //fArbolTratamiento(mat,i,j,arbolCuenta,matrizAux);
                                if(matrizAux[posicion].tiempo >= 5){
                                    randoE= rand()%100;
                                    //printf("aca tiempo dentro de 8\n");
                                    if((matrizAux[posicion].edad >= 52 && matrizAux[posicion].edad <=156)){
                                        if(randoE<= 3){
                                            ///aca iria recuperado
                                            aux[posicion].estado=VERDE;
                                            aux[posicion].edad= matrizAux[posicion].edad;
                                            aux[posicion].herida= 0;
                                            aux[posicion].tiempo= 0;
                                        }
                                        else{
                                            ///aca iria podado
                                            aux[posicion].estado==BLANCO;
                                            aux[posicion].edad= matrizAux[posicion].edad;
                                            aux[posicion].herida= 0;
                                            aux[posicion].tiempo= 0;
                                        }

                                    }



                                    if((matrizAux[posicion].edad >= 157 && matrizAux[posicion].edad <=1820)){/// arbol adulto
                                        if(randoE <= 15){
                                            ///aca iria recuperado
                                            aux[posicion].estado=VERDE;
                                            aux[posicion].edad= matrizAux[posicion].edad;
                                            aux[posicion].herida= 0;
                                            aux[posicion].tiempo= 0;
                                        }
                                        else{
                                            ///aca iria podado
                                            aux[posicion].estado=BLANCO;
                                            aux[posicion].edad= matrizAux[posicion].edad;
                                            aux[posicion].herida= 0;
                                            aux[posicion].tiempo= 0;
                                        }

                                    }


                                    if(matrizAux[posicion].edad >=1821){ ///arbol viejo
                                        if(randoE <= 53){
                                            ///aca iria recuperado
                                            aux[posicion].estado= VERDE;
                                            aux[posicion].edad= matrizAux[posicion].edad;
                                            aux[posicion].herida= 0;
                                            aux[posicion].tiempo= 0;
                                        }
                                        else{
                                            ///aca iria reemplazado por un arbol joven
                                            aux[posicion].estado= VERDE;
                                            aux[posicion].edad= 1;
                                            aux[posicion].herida= 0;
                                            aux[posicion].tiempo= 0;
                                        }

                                    }

                                }
                                else{

                                    aux[posicion].estado=matrizAux[posicion].estado;
                                    aux[posicion].edad= matrizAux[posicion].edad;
                                    aux[posicion].herida= matrizAux[posicion].herida;
                                    aux[posicion].tiempo= matrizAux[posicion].tiempo + 1;
                                }
                            }
                            else{
                                if(matrizAux[posicion].estado== BLANCO){
                                    if(matrizAux[posicion].tiempo == 12){

                                        aux[posicion].estado= VERDE;
                                        aux[posicion].edad= matrizAux[posicion].edad;
                                        aux[posicion].herida= 0;
                                        aux[posicion].tiempo= 0;
                                    }
                                    else{

                                        aux[posicion].estado= BLANCO;
                                        aux[posicion].edad= matrizAux[posicion].edad;
                                        aux[posicion].herida= matrizAux[posicion].herida;
                                        aux[posicion].tiempo= matrizAux[posicion].tiempo+1;
                                    }
                                }
                            }
                        }
                    }
               }
               aux[posicion].edad= aux[posicion].edad+1;
             }
        }
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///actualizar matriz
        
        MPI_Gather(
        aux, 
        sizeof(Arbol)*(N*tam), 
        MPI_BYTE,
        mat, 
        sizeof(Arbol)*(N*tam), 
        MPI_BYTE, 
        0,
        MPI_COMM_WORLD);
        

        // if(my_rank==0){
        //     printf("Muestro matriz en semana %d\n",ciclo+1);
        //     imprimirMatriz(mat);
            
        // }

    }

    if(my_rank==0){
        free((void*)mat);
        //printf("Ejecucion numero: %d\n",cantEjecuciones+1);
        endwtime = MPI_Wtime(); // Termina de medir el tiempo Total

        tiempofinal= endwtime- startwtime;
        printf("Tiempo de ejecucion de una vuelta: %f\n",tiempofinal);
        tiempoTotal+= tiempofinal;
    }
    /*tiempo_final = clock();

    segundos = (double)(tiempo_final - tiempo_inicio) / CLOCKS_PER_SEC; /*según que estes midiendo el tiempo en segundos es demasiado grande

    printf("Tiempo de ejecucion de una vuelta: %f\n",segundos);
    tiempoTotal += (tiempo_final-tiempo_inicio);*/

        
    } 
    //printf("el tiempo promedio de las ejecuciones fue: %f\n",(double)(tiempoTotal/10)/CLOCKS_PER_SEC);*/
    if(my_rank==0){
        printf("el tiempo promedio de las ejecuciones fue: %f\n",(tiempoTotal/10));
    }
    MPI_Finalize();
    
    free((void*)matrizAux);
    free((void*)aux);
    free((void*)matrizAuxP);
    free((void*)matrizAuxP2);
}
