#include <iostream>
#include <thread>
#include <string.h>
#include <microhttpd.h>
#include "GPIOlib.h"

#define HTTP_PORT 8888

using namespace std;
using namespace GPIO;

int state = 0;

int answer_to_connection(void *cls, struct MHD_Connection *connection,
                         const char *url,
                         const char *method, const char *version,
                         const char *upload_data,
                         size_t *upload_data_size, void **con_cls)
{
    printf("%s %s %s\n", version, method, url);

    if (strcmp(url, "/forward") == 0)
    {
        state = 1;
    }
    else if (strcmp(url, "/reverse") == 0)
    {
        state = 2;
    }
    else if (strcmp(url, "/left") == 0)
    {
        state = 3;
    }
    else if (strcmp(url, "/right") == 0)
    {
        state = 4;
    }
    else if (strcmp(url, "/stop") == 0)
    {
        state = 0;
    }

    const char *page = "<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"></head><body><div style=\"font-size:48px;\"><a href=\"/forward\">Forward</a><br><a href=\"/reverse\">Reverse</a><br><a href=\"/left\">Left</a><br><a href=\"/right\">Right</a><br><a href=\"/stop\">Stop</a></div></body></html>";
    struct MHD_Response *response;
    int ret;
    response = MHD_create_response_from_buffer(strlen(page), (void *)page, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

void looper()
{
    while (1) {
        switch (state) {
            case 0:
                cout << "Stop" << endl;
                init();
                break;
            case 1:
                cout << "Forward" << endl;
                controlLeft(FORWARD, 50);
                controlRight(FORWARD, 50);
                break;
            case 2:
                cout << "Reverse" << endl;
                controlLeft(BACKWARD, 50);
                controlRight(BACKWARD, 50);
                break;
            case 3:
                cout << "Left" << endl;
                turnTo(-90);
                break;
            case 4:
                cout << "Right" << endl;
                turnTo(90);
                break;
            default:
                init();
                turnTo(0);
                state = 0;
                break;
        }
        delay(20);
    }
}

int main(int argc, char **argv)
{
    cout << "    ______          _ _   _\n    |  ____|        (_) | (_)\n    | |__  __  _____ _| |_ _ _ __   __ _\n    |  __| \ \/ / __| | __| | '_ \ / _` |\n    | |____ >  < (__| | |_| | | | | (_| |\n    |______/_/\_\___|_|\__|_|_| |_|\__, |\n                                    __/ |\n                                   |___/ " << endl << endl;
    cout << "Init GPIO" << endl;
    init();

    cout << "Starting HTTP Server" << endl;
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, HTTP_PORT, NULL, NULL, &answer_to_connection, NULL, MHD_OPTION_END);
    cout << "HTTP Server started" << endl;

    cout << "Turn to 0" << endl;
    turnTo(0);

    cout << "Reset counter" << endl;
    resetCounter();
    
    looper();
}
