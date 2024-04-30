#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <time.h>

#define MAX_BUFFER 5 /* tamaño del buffer */
#define DATOS_A_CONSUMIR 100 /* numero de datos a producir/consumir */

/* cola de entrada de mensajes para el productor */
mqd_t almacen1;

/* cola de entrada de mensajes para el consumidor */
mqd_t almacen2;

int main(void) {
	struct mq_attr attr; // Atributos de la cola 
	
	attr.mq_maxmsg = MAX_BUFFER;
	attr.mq_msgsize = sizeof (char);
	
	//El productor se encarga del borrado de los buffers de entrada

	/* Apertura de los buffers */
	almacen1 = mq_open("/ALMACEN1", O_CREAT|O_WRONLY, 0777, &attr);
	almacen2 = mq_open("/ALMACEN2", O_CREAT|O_RDONLY, 0777, &attr);
	
	if ((almacen1 == -1) || (almacen2 == -1)) {
		perror ("mq_open");
		exit(EXIT_FAILURE);
	}
	
	consumidor(); //Se llama a la funcion consumidor
	
	mq_close(almacen1);
	mq_close(almacen2);
	
	exit(EXIT_SUCCESS);
}

void consumidor(){
	int vel=0;
	char item, empty=' ';
	struct mq_attr attr;
	
	
	srand(time(NULL));

	// Se comprueba que el buffer este lleno al inicio
	for(int i=0; i < MAX_BUFFER; i++){
	
		if(attr.mq_curmsgs == MAX_BUFFER){
			printf("BUFFER LLENO\n");
		}
		
		//Se mandan 5 huecos vacios
		if(mq_send(almacen1, &empty, sizeof(char), 0) == -1){
			exit(0);
		}
	}

    //Se consumen los items
	for(int i=0; i < DATOS_A_CONSUMIR; i++){
	
		//Recibe un item
		if(mq_receive(almacen2, &item, sizeof(char), NULL) == -1){
			exit(0);
		}
		
		mq_getattr(almacen1, &attr); //Se obtiene el numero de mensajes en la cola
		
		//Envia un hueco vacio
		if(mq_send(almacen1, &empty, sizeof(char), 0) == -1){
			exit(0);
		}
		
		mq_getattr(almacen1, &attr);
		printf("Item %c consumido\n", item);
		sleep(rand()%2);
	}
	
	//Se comprueba que el buffer este vacio al acabar
	mq_getattr(almacen1, &attr);
	
	if(attr.mq_curmsgs==MAX_BUFFER){
		printf("BUFFER VACIO AL FINALIZAR\n");
	}
	
}