#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/*
Сервер.
    Создать гнездо без установления соединения домена INET. Присвоить ему имя.
    Записать в гнездо информацию(имена) обо всех файлах текущего каталога.
    Распечатать ответы клиентов.

Клиент1.
    Создать гнездо без установления соединения домена INET. Считать из серверного
    гнезда информацию. Определить количество строк, содержащихся в файлах, имена
    которых указаны в полученной информации, и передать ответ в серверное гнездо.

Клиент2.
    Создать гнездо без установления соединения домена INET. Считать из серверного
    гнезда информацию. Определить количество байтов, содержащихся в файлах, имена
    которых указаны в полученной информации, и передать ответ в серверное гнездо.
*/

int main()
{

    FILE *fp;
    if ((fp = popen("ls -p | grep -v /", "r")) == NULL)
    {
        fprintf(stderr, "\n<SERVER>\nError while popen\n\n");
        return 3;
    }

    char output[2048] = "";
    char line[2048];

    int i = 0;
    while (1)
    {
        if (fgets(line, 2048, fp) == NULL)
            break;
        i++;
        strcat(output, line);
    }

    int sockfd;
    char buffer[2048];
    struct sockaddr_in server_addr, client1_addr, client2_addr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("<SERVER>\nSocket creation failed\n\n");
    }

    printf("<SERVER>\nSocket was created\n\n");

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client1_addr, 0, sizeof(client1_addr));
    memset(&client2_addr, 0, sizeof(client2_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Name the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("<SERVER>\nBind failed\n\n");
    }

    int client1_size = sizeof(client1_addr);
    int client2_size = sizeof(client2_addr);

    int client1_bytes, client2_bytes;

    sleep(1);
    client1_bytes = recvfrom(sockfd, (char *)buffer, 2048, MSG_WAITALL, (struct sockaddr *)&client1_addr, &client1_size);
    buffer[client1_bytes] = '\0';
    printf("<SERVER>\nMessage from client1:\n%s\n\n", buffer);

    sleep(1);
    sendto(sockfd, output, 2048, 0, (const struct sockaddr *)&client1_addr, client1_size);

    return 0;
}
