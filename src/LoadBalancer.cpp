#include "LoadBalancer.h"

LoadBalancer::LoadBalancer(int initialServers,
                           int minQueueMultiplier,
                           int maxQueueMultiplier,
                           int scaleCooldown)
    : request_queue_(),
      servers_(),
      ip_blocker_(),
      current_time_{0},
      min_queue_multiplier_{minQueueMultiplier},
      max_queue_multiplier_{maxQueueMultiplier},
      scale_cooldown_{scaleCooldown},
      last_scale_time_{0},
      stats_{} {
    // TODO
}

void LoadBalancer::addServer() {
    // TODO
}

void LoadBalancer::removeServer() {
    // TODO
}

void LoadBalancer::runCycle() {
    // TODO
}

void LoadBalancer::generateInitialQueue() {
    // TODO
}

LoadBalancerStats LoadBalancer::getStats() const {
    // TODO
    return stats_;
}

RequestQueue& LoadBalancer::getQueue() {
    // TODO
    return request_queue_;
}

int LoadBalancer::getServerCount() const {
    // TODO
    return static_cast<int>(servers_.size());
}

int LoadBalancer::getCurrentTime() const {
    // TODO
    return current_time_;
}

void LoadBalancer::assignRequests() {
    // TODO
}

void LoadBalancer::checkScaling() {
    // TODO
}

Request LoadBalancer::generateRandomRequest() const {
    // TODO
    return Request{};
}

