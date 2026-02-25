#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <queue>

#include "Request.h"

class RequestQueue {
public:
    RequestQueue();

    void enqueue(const Request& request);
    Request dequeue();

    bool isEmpty() const;
    int  size() const;

private:
    std::queue<Request> queue_;
};

#endif 


