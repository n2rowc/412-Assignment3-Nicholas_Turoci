#include "Switch.h"

Switch::Switch(LoadBalancer* processingLB, LoadBalancer* streamingLB)
    : processing_lb_(processingLB),
      streaming_lb_(streamingLB),
      stats_{} {
    // TODO
}

void Switch::routeRequest(const Request& request) {
    // TODO
}

void Switch::runCycle() {
    // TODO
}

SwitchStats Switch::getStats() const {
    // TODO
    return stats_;
}

