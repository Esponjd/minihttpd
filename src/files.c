#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

int abrir_archivo(char *ruta){

    int fd_archivo;
    char path_archivo[512];

    //Comprobar que la ruta sea válida
    if(ruta == NULL || ruta[0] != '/'){
        return -1;
    }

    //Construir la ruta para el cliente
    int n = snprintf(path_archivo, sizeof(path_archivo),"www%s",ruta);

    fd_archivo = open(path_archivo, O_RDONLY);
    if (fd_archivo == -1){
        perror("Error al abrir el archivo");
        return -1;
    }

    return fd_archivo;

}