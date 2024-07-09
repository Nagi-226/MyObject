// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "database.c" // 包含数据库操作

#define PORT 8080
#define BUF_SIZE 1024

void *handle_client(void *arg);
void query_word(int client_sock, const char *word, int user_id);

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    if (!init_db("dictionary.db")) {
        fprintf(stderr, "Failed to initialize database\n");
        return EXIT_FAILURE;
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d\n", PORT);

    while ((client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) >= 0) {
        pthread_t tid;
        int *client_sock_ptr = malloc(sizeof(int));
        *client_sock_ptr = client_sock;
        pthread_create(&tid, NULL, handle_client, client_sock_ptr);
    }

    close_db();
    return 0;
}

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    free(arg);
    char buffer[BUF_SIZE];
    int user_id = -1;

    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int read_size = recv(client_sock, buffer, BUF_SIZE, 0);
        if (read_size <= 0) {
            break;
        }

        char *cmd = strtok(buffer, " ");
        if (strcmp(cmd, "REGISTER") == 0) {
            char *username = strtok(NULL, " ");
            char *password = strtok(NULL, " ");
            if (register_user(username, password)) {
                send(client_sock, "REGISTERED\n", 11, 0);
            } else {
                send(client_sock, "REGISTER_FAILED\n", 16, 0);
            }
        } else if (strcmp(cmd, "LOGIN") == 0) {
            char *username = strtok(NULL, " ");
            char *password = strtok(NULL, " ");
            if (authenticate_user(username, password)) {
                user_id = 1; // 假设用户ID为1
                send(client_sock, "LOGIN_SUCCESS\n", 14, 0);
            } else {
                send(client_sock, "LOGIN_FAILED\n", 13, 0);
            }
        } else if (strcmp(cmd, "QUERY") == 0 && user_id != -1) {
            char *word = strtok(NULL, " ");
            query_word(client_sock, word, user_id);
        } else {
            send(client_sock, "INVALID_COMMAND\n", 16, 0);
        }
    }

    close(client_sock);
    return NULL;
}

void query_word(int client_sock, const char *word, int user_id) {
    FILE *file = fopen("dictionary.txt", "r");
    if (!file) {
        send(client_sock, "ERROR_OPENING_DICTIONARY\n", 25, 0);
        return;
    }

    char line[BUF_SIZE];
    while (fgets(line, sizeof(line), file)) {
        char *dict_word = strtok(line, ":");
        char *definition = strtok(NULL, "\n");
        if (strcmp(dict_word, word) == 0) {
            log_query(user_id, word);
            send(client_sock, definition, strlen(definition), 0);
            fclose(file);
            return;
        }
    }

    send(client_sock, "WORD_NOT_FOUND\n", 15, 0);
    fclose(file);
}