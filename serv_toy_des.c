// TOY DES Cryptography and Network Security 1 HW1 Josh Barthelmess

#include "HW1.h"
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<strings.h>
#include<stdio.h>


void do_decrypt(char* plain, char* dest, int num_bytes, int key)
{
    int i = 0;
    while(i < num_bytes)
    {
        int encrypt_text = plain[i] & 255;
        int result = decrypt(encrypt_text, key);
        dest[i] = result & 255;
        i++;
    }
}

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        fprintf(stderr, "USAGE: ./a.out <KEY> <FILENAME> <PORT>\n");
        return EXIT_FAILURE;
    }
    
    int key = atoi(argv[1]);
    int port = atoi(argv[3]);
    
    struct sockaddr_in local, client;
    
    bzero(&local, sizeof(local));
    local.sin_port = htons(port);
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(bind(fd, (struct sockaddr*) &local, sizeof(local)) == -1)
    {
        fprintf(stderr, "Bind Error\n");
        return EXIT_FAILURE;
    }
    if(listen(fd, 2) == -1)
    {
        fprintf(stderr, "Listen error\n");
        return EXIT_FAILURE;
    }
    int unused;
    int conn = accept(fd, (struct sockaddr *) &client, &unused);
    if(conn <0)
    {
        fprintf(stderr, "accept error\n");
        return EXIT_FAILURE;
    }
    
    // WE ARE NOW CONNECT TO SENDER
    int file_d = open(argv[2], O_CREAT | O_WRONLY, S_IRWXU);
    
    char * buf = malloc(512);
    char * decrypt_text = malloc(512);
    
    int bytes_recv = recv(conn, buf, 512, 0);
    int written;
    while(bytes_recv >0)
    {
        do_decrypt(buf, decrypt_text, bytes_recv, key);
        written = write(file_d, decrypt_text, bytes_recv);
        if(written != bytes_recv)
        {
            fprintf(stderr, "write error\n");
            return EXIT_FAILURE;
        }
        bzero(decrypt_text, 512);
        bytes_recv = recv(conn, buf, 512, 0);
    }
    /*
    close(conn);
    close(fd);
    close(file_d);
    free(buf);
    free(decrypt_text);
    */
    return EXIT_SUCCESS;
}
