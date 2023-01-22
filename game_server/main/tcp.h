#ifndef SOCKET_H
#define SOCKET_H

#define PORT                80
#define ADDR_STRING_LEN     128
#define KEEPALIVE_IDLE      5
#define KEEPALIVE_INTERVAL  5
#define KEEPALIVE_COUNT     5
#define RXBUFF_MAX          20
#define TCP_TAG             "esp32_game_server"
#define RX_BUFFER_MAX       50
#define TX_BUFFER_MAX       50

// tcp
void tcp_server_task(void *pv_params);
void retransmit(int socket);
void start_tcp();

// rx buffer
char *read_rx_buffer();

#endif
