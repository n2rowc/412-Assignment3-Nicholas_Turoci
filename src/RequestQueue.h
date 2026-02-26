/**
 * @file RequestQueue.h
 * @brief FIFO queue for Request objects.
 */

#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <queue>

#include "Request.h"

/**
 * @class RequestQueue
 * @brief Simple FIFO queue for Request objects.
 */
class RequestQueue {
public:
    /** @brief Construct an empty queue. */
    RequestQueue();

    /**
     * @brief Add a request to the back of the queue.
     * @param request The request to enqueue.
     */
    void enqueue(const Request& request);

    /**
     * @brief Remove and return the request at the front of the queue.
     * @return The request that was at the front.
     */
    Request dequeue();

    /**
     * @brief Check if the queue has no pending requests.
     * @return true if the queue is empty.
     */
    bool isEmpty() const;

    /**
     * @brief Get the number of pending requests.
     * @return The current queue size.
     */
    int size() const;

private:
    std::queue<Request> queue_;
};

#endif // REQUESTQUEUE_H
