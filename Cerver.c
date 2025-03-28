#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>

int main() {
    int server_socket, client_socket;
    char buffer[256];
    char response[256];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("Erro no bind");
        exit(EXIT_FAILURE);
    }

    listen(server_socket, 5);
    printf("Servidor ouvindo na porta 9002 vai bateria!!!!!\n");

    while (1) {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Erro ao aceitar conexão :(");
            continue;
        }
        printf("Cliente conectado :D\n");

        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            perror("Erro ao receber dados ;-;");
            close(client_socket);
            continue;
        }
        buffer[strcspn(buffer, "\r\n")] = 0;
        printf("Eba! Dados recebidos: %s\n", buffer);

        if (strcmp(buffer, "EXIT") == 0) {
            printf("Comando EXIT recebido. Encerrando o servidor ate mais\n");
            close(client_socket);
            break;
        }


        double n1, n2, resultado;
        char operador;
        if (sscanf(buffer, "%lf %c %lf", &n1, &operador, &n2) != 3) {
            sprintf(response, "Erro: entrada invalida. Formato esperado: n1 operador n2 ou EXIT escreve direito");
            send(client_socket, response, strlen(response), 0);
            close(client_socket);
            continue;
        }

        switch(operador) {
            case '+':
                resultado = n1 + n2;
                break;
            case '-':
                resultado = n1 - n2;
                break;
            case '*':
                resultado = n1 * n2;
                break;
            case '/':
                if (n2 == 0) {
                    sprintf(response, "Erro: divisao por zero nao sou um computador quantico");
                    send(client_socket, response, strlen(response), 0);
                    close(client_socket);
                    continue;
                }
                resultado = n1 / n2;
                break;
            case '^':
                resultado = pow(n1, n2);
                break;
            case '%':
                if (n2 == 0) {
                    sprintf(response, "Erro: modulo por zero.");
                    send(client_socket, response, strlen(response), 0);
                    close(client_socket);
                    continue;
                }
                resultado = fmod(n1, n2);
                break;
            case 'm':
                resultado = (n1 > n2) ? n1 : n2;
                break;
            default:
                sprintf(response, "Erro: nao mapeamos esse operador nao fomos tao criativos");
                send(client_socket, response, strlen(response), 0);
                close(client_socket);
                continue;
        }

        sprintf(response, "Resultado: %lf", resultado);
        send(client_socket, response, strlen(response), 0);

        close(client_socket);
    }

    close(server_socket);
    return 0;
}