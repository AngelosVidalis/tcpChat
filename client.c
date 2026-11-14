#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>



char *addresses[] = {
    "192.168.1.11",
    /*
    "192.168.1.21",
    "192.168.1.31",
    "192.168.1.41",
    "192.168.1.51"
     */
};
size_t count = sizeof(addresses) / sizeof(addresses[0]);


int c_connect( struct sockaddr_in *p_server_addr, socklen_t *p_s_addrlen, int *setopt_enable, int i){

    int cfd;


    /* CLIENT VARIABLES */
    cfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (cfd == -1)
    {
        fprintf(stderr, "Error at bind() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    if( setsockopt(cfd, SOL_SOCKET, SO_REUSEADDR, setopt_enable, sizeof(*setopt_enable)) == -1)
    {
        fprintf(stderr, "Error at setsockopt() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    struct sockaddr_in client_addr = {0};
    socklen_t p_addrlen;

    if (inet_pton(AF_INET, addresses[i], &client_addr.sin_addr) <= 0)
    {
        fprintf(stderr, "Error at inet_pton() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    client_addr.sin_family= AF_INET;

    /* CONNECT */
    if ( connect(cfd, (struct sockaddr *)p_server_addr, *p_s_addrlen) == -1)
    {
        fprintf(stderr, "Error at accept() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    close(cfd);

}

int main()

{
    char buff[16] = {0};

    printf("IP: ");
    if( fgets(buff, sizeof(buff), stdin) == NULL)
    {
        fprintf(stderr, "Error at fgets() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    char ip[16] = {0};
    if( sscanf(buff, "%15s", ip) != 1)
    {
        fprintf(stderr, "Error at sscanf() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    printf("PORT: ");
    if( fgets(buff, sizeof(buff), stdin) == NULL)
    {
        fprintf(stderr, "Error at fgets() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    unsigned short port;
    if( sscanf(buff, "%5hu", &port) != 1)
    {
        fprintf(stderr, "Error at sscanf() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    /* SERVER STRUCT */
    struct sockaddr_in server_addr = {0};
    server_addr.sin_port = htons(port);
    socklen_t s_addrlen;

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
    {
        fprintf(stderr, "Error at inet_pton() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    server_addr.sin_family= AF_INET;
    // server_addr.sin_port = htons(PORT); // htons - unsigned short - host2network
    s_addrlen = sizeof(server_addr);

    int setopt_enable = 1;
    for( int i = 0; i < count; i++)
    {
        //c_connect(&server_addr, &s_addrlen, &setopt_enable, i);
        c_connect(&server_addr, &s_addrlen, &setopt_enable, i);
    }
}