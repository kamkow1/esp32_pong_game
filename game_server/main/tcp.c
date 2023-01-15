#include <stdbool.h>
#include <string.h>
#include <sys/param.h>
#include <lwip/netdb.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include "tcp.h"

void tcp_server_task(void *pv_params)
{
    char address_string[ADDR_STRING_LEN];
    int addr_family     = (int) pv_params;
    int ip_protocol     = 0;
    bool keep_alive     = true;
    bool keep_idle      = KEEPALIVE_IDLE;
    int keep_interval   = KEEPALIVE_INTERVAL;
    int keep_count      = KEEPALIVE_COUNT;

    struct sockaddr_storage destination_addr;

    if (addr_family == AF_INET) {
        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in*)&destination_addr;
        dest_addr_ip4->sin_addr.s_addr    = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family         = AF_INET;
        dest_addr_ip4->sin_port           = htons(PORT);
        ip_protocol                       = IPPROTO_IP;
    }
#ifdef CONFIG_IPV6
    else if (addr_family == AF_INET6) {
        struct sockaddr_in6 *dest_addr_ip6 = (struct sockaddr_in6)&destionation_addr;
        bzero(&des_addr_ip6->sin6_addr.un, sizeof(dest_addr_ip6->sin6_addr.un));
        dest_addr_ip6->sin6_family         = AF_INET6;
        dest_addr_ip6->sin6_port           = htons(PORT);
        ip_protocol                        = IPPORTTO_IP6;
    }
#endif

    int listen_socket = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_socket < 0) {
        ESP_LOGE(TCP_TAG, "failed to create socket (errno): %d", errno);
        vTaskDelete(NULL);
        return;
    }

    int opt = 1;
    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

#if defined(CONFIG_IPV4) && defined(CONFIG_IPV6)
    setcokopt(listen_socket, IPPROTO_IPV6, IPV6_V6ONLY, &opt sizeof(opt));
#endif

    ESP_LOGI(TCP_TAG, "successfully created a socket");

    int error = bind(listen_socket, (struct sockaddr*)&destination_addr, sizeof(destination_addr));
    if (error != 0) {
        ESP_LOGE(TCP_TAG, "failed to bind the socket (errno): %d", errno);
        ESP_LOGE(TCP_TAG, "ip proto: %d", addr_family);
        goto clean;
    }

    ESP_LOGI(TCP_TAG, "socket bound to port: %d", PORT);

    error = listen(listen_socket, 1);
    if (error != 0) {
        ESP_LOGE(TCP_TAG, "an error occured during listening (errno): %d", errno);
        goto clean;
    }

    while (true) {
        ESP_LOGI(TCP_TAG, "socket is listening...");

        struct sockaddr_storage source_addr;
        socklen_t addr_len = sizeof(source_addr);
        int socket = accept(listen_socket, (struct sockaddr*)&source_addr, &addr_len);
        if (socket < 0) {
            ESP_LOGE(TCP_TAG, "failed to accept connection (errno): %d", errno);
            break;
        }

        setsockopt(socket, SOL_SOCKET,  SO_KEEPALIVE,  &keep_alive,    sizeof(int));
        setsockopt(socket, IPPROTO_TCP, TCP_KEEPIDLE,  &keep_idle,     sizeof(int));
        setsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, &keep_interval, sizeof(int));
        setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT,   &keep_count,    sizeof(int));

        if (source_addr.ss_family == PF_INET) {
            inet_ntoa_r(((struct sockaddr_in*)&source_addr)->sin_addr, address_string, sizeof(address_string) - 1);
        }
#ifndef CONFIG_IPV6
        if (source_addr.ss_family == PF_INET6) {
            inet_ntoa_r(((struct sockaddr_in6*)&source_addr)->sin6_addr, address_string, sizeof(address_string) - 1);
        }
#endif

        ESP_LOGI(TCP_TAG, "socket accepted ip address: %s", address_string);

        int len = 20, size_recv, total_size = 0;
        char rx_buffer[len];
        rx_buffer[len] = '\0';
        while (true) {
            if ((size_recv = recv(socket, rx_buffer, len, 0)) < 0) break;
            total_size += size_recv;
        }
        
        printf("received buffer: \n%s\n", rx_buffer);
        
        shutdown(socket, 0);
        close(socket);
    }

clean:
    close(listen_socket);
    vTaskDelete(NULL);
}

void start_tcp()
{
    xTaskCreate(tcp_server_task, "game_server", 4096, (void*)AF_INET, 5, NULL);
}


