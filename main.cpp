#include <iostream>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <netdb.h>

#include <signal.h>
#include <sys/wait.h>

#define MYPORT "6666"       // the port users will be connecting to
#define MAXBUFLEN 100

using namespace std;

int main()
{
    pid_t pid;
    int wstatus;

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;

    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP


    cout << "IT's GGBay\n";

    if((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }
        break;
    }

    if(p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    addr_len = sizeof their_addr;

    if((numbytes = recvfrom(sockfd, buf, (MAXBUFLEN - 1) , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    while(1) {
        pid = fork();
        if(pid == 0) {
            // child process
            execl("/home/vitalii/.local/bin/streamlink", "streamlink", "--player", "cvlc --fullscreen", buf, "best", NULL);
        }
        else {
            // parent process
            memset(buf, '\0', sizeof(buf));

            if((numbytes = recvfrom(sockfd, buf, (MAXBUFLEN - 1) , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                perror("recvfrom");
                exit(1);
            }

            // kill child process
            kill(pid, SIGTERM);

            waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
        }
    }

    close(sockfd);

    return 0;
}
