# Makefile for Proxy Lab 
#
# You may modify this file any way you like (except for the handin
# rule). You instructor will type "make" on your specific Makefile to
# build your proxy from sources.

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread

all: echoclient echoserveri proxy

# csapp.o: csapp.c csapp.h
# 	$(CC) $(CFLAGS) -c csapp.c

proxy.o: proxy.c csapp.h
	$(CC) $(CFLAGS) -c proxy.c

proxy: proxy.o csapp.o
	$(CC) $(CFLAGS) proxy.o csapp.o -o proxy $(LDFLAGS)

#client
echoclient.o: echoclient.c csapp.h
	$(CC) $(CFLAGS) -c echoclient.c
 
echoclient: echoclient.o csapp.o
	$(CC) $(CFLAGS) echoclient.o csapp.o -o echoclient $(LDFLAGS)

# echo.c
# echo.o: echo.c
# 	$(CC) $(CFLAGS) -c echo.c

#echo server
echoserveri.o: echoserveri.c csapp.h
	$(CC) $(CFLAGS) -c echoserveri.c

echoserveri: echoserveri.o csapp.o echo.o
	$(CC) $(CFLAGS) echoserveri.o csapp.o echo.o -o echoserveri $(LDFLAGS)

# Creates a tarball in ../proxylab-handin.tar that you can then
# hand in. DO NOT MODIFY THIS!
handin:
	(make clean; cd ..; tar cvf $(USER)-proxylab-handin.tar proxylab-handout --exclude tiny --exclude nop-server.py --exclude proxy --exclude driver.sh --exclude port-for-user.pl --exclude free-port.sh --exclude ".*")

# clean:
# 	rm -f *~ *.o proxy core *.tar *.zip *.gzip *.bzip *.gz
# clean:
# 	rm -f *~ *.o echoclient core *.tar *.zip *.gzip *.bzip *.gz
# clean:
# 	rm -f *~ *.o echoserveri core *.tar *.zip *.gzip *.bzip *.gz
clean:
	rm -f *~ *.o echoserveri echoclient core *.tar *.zip *.gzip *.bzip *.gz

