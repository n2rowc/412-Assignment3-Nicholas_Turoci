#include "Switch.h"
#include <stdexcept>

Switch::Switch(LoadBalancer* processingLB, LoadBalancer* streamingLB)
    : processing_lb_(processingLB),
      streaming_lb_(streamingLB),
      stats_{} {
    if (processingLB == nullptr || streamingLB == nullptr) {
        throw std::invalid_argument("LoadBalancers cannot be null");
    }
}

void Switch::routeRequest(const Request& request) {
    if (request.job_type == 'P') {
        processing_lb_->getQueue().enqueue(request);
        stats_.routed_to_processing++;
    }
    else if (request.job_type == 'S') {
        streaming_lb_->getQueue().enqueue(request);
        stats_.routed_to_streaming++;
    }
}

void Switch::runCycle() {
    processing_lb_->runCycle();
    streaming_lb_->runCycle();
}

SwitchStats Switch::getStats() const {
    SwitchStats stats = stats_;
    return stats;
}

