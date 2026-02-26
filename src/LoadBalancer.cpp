/**
 * @file LoadBalancer.cpp
 * @brief Implementation of the LoadBalancer class.
 */

#include "LoadBalancer.h"
#include <cstdlib>
using namespace std;

LoadBalancer::LoadBalancer(int initialServers,
                           int minQueueMultiplier,
                           int maxQueueMultiplier,
                           int scaleCooldown,
                           int minProcessTime,
                           int maxProcessTime,
                           double requestProbability)
    : request_queue_(),
      servers_(),
      ip_blocker_(),
      current_time_{0},
      min_queue_multiplier_{minQueueMultiplier},
      max_queue_multiplier_{maxQueueMultiplier},
      scale_cooldown_{scaleCooldown},
      last_scale_time_{0},
      min_process_time_{minProcessTime},
      max_process_time_{maxProcessTime},
      request_probability_{requestProbability},
      stats_{} {
    for (int i = 0; i < initialServers; i++) {
        servers_.push_back(new WebServer(i));
    }
}

void LoadBalancer::addServer() {
    int queue_size = request_queue_.size();
    int servers_before = static_cast<int>(servers_.size());
    servers_.push_back(new WebServer(servers_.size()));
    servers_added_++;
    int servers_after = static_cast<int>(servers_.size());
    if (log_stream_) {
        int avg_before = servers_before > 0 ? queue_size / servers_before : 0;
        int avg_after  = servers_after > 0 ? queue_size / servers_after : 0;
        *log_stream_ << "[" << log_name_ << "] Server ADDED: servers " << servers_before << " -> " << servers_after
                     << ", queue size=" << queue_size
                     << ", avg requests/server before=" << avg_before << " after=" << avg_after
                     << ", cycle=" << current_time_ << "\n";
    }
    if (log_html_) {
        int avg_before = servers_before > 0 ? queue_size / servers_before : 0;
        int avg_after  = servers_after > 0 ? queue_size / servers_after : 0;
        *log_html_ << "<p><span class=\"lb-name\">[" << log_name_ << "]</span> <span class=\"added\">Server ADDED:</span> "
                   << "servers " << servers_before << " &rarr; " << servers_after
                   << ", queue size=" << queue_size
                   << ", avg requests/server before=" << avg_before << " after=" << avg_after
                   << ", cycle=" << current_time_ << "</p>\n";
    }
}

void LoadBalancer::removeServer() {
    if (!servers_.empty()) {
        int queue_size = request_queue_.size();
        int servers_before = static_cast<int>(servers_.size());
        delete servers_.back();
        servers_.pop_back();
        servers_removed_++;
        int servers_after = static_cast<int>(servers_.size());
        if (log_stream_) {
            int avg_before = servers_before > 0 ? queue_size / servers_before : 0;
            int avg_after  = servers_after > 0 ? queue_size / servers_after : 0;
            *log_stream_ << "[" << log_name_ << "] Server REMOVED: servers " << servers_before << " -> " << servers_after
                         << ", queue size=" << queue_size
                         << ", avg requests/server before=" << avg_before << " after=" << avg_after
                         << ", cycle=" << current_time_ << "\n";
        }
        if (log_html_) {
            int avg_before = servers_before > 0 ? queue_size / servers_before : 0;
            int avg_after  = servers_after > 0 ? queue_size / servers_after : 0;
            *log_html_ << "<p><span class=\"lb-name\">[" << log_name_ << "]</span> <span class=\"removed\">Server REMOVED:</span> "
                       << "servers " << servers_before << " &rarr; " << servers_after
                       << ", queue size=" << queue_size
                       << ", avg requests/server before=" << avg_before << " after=" << avg_after
                       << ", cycle=" << current_time_ << "</p>\n";
        }
    }
}

void LoadBalancer::runCycle(int currentTime) {
    current_time_ = currentTime;
    for (size_t i = 0; i < servers_.size(); i++) {
        if (servers_[i]->processRequest(current_time_)) {
            total_requests_processed_++;
        }
    }
    assignRequests();
    if (scale_cooldown_ > 0 && (current_time_ % scale_cooldown_) == 0) {
        checkScaling();
    }
    if (request_queue_.size() > peak_queue_size_) {
        peak_queue_size_ = request_queue_.size();
    }
}

LoadBalancerStats LoadBalancer::getStats() const {
    LoadBalancerStats s{};
    s.current_time             = current_time_;
    s.total_requests_generated = total_requests_generated_;
    s.total_requests_processed = total_requests_processed_;
    s.total_requests_blocked   = total_requests_blocked_;
    s.peak_queue_size          = peak_queue_size_;
    s.servers_added            = servers_added_;
    s.servers_removed          = servers_removed_;
    return s;
}

RequestQueue& LoadBalancer::getQueue() {
    return request_queue_;
}

IPBlocker& LoadBalancer::getBlocker() {
    return ip_blocker_;
}

int LoadBalancer::getServerCount() const {
    return static_cast<int>(servers_.size());
}

int LoadBalancer::getBusyServerCount() const {
    int busy = 0;
    for (size_t i = 0; i < servers_.size(); i++) {
        if (!servers_[i]->isAvailable()) {
            busy++;
        }
    }
    return busy;
}

int LoadBalancer::getCurrentTime() const {
    return current_time_;
}

void LoadBalancer::setLogStream(std::ostream* os, const std::string& name, std::ostream* htmlOs) {
    log_stream_ = os;
    log_html_   = htmlOs;
    log_name_   = name;
}

void LoadBalancer::assignRequests() {
    for (size_t i = 0; i < servers_.size(); i++) {
        if (!request_queue_.isEmpty() && servers_[i]->isAvailable()) {
            servers_[i]->assignRequest(request_queue_.dequeue(), current_time_);
        }
    }
}

void LoadBalancer::checkScaling() {
    int queue_size   = request_queue_.size();
    int server_count = static_cast<int>(servers_.size());

    if (queue_size > server_count * max_queue_multiplier_) {
        addServer();
        last_scale_time_ = current_time_;
    }
    else if (queue_size < server_count * min_queue_multiplier_ && server_count > 1) {
        removeServer();
        last_scale_time_ = current_time_;
    }
}
