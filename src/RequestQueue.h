#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <queue>

#include "Request.h"

// Simple FIFO queue for Request objects.
class RequestQueue {
public:
    // Construct an empty queue.
    RequestQueue();

    // Add a request to the back of the queue.
    void enqueue(const Request& request);

    // Remove and return the request at the front of the queue.
    Request dequeue();

    // True if the queue has no pending requests.
    bool isEmpty() const;

    // Number of pending requests.
    int size() const;

private:
    std::queue<Request> queue_;
};

#endif // REQUESTQUEUE_H



