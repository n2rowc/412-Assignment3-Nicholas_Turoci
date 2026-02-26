#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <vector>
#include <string>

#include "RequestQueue.h"
#include "WebServer.h"
#include "IPBlocker.h"

// Aggregate statistics describing one LoadBalancer run.
struct LoadBalancerStats {
    int current_time{};             // simulation time in clock cycles
    int total_requests_generated{}; // how many requests were created
    int total_requests_processed{}; // how many finished successfully
    int total_requests_blocked{};   // how many were rejected by IPBlocker
    int peak_queue_size{};          // largest queue length observed
    int servers_added{};            // number of times we scaled up
    int servers_removed{};          // number of times we scaled down
};

// Manages the request queue, web server pool, and scaling behavior.
class LoadBalancer {
public:
    // Configure initial server count, scaling thresholds, and request-generation config.
    LoadBalancer(int initialServers,
                 int minQueueMultiplier = 50,
                 int maxQueueMultiplier = 80,
                 int scaleCooldown      = 10,
                 int minProcessTime     = 5,
                 int maxProcessTime     = 50,
                 double requestProbability = 0.05);

    // Add or remove a single web server from the pool.
    void addServer();
    void removeServer();

    // Execute one simulation cycle (update servers, queue, and scaling).
    void runCycle();

    // Fill the queue with an initial batch of requests (servers * 100).
    void generateInitialQueue();

    // Access simulation statistics, queue, and IP blocker.
    LoadBalancerStats getStats() const;
    RequestQueue&     getQueue();
    IPBlocker&        getBlocker();

    int getServerCount() const;
    int getBusyServerCount() const;
    int getCurrentTime() const;

private:
    // Helpers used internally by runCycle().
    void    assignRequests();          // give queued work to available servers
    void    checkScaling();           // decide whether to add/remove servers
    Request generateRandomRequest() const; // build a new random Request

private:
    RequestQueue            request_queue_;
    std::vector<WebServer*> servers_;
    IPBlocker               ip_blocker_;

    int current_time_{0};
    int min_queue_multiplier_{50};
    int max_queue_multiplier_{80};
    int scale_cooldown_{10};
    int last_scale_time_{0};
    int min_process_time_{5};
    int max_process_time_{50};
    double request_probability_{0.05};

    int total_requests_generated_{0};
    int total_requests_processed_{0};
    int total_requests_blocked_{0};
    int peak_queue_size_{0};
    int servers_added_{0};
    int servers_removed_{0};

    LoadBalancerStats stats_;
};

#endif // LOADBALANCER_H


