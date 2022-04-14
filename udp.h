#ifndef UDP_H
#define UDP_H

#define DEFAULT_BINDING_PORT (8000)

uint8_t sock_buf[65535];
int bytes;

int init_socket(int bind_port);
int receive();

#endif /* UDP_H */
