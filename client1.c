// Client side implementation of UDP client-server model
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
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("<CLIENT 1>\nSocket creation failed\n\n");
    }

        printf("<CLIENT 1>\nSocket was created\n\n");

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;

    sendto(sockfd, output, 2048, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    n = recvfrom(sockfd, (char *)buffer, 2048, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("<CLIENT 1>\nMessage from server:\n%s\n\n", buffer);

    close(sockfd);
    return 0;
}
