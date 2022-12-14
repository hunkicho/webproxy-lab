/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
 *     GET method to serve static and dynamic content.
 *
 * Updated 11/2019 droh
 *   - Fixed sprintf() aliasing issue in serve_static(), and clienterror().
 */
// #include "csapp.h"

// void doit(int fd);
// void read_requesthdrs(rio_t *rp);
// int parse_uri(char *uri, char *filename, char *cgiargs);
// void serve_static(int fd, char *filename, int filesize);
// void get_filetype(char *filename, char *filetype);
// void serve_dynamic(int fd, char *filename, char *cgiargs);
// void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
//                  char *longmsg);

// /* 한개의 HTTP 트랜잭션 처리 */
// void doit(int fd) // fd = file desciptor
// {
//   int is_static;
//   struct stat sbuf;
//   // MAXLINE : 8192, 2^23, 8kbyte
//   char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
//   char filename[MAXLINE], cgiargs[MAXLINE];
//   rio_t rio;

//   /* Read request line and headers */

//   //식별자 clientfd를 rio에 위치한 rio_t 타입의 읽기 버퍼와 연결
//   //(clientfd로 rio_t 구조체를 초기화) 
//   Rio_readinitb(&rio, fd);
//   // 다음 텍스트 줄을 rio에서 읽고 이것을 buf로 복사하고 텍스트라인을 null로 종료시킨다
//   Rio_readlineb(&rio, buf, MAXLINE); // 요청라인을 읽고 분석
//   printf("Request headers:\n");
//   printf("%s", buf);
//   printf("what is sscanf??\n");
//   sscanf(buf, "%s %s %s", method, uri, version);
//   printf("what is sscanf finish??\n");
//   if(strcasecmp(method, "GET") && strcasecmp(method, "HEAD"))
//   {
//     clienterror(fd, method, "501", "Not implemented", "Tiny does not implement this method");
//     return;
//   }
//   read_requesthdrs(&rio);  //post면 무시??

//   /* Parse URI from GET request */
//   is_static = parse_uri(uri, filename, cgiargs);    // 정적인지 동적인지 판단단
//   if(stat(filename, &sbuf) < 0)   // 디스크에 파일이 없으면
//   {
//     clienterror(fd, filename, "404", "Not found", "Tiny couldn't find this file");
//     return;
//   }

//   /* Serve static content */
//   if(is_static)
//   {
//     if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))  //보통 파일이 아니면서 GET 권한을 가지고 있지 않는
//     {
//       clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't read the file");
//       return;
//     }
//     serve_static(fd, filename, sbuf.st_size);
//   }
//   else /* Serve dynamic content */
//   {
//     if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))   //보통 파일이 아니면서 POST 권한을 가지고 있지 않는
//     {
//       clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't run the CGI program");
//       return;
//     }
//     serve_dynamic(fd, filename, cgiargs);
//   }
// }

// void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
// {
//   char buf[MAXLINE], body[MAXBUF];

//   /* Build the HTTP response body */
//   sprintf(body, "<html><title>Tiny Error</title>");
//   sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
//   sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
//   sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
//   sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

//   /* Print the HTTP response */
//   sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
//   Rio_writen(fd, buf, strlen(buf));
//   sprintf(buf, "content-type: text/html\r\n");
//   Rio_writen(fd, buf, strlen(buf));
//   sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
//   Rio_writen(fd, buf, strlen(buf));
//   Rio_writen(fd, body, strlen(body));
// }

// void read_requesthdrs(rio_t *rp)
// {
//   char buf[MAXLINE];

//   Rio_readlineb(rp, buf, MAXLINE);
//   while(strcmp(buf, "\r\n"))
//   {
//     Rio_readlineb(rp, buf, MAXLINE);
//     printf("%s", buf);
//   }
//   return;
// }

// int parse_uri(char *uri, char *filename, char *cgiargs)
// {
//   char *ptr;

//   if(!strstr(uri, "cgi-bin"))        /* Static content */
//   {
//     strcpy(cgiargs, "");       // cgi인자 스트링을 지우기
//     strcpy(filename, ".");     // 리눅스 상대경로로 변환 ㄷㅌ) ./index.html
//     strcat(filename, uri);
//     if(uri[strlen(uri) -1] == '/')
//     {
//       strcat(filename, "home.html");
//     }
//     return 1;
//   }
//   else
//   {
//     ptr = index(uri, '?');
//     if(ptr)
//     {
//       strcpy(cgiargs, ptr+1);   //cgi 파라미터 추출
//       *ptr = '\0';
//     }
//     else
//     {
//       strcpy(cgiargs, "");
//     }
//     strcpy(filename, ".");
//     strcat(filename, uri);
//     return 0;
//   }
// }

