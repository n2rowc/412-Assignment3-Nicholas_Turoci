#ifndef SWITCH_H
#define SWITCH_H

#include "LoadBalancer.h"
#include "Request.h"

// Statistics for how many jobs were routed to each LoadBalancer.
struct SwitchStats {
    int routed_to_processing{}; // 'P' jobs
    int routed_to_streaming{};  // 'S' jobs
};

// Bonus: routes requests to different load balancers by job_type.
class Switch {
public:
    // processingLB handles 'P' jobs, streamingLB handles 'S' jobs.
    Switch(LoadBalancer* processingLB, LoadBalancer* streamingLB);

    // Send a single Request to the correct LoadBalancer based on job_type.
    void routeRequest(const Request& request);

    // Advance both managed load balancers by one cycle.
    void runCycle();

    // Snapshot of routing statistics.
    SwitchStats getStats() const;

private:
    LoadBalancer* processing_lb_;
    LoadBalancer* streaming_lb_;
    SwitchStats   stats_;
};

#endif // SWITCH_H


