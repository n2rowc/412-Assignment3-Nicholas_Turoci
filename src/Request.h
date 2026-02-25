#ifndef REQUEST_H
#define REQUEST_H

#include <string>

// Represents a single client request in the simulation.
// This is the \"unit of work\" that moves through the system.
struct Request {
    // Source IP address of the client sending the request.
    std::string ip_in;

    // Destination IP address where the response will be sent.
    std::string ip_out;

    // How long this request takes to process (in clock cycles).
    int processing_time;

    // Job type: 'P' for processing, 'S' for streaming.
    char job_type;

    // Clock cycle when this request entered the system.
    int arrival_time;
};

#endif // REQUEST_H



