#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>

#define PORT 12345
#define LCD_FILE "/dev/lcd_screen" //Module LCD
#define DHT11_FILE "/dev/my_dht11" //Module DHT11
#define LOG_FILE "temperature_log.txt"
#define BUFFER_SIZE 10

static char buffer[1024] = {0};
static char temp_str[BUFFER_SIZE] = {};
static char message[BUFFER_SIZE] = {}; //Message personnalisee
static char history[50];
static char ligne[120];
static char time_str[20];
static char read_line[1000];

// Ajoutez ces variables globales
pthread_t temp_thread;
static int capture_temp_enabled = 0;
static int capture_completed = 0;

void capture_temp();

// Modifiez la fonction capture_temp pour utiliser un thread
void* capture_temperature(void* arg) {
    while (capture_temp_enabled) {
	 capture_temp();
	 sleep(1);
    }
    pthread_exit(NULL);
    capture_completed = 1;
   }


void read_temp(int client_socket) {
    int fd2 = open(DHT11_FILE,O_RDONLY,0644);
    if (fd2 == -1) {
      perror("error open file\n");
      }
    ssize_t bytes_read = read(fd2,temp_str,BUFFER_SIZE);
    if (bytes_read == -1) {
       perror("Error read sensor DHT11 \n");
       close(fd2);
     } else {
       temp_str[bytes_read] = '\0';
       printf("Received Temperature = %s \n",temp_str);
       // Stocker la valeur de temp      rature dans l'historique
       send(client_socket,temp_str,BUFFER_SIZE,0);
       }
       close(fd2);
}

//Amelioration du programme sans besoin de se connecter au client
void capture_temp() {

   time_t start_time = time(0);
   time_t current_time;
   struct tm *time_info;
   char time_str[20];

   while (time(0) - start_time < 15) {
	    int fd3 = open(DHT11_FILE, O_RDONLY, 0644);

            if (fd3 == -1) {
              perror("error open file\n");
              return;
             }

            ssize_t bytes_read = read(fd3, ligne, BUFFER_SIZE);
	    //printf(" new bytes_read = %d \n",bytes_read);
            if (bytes_read == -1) {
                perror("Error read sensor DHT11 \n");
                break;
            }
            ligne[bytes_read] = '\0';

	    //printf("Valeur de temperature  = %s \n",ligne);

	    FILE *log_file = fopen(LOG_FILE, "a");
   	    if (log_file == NULL) {
        	perror("Error opening log file\n");
       		 return;
            }

	    time(&current_time);
            time_info = localtime(&current_time);
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);

	    fprintf(log_file, "Date/heure : %s, Température : %s °C\n", time_str, ligne);

	    sleep(5);
            close(fd3);
            fclose(log_file);
        }
    }

void write_lcd(int client_socket) {
     read(client_socket, message, sizeof(message));
     int fd = open(LCD_FILE, O_WRONLY, 0644);
     ssize_t bytes_read_2 = read(fd,temp_str,sizeof(temp_str));
     temp_str[bytes_read_2] = '\0';
     if (fd == -1) {
          perror("error open file\n");
     }
     ssize_t bytes_written = write(fd,message, sizeof(message) - 1);
     if (bytes_written == -1) {
          perror("writing error to LCD \n");
         close(fd);
     }
     close(fd);
}


void write_temp_LCD(int client_socket)
{
     int fd = open(LCD_FILE, O_WRONLY, 0644);
     int fd3 = open(DHT11_FILE,O_RDONLY,0644);
     ssize_t bytes_read_2 = read(fd3,temp_str,sizeof(temp_str));
     temp_str[bytes_read_2] = '\0';
     if (fd3 == -1) {
          perror("error open file\n");
     }
     printf("Received Temperature = %s \n",temp_str);
     ssize_t bytes_written_2 = write(fd,temp_str, sizeof(temp_str));
     if (bytes_written_2 == -1) {
         perror("writing error to LCD \n");
	 close(fd);
	 close(fd3);
     }
     close(fd);
     close(fd3);
}

void read_history(int client_socket) {
   // Ouvrir le fichier journal en lecture
    FILE *log_file = fopen(LOG_FILE, "rb");
    if (log_file == NULL) {
        perror("Error opening log file for reading\n");
        return;
    } else {
        fseek(log_file,0,SEEK_END);
        long file_size = ftell(log_file);
	fseek(log_file,0,SEEK_SET);

        if(file_size > 0) {
	   char* file_content = (char*)malloc(file_size + 1);

	if(fread(file_content,1,file_size,log_file) == (size_t)file_size) {
	   file_content[file_size] = '\0';
	   write(client_socket,file_content,file_size);
	}
       free(file_content);
       }
    }
    fclose(log_file);
}

void handle_client(int client_socket) {

     while (1) {
        int valread = read(client_socket, buffer, sizeof(buffer));
        if (valread <= 0) {
            break;
        }

	printf("Option selectionned by client %s \n",buffer);


	if (strcmp(buffer, "START_CAPTURE") == 0) {
            capture_completed = 1; // Activate capture temperature
        }

        if (strcmp(buffer, "STOP_CAPTURE") == 0) {
            capture_completed = 0; // Desativate capture temperature
	    if (capture_completed) {
       	        printf("La capture de température est terminée.\n");
            }
	}

	message[0] =  '\0';

        switch (buffer[0]) {
	   //Ecriture de LCD
            case '1':
		write_lcd(client_socket);
                break;
	   //Lecture de la temperature
	   case '2':
		read_temp(client_socket);
                break;
           //Faire la capture de la temperature
	   case '3':
                capture_temp(client_socket);
                break;
           //Lecture de l'historique
	    case '4':
		read_history(client_socket);
                break;
	    //Ecriture write_temp_LCD
            case '5':
		write_temp_LCD(client_socket);
                break;
	    case '6':
		break;
	    case '7':
	        break;
	    case '8':
		break;
            default:
               break;
	}
        snprintf(buffer, sizeof(buffer), "%s", message);
        write(client_socket, buffer, strlen(buffer));
    }
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Wait for connections...\n");

   // Créez le thread de capture de température
    if (pthread_create(&temp_thread, NULL, capture_temperature, NULL) != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

   while (1) {
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len)) < 0) {
            perror("Acceptance failed");
            exit(EXIT_FAILURE);
        }

	printf("New connection accepted \n");

        handle_client(client_socket);
    }

    pthread_join(temp_thread, NULL);

    return 0;
}
