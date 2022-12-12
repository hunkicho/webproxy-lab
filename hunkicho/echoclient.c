#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if(argc != 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);
    //식별자 clientfd를 rio에 위치한 rio_t 타입의 읽기 버퍼와 연결
    //(clientfd로 rio_t 구조체를 초기화)
    Rio_readinitb(&rio, clientfd);

    while(Fgets(buf, MAXLINE, stdin) != NULL)
    {
        for(int i=0; i < sizeof(buf) / sizeof(char); i++)
        {
            printf("%c",i);
        }
        
        //buf에서 clientfd로 strlen(buf)바이트로 전송
        Rio_writen(clientfd, buf, strlen(buf));
        // 다음 텍스트 줄을 rio에서 읽고 이것을 buf로 복사하고 텍스트라인을 null로 종료시킨다
        Rio_readlineb(&rio, buf, MAXLINE);
        // buf가 가지고 있는 문자열을 stdout에 쓴다
        Fputs(buf, stdout);
    }

    Close(clientfd);
    exit(0);
}