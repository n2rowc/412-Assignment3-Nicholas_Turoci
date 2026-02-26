/**
 * @file Request.h
 * @brief Defines the Request struct used as the unit of work in the simulation.
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @struct Request
 * @brief Represents a single client request in the simulation.
 *        This is the unit of work that moves through the queue and servers.
 */
struct Request {
    /** @brief Source IP address of the client sending the request. */
    std::string ip_in;

    /** @brief Destination IP address where the response will be sent. */
    std::string ip_out;

    /** @brief How long this request takes to process (in clock cycles). */
    int processing_time;

    /** @brief Job type: 'P' for processing, 'S' for streaming. */
    char job_type;

    /** @brief Clock cycle when this request entered the system. */
    int arrival_time;
};

#endif // REQUEST_H
