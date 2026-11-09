    #include <netdb.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <sys/epoll.h>

    #define BUF_SIZE 1024
    #define QLEN 30
    #define MAX_EVENTS 10

    int passivesockTCP(const char *service, int qlen){

        int s, sfd;
        struct addrinfo hints;
        struct addrinfo *result, *aux_res;

        memset(&hints,0,sizeof(hints));
        hints.ai_family = AF_UNSPEC;  //Se permite IPV4 o IPV6
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = 0;  //0 Indica que el sistema operativo escoge el protocolo      
        hints.ai_flags = AI_PASSIVE;

        s = getaddrinfo(NULL,service,&hints,&result);

        if(s !=0){
            fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(s));
            return -1;
        }

        sfd = -1;

        for(aux_res = result; aux_res != NULL; aux_res = aux_res->ai_next){

            //Se añade la flag SOCK_NONBLOCK para especificar el modo del socket (no bloqueante)

            sfd = socket(aux_res->ai_family,aux_res->ai_socktype | SOCK_NONBLOCK, aux_res->ai_protocol);    
            
            if (sfd == -1)
                continue;
            
            if (bind(sfd, aux_res->ai_addr,aux_res->ai_addrlen) == -1){
                close(sfd);
                continue;
            }

            if(listen(sfd, qlen) == -1){
                close(sfd);
                continue;
            }

            break;
        }

        freeaddrinfo(result);

        if (sfd ==-1){
            perror("No se pudo crear el socket");
            exit(EXIT_FAILURE);
        }

        return sfd;
    }

    int server(){

        int msock, nsd, epoll_fd, num_events;
        const char service* = "8080";    

        msock = passivesockTCP(service,QLEN);
        
        //Creación de epoll
        epoll_fd = epoll_create1(0);

        if (epoll_fd == -1){
            perror("Error al crear epoll");
            exit(EXIT_FAILURE);
        
        }

        struct epoll_event ev ={
            .events = EPOLLIN,
            .data.fd = msock;
        };

        struct epoll_event events [MAX_EVENTS];


        if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD, msock, &ev) == -1){
            perror("Error al configurar epoll");
            exit(EXIT_FAILURE);
        }

        while(1){

            int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS,-1){
            if(num_events == -1){
                perror("Error en epoll_wait");
                exit(EXIT_FAILURE);
            }
                //Recorrer los eventos que están listos 
                for(int i=0; i<num_events;i++){

                    //fd auxiliar para recorrer la lista de eventos listos
                    int aux_fd = events[i].data.fd;

                        printf("FD actual: %d\n",aux_fd);

                        //Si epoll detecta un evento de msock, acepta la conexión del cliente
                        if(aux_fd == msock){

                            nsd = accept(msock, NULL, NULL);
                            
                            //Configuración del evento 
                            ev.events = EPOLLIN;
                            ev.data.fd = nsd;

                            epoll_ctl(epoll_fd,EPOLL_CTL_ADD, nsd,&ev);
                            
                            snprintf("Nuevo cliente, fd:%d\n", nsd);
                        }

                        else{

                        //Caso contrario, se procesan los eventos de los clientes

                        nsd = events[i].data.fd;

                        //Lógica de read, write conjunto el parsing para construir el mensaje de respuesta para el cliente
                

                }

                }
            }
        }
    }