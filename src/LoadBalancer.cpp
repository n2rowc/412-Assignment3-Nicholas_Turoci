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
    servers_.push_back(new WebServer(servers_.size()));
    servers_added_++;
}

void LoadBalancer::removeServer() {
    if (!servers_.empty()) {
        delete servers_.back();
        servers_.pop_back();
        servers_removed_++;
    }
}

void LoadBalancer::runCycle() {
    current_time_++;
    for (size_t i = 0; i < servers_.size(); i++) {
        if (servers_[i]->processRequest(current_time_)) {
            total_requests_processed_++;
        }
    }
    if (request_probability_ > 0.0 && (rand() / static_cast<double>(RAND_MAX)) < request_probability_) {
        Request r = generateRandomRequest();
        if (!ip_blocker_.isBlocked(r.ip_in)) {
            request_queue_.enqueue(r);
            total_requests_generated_++;
        } else {
            total_requests_blocked_++;
        }
    }
    assignRequests();
    checkScaling();
    if (request_queue_.size() > static_cast<size_t>(peak_queue_size_)) {
        peak_queue_size_ = static_cast<int>(request_queue_.size());
    }
}

void LoadBalancer::generateInitialQueue() {
    for (int i = 0; i < servers_.size() * 100; i++) {
        request_queue_.enqueue(generateRandomRequest());
    }
}

LoadBalancerStats LoadBalancer::getStats() const {
    stats_.current_time = current_time_;
    stats_.total_requests_generated = total_requests_generated_;
    stats_.total_requests_processed = total_requests_processed_;
    stats_.total_requests_blocked = total_requests_blocked_;
    stats_.peak_queue_size = peak_queue_size_;
    stats_.servers_added = servers_added_;
    stats_.servers_removed = servers_removed_;
    return stats_;
}

RequestQueue& LoadBalancer::getQueue() {
    RequestQueue& request_queue = request_queue_;
    return request_queue_;
}

IPBlocker& LoadBalancer::getBlocker() {
    return ip_blocker_;
}

int LoadBalancer::getServerCount() const {
    return static_cast<int>(servers_.size());
}

int LoadBalancer::getCurrentTime() const {
    return current_time_;
}

void LoadBalancer::assignRequests() {
    for (int i = 0; i < servers_.size(); i++) {
        if (!request_queue_.isEmpty() && servers_[i]->isAvailable()) {
            servers_[i]->assignRequest(request_queue_.dequeue(), current_time_);
        }
    }
}

void LoadBalancer::checkScaling() {
    if (request_queue_.size() < servers_.size() * min_queue_multiplier_) {
        addServer();
    }
    else if (request_queue_.size() > servers_.size() * max_queue_multiplier_) {
        removeServer();
    }
}

Request LoadBalancer::generateRandomRequest() const {
    Request request;
    int ip_c = rand() % 256;
    int ip_d = rand() % 256;
    request.ip_in = "192.168." + to_string(ip_c) + "." + to_string(ip_d);

    int ip2_b = rand() % 256;
    int ip2_c = rand() % 256;
    int ip2_d = rand() % 256;
    request.ip_out = "10." + to_string(ip2_b) + "." +
                     to_string(ip2_c) + "." + to_string(ip2_d);

    int range = max_process_time_ - min_process_time_ + 1;
    request.processing_time = min_process_time_ + (rand() % range);

    request.job_type = (rand() % 2 == 0) ? 'P' : 'S';
    request.arrival_time = current_time_;

    return request;
}