// void serve_static(int fd, char *filename, int filesize)
// {
//   int srcfd;
//   char *srcp, filetype[MAXLINE],  buf[MAXBUF];
//   /* Send response headers to client */
//   get_filetype(filename, filetype);                              // 파일의 접미어 부분을 검사해 파일 타입 결정
//   sprintf(buf, "HTTP/1.0 200 OK\r\n");                           //클라이언트에 응답줄과 응답헤더를 보낸다
//   sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
//   sprintf(buf, "%sConnection: close\r\n", buf);
//   sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
//   sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
//   Rio_writen(fd, buf, strlen(buf));
//   printf("Response headers:\n");
//   printf("%s", buf);

//   /* send response headers to client */

//   //요청한 파일의 내용을 연결식별자 fd로 복사해서 응답 본체를 보낸다.
//   /* 기존교재 방식 */
//   // srcfd = Open(filename, O_RDONLY, 0);                            // 읽기 위해서 filename을 오픈하고 식별자를 얻어온다.
//   // srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);     // 리눅스 mmap 함수로 요청한 파일을 가상메모리 영역으로 매핑
//   // Close(srcfd);                                                   // 파일을 메모리로 매핑한 후 이 식별자는 필요가 없기 때문에 닫는다(메모리 누수 방지)
//   // Rio_writen(fd, srcp, filesize);                                 // 실제로 파일을 클라이언트에 전송(주소 srcp에서 시작하는 filesize 바이트를 클라이언트의 연결식별자로 복사)
//   // Munmap(srcp, filesize);                                         // 매핑된 가상 메모리 주소를 반환(메모리 누수 방지)

//   /* 연습문제 11.9 */
//   srcfd = Open(filename, O_RDONLY, 0);
//   srcp = Malloc(filesize);
//   Rio_readn(srcfd, srcp, filesize);
//   Close(srcfd);
//   Rio_writen(fd, srcp, filesize);
//   Free(srcp);
// }

// /* get_filetype - Derive file type form filename*/
// void get_filetype(char *filename, char *filetype)
// {
//   if(strstr(filename, ".html"))
//   {
//     strcpy(filetype, "text/html");
//   }
//   else if(strstr(filename, ".gif"))
//   {
//     strcpy(filetype, "image/gif");
//   }
//   else if(strstr(filename, ".png"))
//   {
//     strcpy(filetype, "image/png");
//   }
//   else if(strstr(filename, ".jpg"))
//   {
//     strcpy(filetype, "image/jpeg");
//   }
//   else if(strstr(filename, ".mpg"))
//   {
//     strcpy(filetype, "image/mpg");
//   }
//   else
//   {
//     strcpy(filetype, "text/plain");
//   }
// }

// /*
//   Tiny 자식 프로세스를 fork하고, 그 후에 CGI 프로그램을 자식의 컨텍스트에서 실행하며 모든 종류의 동적 컨텐츠를 제공
//   클라이언트에  성공을 알려주는 응답 라인을 보내는 것으로 시작
//   CGI프로그램은 응답의 나머지 부분을 보내야 한다.
// */

// void serve_dynamic(int fd, char *filename, char *cgiargs)
// {
//   char buf[MAXLINE], *emptylist[] = {NULL};

//   /* Return first part of HTTP response */                              // 첫번째 응답을 보냄
//   sprintf(buf, "HTTP/1.0 200 OK\r\n");
//   Rio_writen(fd, buf, strlen(buf));
//   sprintf(buf, "Server: Tiny Web Server\r\n");
//   Rio_writen(fd, buf, strlen(buf));

//   if(Fork() == 0)  /* Child */                                          // 새로운 자식을 fork
//   {
//     /* Real server would set all CGI vars here */
//     setenv("QUERY_STRING", cgiargs, 1);                                 // QUERY_STRING 환경변수를 요청 URI의 CGI 인자들로 초기화(다른 CGI환경변수들도 마찬가지로 설정)
//     Dup2(fd, STDOUT_FILENO);           /* Redirect stdout to client */  //자식은 자식의 표준 출력을 연결 파일 식별자로 재지정(파일을 복사하는거(앞에꺼를 뒤에로))
//     Execve(filename, emptylist, environ);   /* Run CGI program */       //CGI프로그램을 로드하고 실행  
//   }
//   Wait(NULL); /* Parent waits for and reaps child */                    //부모는 자식이 종료되어 정리되는 것을 기다리기 위해 wait함수에서 블록
// }

// /*Tiny는 반복실행 서버로 명령줄에서 넘겨받은 포트로의 연결 요청을 듣는다.*/

