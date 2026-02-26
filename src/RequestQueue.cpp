/**
 * @file RequestQueue.cpp
 * @brief Implementation of the RequestQueue class.
 */

#include "RequestQueue.h"

RequestQueue::RequestQueue()
    : queue_() {

}

void RequestQueue::enqueue(const Request& request) {
    queue_.push(request);
}

Request RequestQueue::dequeue() {
    Request request = queue_.front();
    queue_.pop();
    return request;
}

bool RequestQueue::isEmpty() const {
    return queue_.empty();
}

int RequestQueue::size() const {
    return static_cast<int>(queue_.size());
}
