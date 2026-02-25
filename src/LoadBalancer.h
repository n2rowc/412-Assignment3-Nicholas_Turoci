#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <vector>
#include <string>

#include "RequestQueue.h"
#include "WebServer.h"
#include "IPBlocker.h"

struct LoadBalancerStats {
    int current_time{};
    int total_requests_generated{};
    int total_requests_processed{};
    int total_requests_blocked{};
    int peak_queue_size{};
    int servers_added{};
    int servers_removed{};
};

class LoadBalancer {
public:
    LoadBalancer(int initialServers,
                 int minQueueMultiplier = 50,
                 int maxQueueMultiplier = 80,
                 int scaleCooldown      = 10);

    void addServer();
    void removeServer();

    void runCycle();

    void generateInitialQueue();

    LoadBalancerStats getStats() const;
    RequestQueue&     getQueue();

    int getServerCount() const;
    int getCurrentTime() const;

private:
    void    assignRequests();
    void    checkScaling();
    Request generateRandomRequest() const;

private:
    RequestQueue            request_queue_;
    std::vector<WebServer*> servers_;
    IPBlocker               ip_blocker_;

    int current_time_{0};
    int min_queue_multiplier_{50};
    int max_queue_multiplier_{80};
    int scale_cooldown_{10};
    int last_scale_time_{0};

    LoadBalancerStats stats_;
};

#endif