// int main(int argc, char **argv) {
//   int listenfd, connfd;
//   char hostname[MAXLINE], port[MAXLINE];
//   socklen_t clientlen;
//   struct sockaddr_storage clientaddr;

//   /* Check command line args */
//   if (argc != 2) {
//     fprintf(stderr, "usage: %s <port>\n", argv[0]);
//     exit(1);
//   }

//   listenfd = Open_listenfd(argv[1]);                                                        // 듣기 소켓 오픈

//   // 반복적으로 연결요청을 접수
//   while (1) {
//     clientlen = sizeof(clientaddr);
//     connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);  // line:netp:tiny:accept     // 연결 요청 접수
//     Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
//     printf("Accepted connection from (%s, %s)\n", hostname, port);
//     doit(connfd);   // line:netp:tiny:doit                                                  // 트랜잭션 수행
//     Close(connfd);  // line:netp:tiny:close
//   }
// }



#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
                 char *longmsg);

/* 한개의 HTTP 트랜잭션 처리 */
void doit(int fd) // fd = file desciptor
{
  int is_static;
  struct stat sbuf;
  // MAXLINE : 8192, 2^23, 8kbyte
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char filename[MAXLINE], cgiargs[MAXLINE];
  rio_t rio;

  /* Read request line and headers */

  //식별자 clientfd를 rio에 위치한 rio_t 타입의 읽기 버퍼와 연결
  //(clientfd로 rio_t 구조체를 초기화) 
  Rio_readinitb(&rio, fd);
  // 다음 텍스트 줄을 rio에서 읽고 이것을 buf로 복사하고 텍스트라인을 null로 종료시킨다
  Rio_readlineb(&rio, buf, MAXLINE); // 요청라인을 읽고 분석
  printf("Request headers:\n");
  printf("%s", buf);
  sscanf(buf, "%s %s %s", method, uri, version);
  if(strcasecmp(method, "GET") && strcasecmp(method, "HEAD"))
  {
    clienterror(fd, method, "501", "Not implemented", "Tiny does not implement this method");
    return;
  }

  //read_requesthdrs(&rio);  //post면 무시??


  /* Parse URI from GET request */
  is_static = parse_uri(uri, filename, cgiargs);    // 정적인지 동적인지 판단단
  printf("what is filename?? %s\n",filename);
  if(stat(filename, &sbuf) < 0)   // 디스크에 파일이 없으면
  {
    clienterror(fd, filename, "404", "Not found", "Tiny couldn't find this file");
    return;
  }

  /* Serve static content */
  if(is_static)
  {
    if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))  //보통 파일이 아니면서 GET 권한을 가지고 있지 않는
    {
      clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't read the file");
      return;
    }
    serve_static(fd, filename, sbuf.st_size);
  }
  else /* Serve dynamic content */
  {
    if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))   //보통 파일이 아니면서 POST 권한을 가지고 있지 않는
    {
      clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't run the CGI program");
      return;
    }
    serve_dynamic(fd, filename, cgiargs);
  }
}

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
  char buf[MAXLINE], body[MAXBUF];

  /* Build the HTTP response body */
  sprintf(body, "<html><title>Tiny Error</title>");
  sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
  sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
  sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
  sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

  /* Print the HTTP response */
  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "content-type: text/html\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
  Rio_writen(fd, buf, strlen(buf));
  Rio_writen(fd, body, strlen(body));
}

void read_requesthdrs(rio_t *rp)
{
  char buf[MAXLINE];

  Rio_readlineb(rp, buf, MAXLINE);
  while(strcmp(buf, "\r\n"))
  {
    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
  }
  return;
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
  char *ptr;

  if(!strstr(uri, "cgi-bin"))        /* Static content */
  {
    strcpy(cgiargs, "");       // cgi인자 스트링을 지우기
    strcpy(filename, ".");     // 리눅스 상대경로로 변환 ex) ./index.html
    strcat(filename, uri);
    if(uri[strlen(uri) -1] == '/')
    {
      strcat(filename, "home.html");
    }
    return 1;
  }
  else
  {
    ptr = index(uri, '?');
    if(ptr)
    {
      strcpy(cgiargs, ptr+1);   //cgi 파라미터 추출
      *ptr = '\0';
    }
    else
    {
      strcpy(cgiargs, "");
    }
    strcpy(filename, ".");
    strcat(filename, uri);
    return 0;
  }
}

