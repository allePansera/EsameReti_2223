#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 2525
#define BUFFER_SIZE 1024

int main() {
    int sockfd, newsockfd;
    socklen_t client_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    // Creazione del socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Errore nella creazione del socket");
        exit(1);
    }

    // Inizializzazione della struttura degli indirizzi del server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Binding del socket all'indirizzo del server
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Errore nel binding del socket");
        exit(1);
    }

    // Inizio dell'ascolto del socket
    if (listen(sockfd, 5) < 0) {
        perror("Errore nell'ascolto del socket");
        exit(1);
    }

    printf("Server in ascolto sulla porta %d...\n", PORT);

    while (1) {
        // Accettazione di una nuova connessione
        client_len = sizeof(client_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (newsockfd < 0) {
            perror("Errore nell'accettazione della connessione");
            exit(1);
        }

        // Ottieni il nome dell'host del server
        char hostname[BUFFER_SIZE];
        if (gethostname(hostname, BUFFER_SIZE) < 0) {
            perror("Errore nell'ottenimento del nome host");
            exit(1);
        }

        // Crea il messaggio di benvenuto
        snprintf(buffer, BUFFER_SIZE, "Welcome from %s\n", hostname);

        // Invia il messaggio di benvenuto al client utilizzando write()
        if (write(newsockfd, buffer, strlen(buffer)) < 0) {
            perror("Errore nell'invio del messaggio");
            exit(1);
        }

        // Chiudi la connessione
        close(newsockfd);
    }

    // Chiudi il socket principale
    close(sockfd);

    return 0;
}
