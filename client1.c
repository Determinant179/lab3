#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/*
Клиент1.
    Создать гнездо без установления соединения домена INET. Считать из серверного
    гнезда информацию. Определить количество строк, содержащихся в файлах, имена
    которых указаны в полученной информации, и передать ответ в серверное гнездо.
*/

char *operation(char *file_names)
{

    char output[2048] = "Lines amount:\n";
    char input[2048] = "";

    for (size_t i = 0; i < strlen(file_names); i++)
    {
        if (file_names[i] != '\n')
            strncat(input, &file_names[i], 1);
        else
        {
            char cmd[2048] = "wc -l ";
            char output_line[2048] = "";

            strcat(cmd, input);

            FILE *fp;
            if ((fp = popen(cmd, "r")) == NULL)
            {
                fprintf(stderr, "\n<CLIENT 1>\nError while popen\n\n");
                return 3;
            }

            fgets(output_line, 2048, fp);

            strcat(output, output_line);
            strcpy(input, "");
        }
    }

    return output;
}

int main()
{

    int sockfd;
    char buffer[2048];
    struct sockaddr_in server_addr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "<CLIENT 1>\nSocket creation failed\n\n");
        return 3;
    }

    printf("<CLIENT 1>\nSocket was created\n\n");

    memset(&server_addr, 0, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int server_size = sizeof(server_addr);
    int server_bytes = -1;

    char identify_message[] = "identify";

    while (server_bytes == -1)
    {
        sleep(1);
        sendto(sockfd, identify_message, strlen(identify_message), 0, (const struct sockaddr *)&server_addr, server_size);

        sleep(1);
        server_bytes = recvfrom(sockfd, (char *)buffer, 2048, MSG_DONTWAIT, (struct sockaddr *)&server_addr, &server_size);
    }

    printf("<CLIENT 1>\nMessage from server:\n%s\n\n", buffer);

    sleep(1);
    sendto(sockfd, operation(buffer), 2048, 0, (const struct sockaddr *)&server_addr, server_size);

    close(sockfd);
    return 0;
}
