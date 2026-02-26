/**
 * @file WebServer.h
 * @brief Defines the WebServer class and ServerStats struct.
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @struct ServerStats
 * @brief Summary statistics for a single web server.
 */
struct ServerStats {
    /** @brief Identifier for this server. */
    int server_id{};
    /** @brief Number of requests this server has completed. */
    int requests_processed{};
    /** @brief Total time spent working (clock cycles). */
    int total_busy_time{};
};

/**
 * @class WebServer
 * @brief Represents one web server that processes at most one Request at a time.
 */
class WebServer {
public:
    /**
     * @brief Construct a server with a given numeric id.
     * @param id Unique identifier for this server.
     */
    explicit WebServer(int id);

    /**
     * @brief Give this server a new request to work on.
     * @param request The request to process.
     * @param currentTime The global simulation clock when the job is assigned.
     */
    void assignRequest(const Request& request, int currentTime);

    /**
     * @brief Advance this server by one cycle at currentTime.
     * @param currentTime The current simulation clock.
     * @return true if the current request finished this cycle.
     */
    bool processRequest(int currentTime);

    /**
     * @brief Check if the server is idle and ready for another request.
     * @return true if the server has no current request.
     */
    bool isAvailable() const;

    /**
     * @brief Get a snapshot of this server's statistics.
     * @return ServerStats with server_id, requests_processed, total_busy_time.
     */
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
