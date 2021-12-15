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

int main()
{

    const char *file_names[] = {"file1.txt", "file2.txt", "file3.txt"};
    char output[2048] = "";

    for (size_t i = 0; i < sizeof(file_names) / sizeof(file_names[0]); i++)
    {
        char cmd[2048] = "wc -l ";
        char output_line[2048] = "";

        strcat(cmd, file_names[i]);

        FILE *fp;
        if ((fp = popen(cmd, "r")) == NULL)
        {
            fprintf(stderr, "\n<CLIENT 1>\nError while popen\n\n");
            return 3;
        }

        fgets(output_line, 2048, fp);

        strcat(output, output_line);
    }

    int sockfd;
    char buffer[2048];
    struct sockaddr_in server_addr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("<CLIENT 1>\nSocket creation failed\n\n");
    }

    printf("<CLIENT 1>\nSocket was created\n\n");

    memset(&server_addr, 0, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int server_size = sizeof(server_addr);
    int server_bytes;

    sleep(1);
    sendto(sockfd, output, 2048, 0, (const struct sockaddr *)&server_addr, server_size);

    sleep(1);
    server_bytes = recvfrom(sockfd, (char *)buffer, 2048, MSG_WAITALL, (struct sockaddr *)&server_addr, &server_size);
    buffer[server_bytes] = '\0';
    printf("<CLIENT 1>\nMessage from server:\n%s\n\n", buffer);

    close(sockfd);
    return 0;
}
