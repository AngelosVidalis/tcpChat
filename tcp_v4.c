#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define MAX_PEERS 5
#define LISTEN_BACKLOG 5
#define PORT 50000


/* defined netinet/in.h
struct sockaddr_in {
    sa_family_t    sin_family;    // Address family, unsigned integer
    in_port_t   sin_port;         // Port number
    struct in_addr sin_addr;      // Socket address.
    unsigned char  sin_zero[8];   // Padding
}

struct in_addr {
    in_addr_t s_addr;
};

typedef uint32_t in_addr_t; // unsigned int 32bit
typedef uint16_t in_port_t; // unsigned int 16bit
*/



void cleanup(int *p)
{
    for(int i = 0; i < MAX_PEERS; i++)
    {
        close(p[i]);
    }
}


int main(void){

    int setopt_enable = 1;
    int peers[MAX_PEERS] = {0};
    int sfd; /* server file descriptor */
    int cfd; /* client file descriptor */
    int pfd; /* peer file descriptor */
    char ip[] = "127.0.0.1";

    /* SOCKET 'creates an endpoint for communication and returns a file descriptor that refers to that endpoint' */

    sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sfd == -1)
    {
        fprintf(stderr, "Error at bind() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }
    /*
    int domain, int type, int protocol
    protocol family, communication semantics, protocol
     */

    /* Set local_addr reusability */
    if( setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &setopt_enable, sizeof(setopt_enable)) == -1)
    {
        fprintf(stderr, "Error at setsockopt() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    /* SERVER VARIABLES */
    struct sockaddr_in server_addr = {0};
    socklen_t s_addrlen;

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
     {
        fprintf(stderr, "Error at inet_pton() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }


    server_addr.sin_family= AF_INET;
    // server_addr.sin_port = htons(PORT); // htons - unsigned short - host2network
    s_addrlen = sizeof(server_addr);


    /* BIND 'assigns the address specified by addr to the socket referred to by the file descriptor sockfd “assigning a name to a socket” */
    if( bind(sfd, (struct sockaddr *)&server_addr, s_addrlen) == -1)
    {
        fprintf(stderr, "Error at bind() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }


    /* LISTEN 'marks the socket referred to by sockfd as a passive socket' */
    if(listen(sfd, LISTEN_BACKLOG) == -1)
    {
        fprintf(stderr, "Error at listen() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    printf("IP: %s\n", ip);
    printf("Listening on port %d...\n", PORT);
    // accept() blocks until a client connects
    printf("Waiting for a client...\n");



    /* ACCEPT */

    // struct sockaddr peer_addr = {0};
    // socklen_t p_addrlen = sizeof(peer_addr);
    //pfd = accept(sfd, &peer_addr, &p_addrlen);
    pfd = accept(sfd, NULL, NULL);
    if ( pfd == -1 )
    {
        fprintf(stderr, "Error at accept() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }
    printf("%d\n", pfd);

    /* CLOSE */

    if ( close(sfd) == -1)
    {

        cleanup(peers);
        fprintf(stderr, "Error at close() [%s] in file %s at line # %d\n", strerror(errno), __FILE__, __LINE__ - 2);
        return EXIT_FAILURE;
    }

    cleanup(peers);
    puts("Passed");





}