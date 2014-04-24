#include <stdio.h>
#include <string.h>     // Needed for memset
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions


int main()
{
    int i; // because fuck C for loops
    struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
    struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.

    // Empty the host_info memory
    memset(&host_info, 0, sizeof host_info);
    
    host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
    host_info.ai_socktype = SOCK_STREAM; // We're using TCP

    // Now fill up the linked list of host_info structs with google's address information.
    int status = getaddrinfo(
                  "vortex.labs.overthewire.org" //addres
                , "5842"                        //port
                , &host_info                  
                , &host_info_list
                );

    if (status != 0) printf("getaddrinfo error %i",gai_strerror(status));

    int socketfd ; // The socket descripter
    socketfd = socket(
              host_info_list->ai_family
            , host_info_list->ai_socktype
            , host_info_list->ai_protocol
            );
    
    if (socketfd == -1) printf("socket error ");
    
    status = connect(
                  socketfd
                , host_info_list->ai_addr
                , host_info_list->ai_addrlen
                );

    if (status == -1) printf("connect error\n");
    
    int n =  4; // The amount of integers to recieve
    unsigned int incoming_data_buffer[n];
    ssize_t bytes_recieved;

    for(i = 0; i < n; i++){

        // Get the next integer and put it into the buffer
        bytes_recieved = recv(
                          socketfd
                        , incoming_data_buffer + i
                        , sizeof (unsigned int)
                        , 0
                        );

        // If no data arrives, the program will just wait here until some data arrives.
        if (bytes_recieved == 0) printf("host shut down.\n");
        if (bytes_recieved == -1) printf("recieve error!\n");
        
    }
    
    printf("Recieved integers");
    unsigned int sum = 0;
    for (i = 0; i < n; i++){
        printf("%u\n",incoming_data_buffer[i]);
        sum += incoming_data_buffer[i];
    }
    printf("Sum: %u\n\n",sum);
    

    unsigned int* msgsum = &sum; //The sum is the message to send back.
    int len = sizeof(msgsum);
    ssize_t bytes_sent;
    bytes_sent = send(
                  socketfd
                , msgsum
                , len
                , 0
                );
    
    char incoming_data_buffer2[1000];
    bytes_recieved = recv(
                          socketfd
                        , incoming_data_buffer2
                        , 1000
                        , 0
                        );
    
    // If no data arrives, the program will just wait here until some data arrives.
    if (bytes_recieved == 0) printf("host shut down.\n");
    if (bytes_recieved == -1) printf("recieve error!\n");
    
    printf(incoming_data_buffer2);
    printf("\n");
    
    freeaddrinfo(host_info_list);
    close(socketfd);

    return 0;
}    
