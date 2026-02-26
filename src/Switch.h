/**
 * @file Switch.h
 * @brief Defines the Switch class for routing requests by job type (bonus).
 */

#ifndef SWITCH_H
#define SWITCH_H

#include "LoadBalancer.h"
#include "Request.h"

/**
 * @struct SwitchStats
 * @brief Statistics for how many jobs were routed to each LoadBalancer.
 */
struct SwitchStats {
    /** @brief Count of 'P' (processing) jobs routed. */
    int routed_to_processing{};
    /** @brief Count of 'S' (streaming) jobs routed. */
    int routed_to_streaming{};
};

/**
 * @class Switch
 * @brief Routes requests to different load balancers by job_type ('P' or 'S').
 */
class Switch {
public:
    /**
     * @brief Construct the switch with two load balancers.
     * @param processingLB Handles 'P' (processing) jobs.
     * @param streamingLB Handles 'S' (streaming) jobs.
     * @throws std::invalid_argument if either pointer is null.
     */
    Switch(LoadBalancer* processingLB, LoadBalancer* streamingLB);

    /**
     * @brief Send a single Request to the correct LoadBalancer based on job_type.
     * @param request The request to route ('P' -> processing, 'S' -> streaming).
     */
    void routeRequest(const Request& request);

    /**
     * @brief Advance both managed load balancers by one cycle.
     * @param currentTime The current simulation clock.
     */
    void runCycle(int currentTime);

    /**
     * @brief Get a snapshot of routing statistics.
     * @return SwitchStats with routed_to_processing and routed_to_streaming.
     */
    SwitchStats getStats() const;

private:
    LoadBalancer* processing_lb_;
    LoadBalancer* streaming_lb_;
    SwitchStats   stats_;
};

#endif // SWITCH_H
