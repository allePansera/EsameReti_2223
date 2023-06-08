#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){

    // inizializzazione dati
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[256];
    if (argc < 2) {
        fprintf(stderr,"usage %s hostname\n", argv[0]);
        exit(0);
    }
    // portno = atoi(argv[2]);
    portno = 2525;





    
    // 1) SOCKET(PROTOFAMILY, TYPE, PROTOCOL)
    /**
     * descriptor: è un intero
     * protofamily: PF_INET per Internet
     * type: SOCK_STREAM o SOCK-DGRAM (set to 0 since it's not used)
     * */ 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) {
        error("ERROR opening socket");
        
    }
    server = gethostbyname(argv[1]);
    
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    // alloca un array di zeri [0, 0, …, 0]
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    
    serv_addr.sin_port = htons(portno);




    
    // 2) CONNECT(SOCKET, CLIENT_ADDRESS, CLIENT_ADDRESS_LENGTH)
    if (connect(sockfd,(const struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }
    



    // 3) INVIO MESSAGGIO
    // printf("Please enter the message: ");
    
    // bzero(buffer, 256);
    
    // fgets(buffer, 255, stdin);
    
    //n = write(sockfd,buffer,strlen(buffer));
    
    //if (n < 0){
    //    error("ERROR writing to socket");
    //}
    
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    
    if (n < 0) {
        error("ERROR reading from socket");
    }
    
    printf("%s\n",buffer);
    return 0;
}
