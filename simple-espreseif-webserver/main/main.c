#include <stdio.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"

static const char *TAG = "ESP32 Server";

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    switch (event_id) {
        case WIFI_EVENT_STA_START:
            printf("WiFi connecting ... \n");
            break;

        case WIFI_EVENT_STA_CONNECTED:
            printf("WiFi connected ... \n");
            break;

        case WIFI_EVENT_STA_DISCONNECTED:
            printf("WiFi lost connection ... \n");
            break;

        case IP_EVENT_STA_GOT_IP:
            printf("WiFi got IP ... \n\n");
            break;

        default:
            break;
    }
}

void wifi_connect(){
      // 1 - Wi-Fi/LwIP Init Phase
    esp_netif_init();                       // TCP/IP initiation
    esp_event_loop_create_default();        // event loop
    esp_netif_create_default_wifi_sta();    // WiFi station
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);

    // 2 - Wi-Fi Configuration Phase
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = "SSID",
            .password = "PASS"
        },
    };
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);

    // 3 - Wi-Fi Start Phase
    esp_wifi_start();

    // 4 - Wi-Fi Connect Phase
    esp_wifi_connect();
}

esp_err_t get_handler(httpd_req_t *req) {
    
    const char resp[] = "<!DOCTYPE html>\
                          <html lang=\"en\">\
                            <head>\
                              <meta charset=\"UTF-8\">\
                              <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
                              <title>Sample Project</title>\
                            </head>\
                            <body>\
                              <h1>Input a String</h1>\
                              <form>\
                                <label for=\"inputString\">Enter a string:</label>\
                                <input type=\"text\" id=\"inputString\" name=\"inputString\">\
                                <button type=\"submit\">Submit</button>\
                              </form>\
                            </body>\
                          </html>";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
esp_err_t get_handler_str(httpd_req_t *req) {
    
    char *buf;
    size_t buf_len;

    // Read the URI line and get the host
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Host: %s", buf);
        }
        free(buf);
    }

    // Read the URI line and get the parameters
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query: %s", buf);
            
            char param[32];
            if (httpd_query_key_value(buf, "str", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "The string value = %s", param);
            }
            if (httpd_query_key_value(buf, "int", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "The int value = %s", param);
            }
        }
        free(buf);
    }

    const char resp[] = "Data was sent";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;

}

// esp_err_t post_handler(httpd_req_t *req) {
    
//     char content[100];
//     size_t recv_size = MIN(req->content_len, sizeof(content));
//     int ret = httpd_req_recv(req, content, recv_size);

//     if (recv_size <= 0) {
//         if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
//             httpd_resp_send_408(req);
//         }
        
//         return ESP_FAIL;

//     }

//     const char resp[] = "URI POST response";
//     httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
//     return ESP_OK;
// }

httpd_uri_t uri_get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_handler,
    .user_ctx = NULL,
};

httpd_uri_t uri_get_input = {
    .uri = "/get",
    .method = HTTP_GET,
    .handler = get_handler_str,
    .user_ctx = NULL,
};

// httpd_uri_t uri_post = {
//     .uri = "/",
//     .method = HTTP_POST,
//     .handler = post_handler,
//     .user_ctx = NULL,
// };

// void server_initiation() {

//     httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();
//     httpd_handle_t server_handle = NULL;

//     httpd_start(&server_handle, &server_config);


//     httpd_register_uri_handler(server_handle, &uri_get);
// }

httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_get_input);
    }

    return server;
}

void stop_webserver(httpd_handle_t server) {
    
    if(server){
      httpd_stop(server);
    }
}


void app_main(void) {
    nvs_flash_init();  
    wifi_connect();   
    start_webserver();

    //stop_webserver(server);
}
