#ifndef UDP_H
#define UDP_H

#define DEFAULT_BINDING_PORT (2055)

extern struct in_addr sender_address;
extern unsigned short sender_port;
extern uint8_t sock_buf[];
extern int bytes;

int init_socket(int bind_port);
int receive();

#endif // UDP_H
