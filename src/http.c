#include <stdio.h>
#include <string.h>

int parsing_HTTP(char *bufRequest){

    char metodo[16];
    char ruta[256];
    char version[16];

    if (sscanf(bufRequest, "%15s %255s %15s", metodo, ruta, version) == 3){

        //Si el metodo proporcionado no es GET, retornamos el código de estado 405 (Method Not Allowed)
        if(strcmp(metodo,"GET") != 0){
        
        }

        //Si la versión de HTTP no es 1.1, retornamos el código de estado 400 (Bad Request)
        if(strcmp(version,"HTTP/1.1") != 0){

        }

        //Lógica para obtener un archivo

        //Si el archivo no existe, retornamos el código de estado 404 (Not Found)
        
    } else{
        // Si no se logró recuperar los 3 elementos de request, retornamos un error
    }
}