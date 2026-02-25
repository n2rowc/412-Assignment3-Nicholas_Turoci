#ifndef SWITCH_H
#define SWITCH_H

#include "LoadBalancer.h"
#include "Request.h"

struct SwitchStats {
    int routed_to_processing{};
    int routed_to_streaming{};
};

class Switch {
public:
    Switch(LoadBalancer* processingLB, LoadBalancer* streamingLB);

    void routeRequest(const Request& request);

    void runCycle();

    SwitchStats getStats() const;

private:
    LoadBalancer* processing_lb_;
    LoadBalancer* streaming_lb_;
    SwitchStats   stats_;
};

#endif

