#include "WebServer.h"

WebServer::WebServer(int id)
    : server_id_(id),
      current_request_(),
      has_request_{false},
      start_time_{0},
      requests_processed_{0} {
}

void WebServer::assignRequest(const Request& request, int currentTime) {
    // TODO
}

bool WebServer::processRequest(int currentTime) {
    // TODO
    return false;
}

bool WebServer::isAvailable() const {
    // TODO
    return true;
}

ServerStats WebServer::getStats() const {
    // TODO
    ServerStats stats;
    stats.server_id          = server_id_;
    stats.requests_processed = requests_processed_;
    return stats;
}

