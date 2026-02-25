#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

// Summary statistics for a single web server.
struct ServerStats {
    int server_id{};          // identifier for this server
    int requests_processed{}; // how many requests it has completed
    int total_busy_time{};    // total time spent working (optional to track)
};

// Represents one web server that processes at most one Request at a time.
class WebServer {
public:
    // Construct a server with a given numeric id.
    explicit WebServer(int id);

    // Give this server a new request to work on.
    // currentTime is the global clock when the job is assigned.
    void assignRequest(const Request& request, int currentTime);

    // Advance this server by one cycle at currentTime.
    // Returns true if the current request finished this cycle.
    bool processRequest(int currentTime);

    // True if the server is idle and ready for another request.
    bool isAvailable() const;

    // Snapshot of this server's statistics.
    ServerStats getStats() const;

private:
    int     server_id_;
    Request current_request_;
    bool    has_request_{false};
    int     start_time_{0};
    int     requests_processed_{0};
    int     total_busy_time_{0};
};

#endif // WEBSERVER_H


