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

void consumidor();

int main () {
    struct mq_attr attr; /* Atributos de la cola */

    attr.mq_maxmsg = MAX_BUFFER;
    attr.mq_msgsize = sizeof(char);

    /* Apertura de los buffers */
    almacen1 = mq_open("/ALMACEN1", O_CREAT | O_WRONLY, 0644, &attr);
    almacen2 = mq_open("/ALMACEN2", O_CREAT | O_RDONLY, 0644, &attr);
    
    if (almacen1 == -1 || almacen2 == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    consumidor();

    mq_close(almacen1);
    mq_close(almacen2);
    mq_unlink("/ALMACEN1");
    mq_unlink("/ALMACEN2");

    exit(EXIT_SUCCESS);
}

void consumidor() {
    char msg; // Asumiendo que el tamaño del mensaje es suficiente para almacenar un 'char'
    struct mq_attr attr;
    
    while (1) { // Ciclo infinito, ajustar según la lógica de terminación deseada
        ssize_t bytes_read = mq_receive(almacen1, &msg, sizeof(msg), NULL);
        printf("Recibimos el mensaje\n");
        if (bytes_read >= 0) {
            printf("Mensaje recibido: %c\n", msg);
        } else {
            perror("mq_receive\n"); // Maneja errores adecuadamente
            exit(EXIT_FAILURE);
        }
    }
}
