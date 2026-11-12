#include <string.h>

const char* obtener_mime(char *ruta){

    //Buscamos el último punto de la ruta, para referenciarnos a la extensión del archivo
    const char *ext = strrchr(ruta,'.');

    //Si no tiene una extensión se devuelve un flujo de bytes
    if(ext == NULL){
        return "application/octet-stream";
    }

    if(strcmp(ext, ".html") == 0)
        return "text/html; charset=utf-8";

    if(strcmp(ext, ".css") == 0)
        return "text/css; charset=utf-8";

    if(strcmp(ext, ".webp") == 0)
        return "image/webp";

    if(strcmp(ext, ".png") == 0)
        return "image/png";

    if(strcmp(ext, ".jpg") == 0)
        return "image/jpg";

    //Si es una extensión desconocida, se devuelve un flujo de bytes.

    return "application/octet-stream";
}