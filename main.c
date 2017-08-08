// PJLINK projectors protocol simple implementaion
// dpolak 2013

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

char *get_ip(char *host);

int main(int argc, char *argv[]) {
    int sock;
    int tmpres;
    struct sockaddr_in *remote;
    char *ip;
    int ret;
    char buffer[128];
    char lamp1[6], lamp2[6];

    char * PanasonicIps[] = { "172.16.11.161", "172.16.11.162", "172.16.11.163", "172.16.11.164", "172.16.11.165",
                              "172.16.11.166", "172.16.11.167", "172.16.11.168", "172.16.11.169", "172.16.11.170",
                              "172.16.11.171", "172.16.11.172", "172.16.11.173", "172.16.11.175", "172.16.11.176",
                              "172.16.11.177", "172.16.11.178", "172.16.11.180", "172.16.11.181", "172.16.11.182",
                              "172.16.11.183"};

    char * PanasonicNames[] = { "T", "C1", "C2", "C3", "C4", "CM1", "CM2",
                                "SK", "w", "F1", "F2",
                                "F3", "F4", "SW", "BSJ", "USJ2", "USJ1",
                                "B", "FG", "KB", "KB" };

    char * EpsonIps[] = { "172.16.11.191", "172.16.11.192" };
    char * EpsonNames[] = { "CH", "ST" };

    int CountPanasonicIps = (sizeof(PanasonicIps)/4);
    int CountPanasonicNames = (sizeof(PanasonicNames)/4);

    for(int i = 0; i <= (CountPanasonicIps - 1);  i++) {
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		ip = get_ip(PanasonicIps[i]);
		remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
		remote->sin_family = AF_INET;
		tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));

		if( tmpres < 0) {
			perror("Can't set remote->sin_addr.s_addr");

		} else if(tmpres == 0) {
			fprintf(stderr, "%s is not a valid IP address\n", ip);

		}
		remote->sin_port = htons(4352);

		if(connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0){
			printf("Not connected\n");
			perror("Err");

		} else {
			memset(buffer, 0, 128);
			recv(sock, buffer, 128, 0);

			if(strncmp(buffer, "PJLINK 0", strlen("PJLINK 0")) == 0) {
				send(sock, "%1LAMP ?\r\n", strlen("%1LAMP ?\r\n"), 0);
				memset(buffer, 0, 128);
				recv(sock, buffer, 128, 0);
				memset(lamp1, 0, 6);
				memset(lamp2, 0, 6);
				buffer[0] = 'a';
				sscanf(buffer, "a1LAMP=%s %*s %s", lamp1, lamp2);
				printf("%s/%s/2000\n", lamp1, lamp2);
			}
		}
		free(remote);
		free(ip);
		close(sock);
    
    }
    return 1;
}

char *get_ip(char *host) {
  struct hostent *hent;
  int iplen = 15; 
  char *ip = (char *)malloc(iplen+1);
  memset(ip, 0, iplen+1);
  
  if((hent = gethostbyname(host)) == NULL) {
	herror("Can't get IP");
  }
  
  if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL) {
    perror("Can't resolve host");
  }
  
  return ip;
}
