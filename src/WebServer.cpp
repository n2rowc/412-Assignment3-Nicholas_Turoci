/**
 * @file WebServer.cpp
 * @brief Implementation of the WebServer class.
 */

#include "WebServer.h"

WebServer::WebServer(int id)
    : server_id_(id),
      current_request_(),
      has_request_{false},
      start_time_{0},
      requests_processed_{0} {
}

void WebServer::assignRequest(const Request& request, int currentTime) {
    current_request_ = request;
    start_time_ = currentTime;
    has_request_ = true;
}

bool WebServer::processRequest(int currentTime) {
    if (!has_request_) {
        return false;
    }
    int elapsed = currentTime - start_time_;
    if (elapsed >= current_request_.processing_time) {
        requests_processed_++;
        has_request_ = false;
        total_busy_time_ += elapsed;
        return true;
    }

    return false; 
}

bool WebServer::isAvailable() const {
    return !has_request_; 
}

ServerStats WebServer::getStats() const {
    ServerStats stats;
    stats.server_id          = server_id_;
    stats.requests_processed = requests_processed_;
    stats.total_busy_time    = total_busy_time_;
    return stats;
}
