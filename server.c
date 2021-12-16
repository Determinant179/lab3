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
    struct sockaddr_in server_addr, current_client_addr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("<SERVER>\nSocket creation failed\n\n");
    }

    printf("<SERVER>\nSocket was created\n\n");

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&current_client_addr, 0, sizeof(current_client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    // Name the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("<SERVER>\nBind failed\n\n");
    }

    int current_client_size = sizeof(current_client_addr);

    int current_client_bytes;

    while (1)
    {
        sleep(1);
        current_client_bytes = recvfrom(sockfd, (char *)buffer, 2048, MSG_DONTWAIT, (struct sockaddr *)&current_client_addr, &current_client_size);
        if (current_client_bytes > -1)
        {
            if (buffer == "identify")
                continue;
            else if (buffer[0] == 'B')
                printf("<SERVER>\nMessage from client2:\n%s\n\n", buffer);
            else if (buffer[0] == 'L')
                printf("<SERVER>\nMessage from client1:\n%s\n\n", buffer);

            sendto(sockfd, output, 2048, 0, (const struct sockaddr *)&current_client_addr, current_client_size);
        }
    }

    return 0;
}
