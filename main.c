#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include "mongoose.h"

#define CONNECTION_SSE_LOCATION MG_DATA_SIZE - 1
#define CONNECTION_SSE_MAGIC    0x69

void ev_handler(struct mg_connection* c, int ev, void* ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;

        if (mg_match(hm->uri, mg_str("/sse"), NULL)) {
            // intiate sse
            const char* headers =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/event-stream\n"
                "Cache-Control: no-cache\n"
                "Connection: keep-alive\r\n\r\n";

            mg_printf(c, headers);
            c->recv.len = 0;

            // mark connection as initialised for sse
            c->data[CONNECTION_SSE_LOCATION] = CONNECTION_SSE_MAGIC;        
        }
        else {
            struct mg_http_serve_opts opts = { .root_dir = "./web/" };
            mg_http_serve_dir(c, hm, &opts);
        }

        return;
    }
}

void sse_keepalive(void* data) {
    struct mg_connection* c;
    for (c = ((struct mg_mgr*)data)->conns; c->next != NULL; c = c->next) {
        if (c->data[CONNECTION_SSE_LOCATION] != CONNECTION_SSE_MAGIC) return;
        mg_printf(c, ":sse keep-alive\n");
        c->recv.len = 0;
    }
}

#define SERVER_HOST_URL        "http://0.0.0.0:8000"
#define HTTP_POLL_FREQ_MS      1000
#define KEEPALIVE_POLL_FREQ_MS 5000
// KEEPALIVE_POLL_FREQ_MS must be >= HTTP_POLL_FREQ_MS

int main() {
    struct mg_mgr mgr;
    
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, SERVER_HOST_URL, ev_handler, NULL);
    mg_timer_add(&mgr, KEEPALIVE_POLL_FREQ_MS, MG_TIMER_REPEAT, sse_keepalive, (void*)&mgr);

    for (;;)
    {
        mg_mgr_poll(&mgr, HTTP_POLL_FREQ_MS);
    }
    
    return 0;
}