#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "files.h"
#include "mime.h"

void respuesta_HTTP(int codigo_estado, char *ruta, int ssock){

    int fd = -1;
    int nread;
    char header[1024];
    char buffer[1024];
    const char *mensaje_estado;
    const char *content_type;
    const char *body = NULL;

    if(codigo_estado == 200){
        fd = abrir_archivo(ruta);
        if (fd == -1){
            codigo_estado = 404;
        }
    }

    switch (codigo_estado){
        case 200:
            mensaje_estado = "OK";
            break;
        case 400:
            mensaje_estado = "Bad Request";
            break;
        case 404:
            mensaje_estado = "Not Found";
            break;
        case 405:
            mensaje_estado = "Method Not Allowed";
            break;
        case 505:
            mensaje_estado = "HTTP Version Not Supported";
        default:
            codigo_estado = 500;
            mensaje_estado = "Internal Server Error";
            break;
    }  
     
        if(codigo_estado == 200){
            content_type = obtener_mime(ruta);
            snprintf(header, sizeof(header), "HTTP/1.1 %d %s \r\n"
                                            "Content-Type: %s \r\n"
                                            "Connection: close \r\n\r\n"
            ,codigo_estado, mensaje_estado, content_type);

            write(ssock, header, strlen(header));

            while((nread = read(fd, buffer, sizeof(buffer))) > 0){
                write(ssock, buffer, nread);
            }
            close(fd);

        } else {
            content_type = "text.html";
            snprintf(header, sizeof(header),
                    "HTTP/1.1 %d %s\r\n"
                    "Content-Type: %s\r\n"
                    "Connection: close\r\n\r\n"
                    ,codigo_estado, mensaje_estado, content_type);

            write(ssock, header, strlen(header));

        }
    }

int parsing_HTTP(char *bufRequest, int ssock){

    char metodo[16];
    char ruta[256];
    char version[16];
    int codigo_estado = 200;

    if (sscanf(bufRequest, "%15s %255s %15s", metodo, ruta, version) != 3){
        codigo_estado = 400;
    }
    
    if(strcmp(metodo,"GET") != 0){
        codigo_estado = 405;
    }

    if(strcmp(version,"HTTP/1.1") != 0){
        codigo_estado = 505;
    }

    respuesta_HTTP(codigo_estado, ruta, ssock);
        
}