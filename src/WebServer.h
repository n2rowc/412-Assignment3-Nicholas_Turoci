#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

struct ServerStats {
    int server_id{};
    int requests_processed{};
    int total_busy_time{};
};

class WebServer {
public:
    explicit WebServer(int id);

    void assignRequest(const Request& request, int currentTime);

    bool processRequest(int currentTime);

    bool isAvailable() const;

    ServerStats getStats() const;

private:
    int     server_id_;
    Request current_request_;
    bool    has_request_{false};
    int     start_time_{0};
    int     requests_processed_{0};
};

#endif 

