#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <time.h>

#define MAX_BUFFER 5 /* tamaño del buffer */
#define DATOS_A_PRODUCIR 100 /* numero de datos a producir/consumir */

/* cola de entrada de mensajes para el productor */
mqd_t almacen1;
/* cola de entrada de mensajes para el consumidor */
mqd_t almacen2;


char producir_item(int iter){
	printf("Produjo la %c\n", 'a'+(iter % MAX_BUFFER));
	return 'a'+(iter % MAX_BUFFER);
}

void productor(){
	int vel=1;
	char item, empty;
	struct mq_attr attr;
	
	
	srand(time(NULL));
	
	for(int i=0; i<DATOS_A_PRODUCIR; i++){
	
		mq_getattr(almacen2, &attr);
		
		if(attr.mq_curmsgs==0){	
			printf("BUFFER VACIO\n");
		}
		
		//Recibir hueco vacio
		if(mq_receive(almacen1, &empty, sizeof(char), NULL) == -1){
			exit(0);
		}
		
		item = producir_item(i);
		
		//Enviar item
		if(mq_send(almacen2, &item, sizeof(char), 0) == -1){
			exit(0);
		}
			
		mq_getattr(almacen2, &attr);	
		
		if(attr.mq_curmsgs==MAX_BUFFER){
			printf("BUFFER LLENO\n");
		}
		
		sleep(rand()%2);
	}
}

int main(void) {
	struct mq_attr attr; /* Atributos de la cola */
	
	attr.mq_maxmsg = MAX_BUFFER;
	attr.mq_msgsize = sizeof (char);
	
	/* Borrado de los buffers de entrada
	por si existian de una ejecucion previa*/
	mq_unlink("/ALMACEN1");
	mq_unlink("/ALMACEN2");

	/* Apertura de los buffers */
	almacen1 = mq_open("/ALMACEN1", O_CREAT|O_RDONLY, 0777, &attr);
	almacen2 = mq_open("/ALMACEN2", O_CREAT|O_WRONLY, 0777, &attr);
	
	if ((almacen1 == -1) || (almacen2 == -1)) {
		perror ("mq_open");
		exit(EXIT_FAILURE);
	}
	
	
	
	productor();
	
	mq_close(almacen1);
	mq_close(almacen2);
	
	exit(EXIT_SUCCESS);
}
