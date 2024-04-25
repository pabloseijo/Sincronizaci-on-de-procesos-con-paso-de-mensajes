#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>   
#include <string.h>     /* For mode constants */

#define MAX_BUFFER 5         /* Tamaño del buffer */
#define DATOS_A_PRODUCIR 100 /* Número de datos a producir/consumir */

/* Cola de entrada de mensajes para el productor y consumidor */
mqd_t almacen1, almacen2;

void productor(void); // Prototipo de la función

int main(void) {
    struct mq_attr attr; /* Atributos de la cola */
    attr.mq_maxmsg = MAX_BUFFER;
    attr.mq_msgsize = sizeof(char);

    /* Borrado de los buffers de entrada por si existían de una ejecución previa */
    mq_unlink("/ALMACEN1");
    mq_unlink("/ALMACEN2");

    /* Apertura de los buffers */
    almacen1 = mq_open("/ALMACEN1", O_CREAT | O_WRONLY, 0644, &attr);
    almacen2 = mq_open("/ALMACEN2", O_CREAT | O_RDONLY, 0644, &attr);
    if (almacen1 == -1 || almacen2 == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    productor();

    mq_close(almacen1);
    mq_close(almacen2);
    mq_unlink("/ALMACEN1");
    mq_unlink("/ALMACEN2");

    exit(EXIT_SUCCESS);
}

void productor(void) {
    // Implementa la producción de mensajes aquí
    for (int i = 0; i < DATOS_A_PRODUCIR; i++) {
        char msg = 'a' + (i % MAX_BUFFER);
        printf("Producimos el elemento %c\n", msg);
        if (mq_send(almacen1, &msg, sizeof(msg), 0) == -1) {
            perror("mq_send");
            exit(EXIT_FAILURE);
        }
    }
}
