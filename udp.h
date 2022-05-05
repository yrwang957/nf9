#ifndef UDP_H
#define UDP_H

#define DEFAULT_BINDING_PORT (2055)

extern uint8_t sock_buf[];
extern int bytes;

int init_socket(int bind_port);
int receive();

#endif // UDP_H
