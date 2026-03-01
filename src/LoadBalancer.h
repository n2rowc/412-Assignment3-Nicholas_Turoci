/**
 * @file LoadBalancer.h
 * @brief Defines the LoadBalancer class and LoadBalancerStats struct.
 */

#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <vector>
#include <string>
#include <ostream>

#include "RequestQueue.h"
#include "WebServer.h"
#include "IPBlocker.h"

/**
 * @struct LoadBalancerStats
 * @brief Aggregate statistics describing one LoadBalancer run.
 */
struct LoadBalancerStats {
    /** @brief Simulation time in clock cycles. */
    int current_time{};
    /** @brief Total requests created. */
    int total_requests_generated{};
    /** @brief Total requests finished successfully. */
    int total_requests_processed{};
    /** @brief Total requests rejected by IPBlocker. */
    int total_requests_blocked{};
    /** @brief Largest queue length observed. */
    int peak_queue_size{};
    /** @brief Number of times a server was added. */
    int servers_added{};
    /** @brief Number of times a server was removed. */
    int servers_removed{};
};

/**
 * @class LoadBalancer
 * @brief Manages the request queue, web server pool, and scaling behavior.
 */
class LoadBalancer {
public:
    /**
     * @brief Construct the load balancer with configuration.
     * @param initialServers Number of web servers to start with.
     * @param minQueueMultiplier Scale-down threshold (queue per server).
     * @param maxQueueMultiplier Scale-up threshold (queue per server).
     * @param scaleCooldown Cycles between scaling checks.
     */
    LoadBalancer(int initialServers,
                 int minQueueMultiplier = 50,
                 int maxQueueMultiplier = 80,
                 int scaleCooldown      = 10);

    /** @brief Add one web server to the pool. */
    void addServer();
    /** @brief Remove one web server from the pool (if any). */
    void removeServer();

    /**
     * @brief Execute one simulation cycle at the given time.
     * @param currentTime Current simulation clock.
     */
    void runCycle(int currentTime);

    /** @brief Get current statistics. */
    LoadBalancerStats getStats() const;
    /** @brief Get the request queue reference. */
    RequestQueue&     getQueue();
    /** @brief Get the IP blocker reference. */
    IPBlocker&        getBlocker();

    /** @brief Get the number of servers in the pool. */
    int getServerCount() const;
    /** @brief Get the number of servers currently busy. */
    int getBusyServerCount() const;
    /** @brief Get the current simulation time. */
    int getCurrentTime() const;

    /**
     * @brief Set optional log streams for add/remove server events.
     * @param os Stream for plain-text log (or nullptr).
     * @param name Label for this LB in log (e.g. "processing").
     * @param htmlOs Stream for HTML log (or nullptr).
     */
    void setLogStream(std::ostream* os, const std::string& name = "", std::ostream* htmlOs = nullptr);

private:
    void assignRequests();
    void checkScaling();

private:
    RequestQueue            request_queue_;
    std::vector<WebServer*> servers_;
    IPBlocker               ip_blocker_;

    int current_time_{0};
    int min_queue_multiplier_{50};
    int max_queue_multiplier_{80};
    int scale_cooldown_{10};
    int last_scale_time_{0};

    std::ostream* log_stream_{nullptr};
    std::ostream* log_html_{nullptr};
    std::string   log_name_;

    int total_requests_generated_{0};
    int total_requests_processed_{0};
    int total_requests_blocked_{0};
    int peak_queue_size_{0};
    int servers_added_{0};
    int servers_removed_{0};

    LoadBalancerStats stats_;
};

#endif // LOADBALANCER_H
