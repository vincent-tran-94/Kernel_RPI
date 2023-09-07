#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>


#define PORT 12345
#define BUFFER_SIZE 10


static char menu_choice[10];
static char message[10];
static char temp_str[BUFFER_SIZE];
static char history_buffer[12000];
static char info[1024];
static char freq_command[20];


int main() {
    int sock = 0;
    struct sockaddr_in server_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    //server_addr.sin_addr.s_addr = inet_addr("192.168.1.117"); //Locale
    //server_addr.sin_addr.s_addr = inet_addr("192.168.1.38"); //Etage 30
    //server_addr.sin_addr.s_addr = inet_addr("192.168.1.124"); //Mezzanine
    server_addr.sin_addr.s_addr = inet_addr("192.168.30.191"); //Soutenance


    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected \n");

    while (1) {
        printf("Menu:\n");
        printf("Option 1: Personnaliser le message d'accueil de l'écran LCD \n");
        printf("Option 2: Consulter la dernière valeur de température capturée. \n");
	printf("Option 3: Lancer la capture de données de la temperature\n");
        printf("Option 4: Accéder à l'historique des valeurs de température. \n");
	printf("Option 5: Afficher la température sur l'écran LCD \n");
        printf("Option 7: Stopper la capture de données de la temperature \n");
	printf("Option 8: Déconnexion \n");
	printf("Choisissez une option : ");
        scanf("%s", menu_choice);

        write(sock, menu_choice, strlen(menu_choice));

	if (strcmp(menu_choice,"1") == 0) {
	   printf("Saisir votre message personnalisée: ");
           scanf("%s",message);
	   write(sock,message,strlen(message));
	   recv(sock, info, sizeof(info), 0);
           printf("%s\n", info);
	   continue;
	}

	if (strcmp(menu_choice,"2") == 0) {
	   recv(sock, temp_str, sizeof(temp_str),0);
           printf("La valeur de température vaut: %s °C  \n", temp_str);
	   continue;
	}

	if (strcmp(menu_choice, "3") == 0) {
        	write(sock, "START_CAPTURE", strlen("START_CAPTURE"));
        	continue;
	}

	if (strcmp(menu_choice, "4") == 0) {
         int bytes_received;

        // Réception des données ligne par ligne
        bytes_received = read(sock, history_buffer, sizeof(history_buffer));
        history_buffer[bytes_received] = '\0';
        printf("%s", history_buffer);

        if (bytes_received == -1) {
                perror("Error receiving data");
        }
             continue;
        }


    	if (strcmp(menu_choice, "7") == 0) {
        	write(sock, "STOP_CAPTURE", strlen("STOP_CAPTURE"));
        	continue;
    	}

	if (strcmp(menu_choice, "8") == 0) {
            printf("Déconnexion\n");
            break;
        }


    }
    close(sock);
    return 0;
}
