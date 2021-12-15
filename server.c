// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Driver code
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
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("<SERVER>\nSocket creation failed\n\n");
    }

    printf("<SERVER>\nSocket was created\n\n");

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("<SERVER>\nBind failed\n\n");
    }

    int len, n;

    len = sizeof(cliaddr); // len is value/resuslt

    n = recvfrom(sockfd, (char *)buffer, 2048, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    printf("<SERVER>\nMessage from client1:\n%s\n\n", buffer);
    sendto(sockfd, output, 2048, 0, (const struct sockaddr *)&cliaddr, len);

    return 0;
}
