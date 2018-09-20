// TOY DES Cryptography and Network Security 1 HW1 Josh Barthelmess

// This file is the "sender" who will send the message

#include "HW1.h"
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<strings.h>
#include<errno.h>

void do_encrypt(char* plain, char* dest, int num_bytes, int key)
{
    int i = 0;
    while(i < num_bytes)
    {
        int plain_text = plain[i] & 255;
        int result = encrypt(plain_text, key);
        dest[i] = result & 255;
        i++;
    }
}

int main(int argc, char** argv)
{
    if(argc != 5)
    {
        fprintf(stderr, "USAGE: ./a.out <KEY> <FILE_PATH> <IP_ADDRESS> <PORT>\n");
        return EXIT_FAILURE;
    }
    
    int key = atoi(argv[1]);
    
    struct sockaddr_in server_info;
    if(inet_pton(AF_INET, argv[3], &server_info.sin_addr) <= 0)
    {
        fprintf(stderr, "IP Address error\n");
        return EXIT_FAILURE;
    }
    
    short port = atoi(argv[4]);
    server_info.sin_port = htons(port);
    server_info.sin_family = AF_INET;
    
    int serv_loc = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(serv_loc, (struct sockaddr*) &server_info, sizeof(server_info)) < 0)
    {
        perror("connection error");
        return EXIT_FAILURE;
    }
    
    // We are now connected to the reciever, just need to encrypt the file and send it's contents
    int fd = open(argv[2], O_RDONLY);
    if(fd < 0)
    {
        perror("File error");
        return EXIT_FAILURE;
    }
    
    fflush( stdout);
    
    char* buf = malloc(512);
    char* encrypt_text = malloc(512);
    bzero(encrypt_text, 512);
    int bytes_read;
    int check;
    while((bytes_read = read(fd, buf, 512)) != 0)
    {
        do_encrypt(buf, encrypt_text, bytes_read, key);
        check = send(serv_loc, encrypt_text, bytes_read, 0);
        if(check != bytes_read)
        {
            fprintf(stderr, "Send error\n");
            return EXIT_FAILURE;
        }
        bzero(encrypt_text, 512);
    }
    
    //free(buf);
    //free(encrypt_text);
    //close(fd);
    //close(serv_loc);
    return EXIT_SUCCESS;
}