void serve_static(int fd, char *filename, int filesize)
{
  printf("in static!!!!!!\n");
  int srcfd;
  char *srcp, filetype[MAXLINE],  buf[MAXBUF];
  /* Send response headers to client */
  get_filetype(filename, filetype);                              // 파일의 접미어 부분을 검사해 파일 타입 결정
  sprintf(buf, "HTTP/1.0 200 OK\r\n");                           //클라이언트에 응답줄과 응답헤더를 보낸다
  sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
  sprintf(buf, "%sConnection: close\r\n", buf);
  sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
  sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
  Rio_writen(fd, buf, strlen(buf));
  printf("Response headers:\n");
  printf("%s", buf);

  /* send response headers to client */

  //요청한 파일의 내용을 연결식별자 fd로 복사해서 응답 본체를 보낸다.
  /* 기존교재 방식 */
  // srcfd = Open(filename, O_RDONLY, 0);                            // 읽기 위해서 filename을 오픈하고 식별자를 얻어온다.
  // srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);     // 리눅스 mmap 함수로 요청한 파일을 가상메모리 영역으로 매핑
  // Close(srcfd);                                                   // 파일을 메모리로 매핑한 후 이 식별자는 필요가 없기 때문에 닫는다(메모리 누수 방지)
  // Rio_writen(fd, srcp, filesize);                                 // 실제로 파일을 클라이언트에 전송(주소 srcp에서 시작하는 filesize 바이트를 클라이언트의 연결식별자로 복사)
  // Munmap(srcp, filesize);                                         // 매핑된 가상 메모리 주소를 반환(메모리 누수 방지)

  /* 연습문제 11.9 */
  srcfd = Open(filename, O_RDONLY, 0);
  srcp = Malloc(filesize);
  Rio_readn(srcfd, srcp, filesize);
  Close(srcfd);
  Rio_writen(fd, srcp, filesize);
  Free(srcp);
}

/* get_filetype - Derive file type form filename*/
void get_filetype(char *filename, char *filetype)
{
  if(strstr(filename, ".html"))
  {
    strcpy(filetype, "text/html");
  }
  else if(strstr(filename, ".gif"))
  {
    strcpy(filetype, "image/gif");
  }
  else if(strstr(filename, ".png"))
  {
    strcpy(filetype, "image/png");
  }
  else if(strstr(filename, ".jpg"))
  {
    strcpy(filetype, "image/jpeg");
  }
  else if(strstr(filename, ".mpg"))
  {
    strcpy(filetype, "image/mpg");
  }
  else if(strstr(filename, ".mp4"))
  {
    strcpy(filetype, "video/mp4");
  }
  else
  {
    strcpy(filetype, "text/plain");
  }
}

/*
  Tiny 자식 프로세스를 fork하고, 그 후에 CGI 프로그램을 자식의 컨텍스트에서 실행하며 모든 종류의 동적 컨텐츠를 제공
  클라이언트에  성공을 알려주는 응답 라인을 보내는 것으로 시작
  CGI프로그램은 응답의 나머지 부분을 보내야 한다.
*/

void serve_dynamic(int fd, char *filename, char *cgiargs)
{
  char buf[MAXLINE], *emptylist[] = {NULL};

  /* Return first part of HTTP response */                              // 첫번째 응답을 보냄
  sprintf(buf, "HTTP/1.0 200 OK\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Server: Tiny Web Server\r\n");
  Rio_writen(fd, buf, strlen(buf));

  if(Fork() == 0)  /* Child */                                          // 새로운 자식을 fork
  {
    /* Real server would set all CGI vars here */
    setenv("QUERY_STRING", cgiargs, 1);                                 // QUERY_STRING 환경변수를 요청 URI의 CGI 인자들로 초기화(다른 CGI환경변수들도 마찬가지로 설정)
    Dup2(fd, STDOUT_FILENO);           /* Redirect stdout to client */  //자식은 자식의 표준 출력을 연결 파일 식별자로 재지정(파일을 복사하는거(앞에꺼를 뒤에로))
    Execve(filename, emptylist, environ);   /* Run CGI program */       //CGI프로그램을 로드하고 실행  
  }
  Wait(NULL); /* Parent waits for and reaps child */                    //부모는 자식이 종료되어 정리되는 것을 기다리기 위해 wait함수에서 블록
}

/*Tiny는 반복실행 서버로 명령줄에서 넘겨받은 포트로의 연결 요청을 듣는다.*/

int main(int argc, char **argv) {
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  /* Check command line args */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);                                                        // 듣기 소켓 오픈

  // 반복적으로 연결요청을 접수
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);  // line:netp:tiny:accept     // 연결 요청 접수
    Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
    doit(connfd);   // line:netp:tiny:doit                                                  // 트랜잭션 수행
    Close(connfd);  // line:netp:tiny:close
  }
}
