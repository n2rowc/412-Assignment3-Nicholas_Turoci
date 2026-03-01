/**
 * @file main.cpp
 * @brief Entry point and simulation driver for the Load Balancer project.
 *        Reads config, creates load balancers and switch, runs the simulation, writes logs.
 */

#include "LoadBalancer.h"
#include "Switch.h"
#include "Request.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

/**
 * @brief Generate a random Request with random IPs, processing time, and job type.
 * @param currentTime Simulation clock for arrival_time.
 * @param minProcessTime Minimum processing time (inclusive).
 * @param maxProcessTime Maximum processing time (inclusive).
 * @return A new Request with random fields.
 */
static Request generateRandomRequest(int currentTime, int minProcessTime, int maxProcessTime) {
    Request r;
    r.ip_in = "192.168." + to_string(rand() % 256) + "." + to_string(rand() % 256);
    r.ip_out = "10." + to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256);
    int range = maxProcessTime - minProcessTime + 1;
    r.processing_time = minProcessTime + (rand() % range);
    r.job_type = (rand() % 2 == 0) ? 'P' : 'S';
    r.arrival_time = currentTime;
    return r;
}

/**
 * @brief Main entry: load config, create LBs and switch, run simulation, write text and HTML logs.
 * @return 0 on success, 1 on config or log file open error.
 */
int main() {
    int    initial_servers      = 0;
    int    simulation_time      = 0;
    int    min_process_time     = 0;
    int    max_process_time     = 0;
    double request_probability  = 0.0;
    int    min_queue_multiplier = 0;
    int    max_queue_multiplier = 0;
    int    scale_cooldown       = 0;
    vector<string> blocked_ranges;
    string          log_file;

    srand(static_cast<unsigned int>(time(nullptr)));
    //open config file
    ifstream config_file("lb_config4.txt");
    if (!config_file.is_open()) {
        cerr << "Error: could not open config file" << endl;
        return 1;
    }
    //take values from config file
    string config_line;
    while (getline(config_file, config_line)) {
        istringstream ss(config_line);
        string key, value;
        getline(ss, key, '=');
        getline(ss, value);

        if (key == "INITIAL_SERVERS") {
            initial_servers = stoi(value);
        } 
        else if (key == "TOTAL_CYCLES") {
            simulation_time = stoi(value);
        } 
        else if (key == "MIN_PROCESS_TIME") {
            min_process_time = stoi(value);
        } 
        else if (key == "MAX_PROCESS_TIME") {
            max_process_time = stoi(value);
        } 
        else if (key == "REQUEST_PROBABILITY") {
            request_probability = stod(value);
        } 
        else if (key == "MIN_QUEUE_MULTIPLIER") {
            min_queue_multiplier = stoi(value);
        } 
        else if (key == "MAX_QUEUE_MULTIPLIER") {
            max_queue_multiplier = stoi(value);
        } 
        else if (key == "SCALE_COOLDOWN") {
            scale_cooldown = stoi(value);
        } 
        else if (key == "BLOCKED_IP_RANGES") {
            istringstream ranges_ss(value);
            string range;
            while (getline(ranges_ss, range, ',')) {
                blocked_ranges.push_back(range);
            }
        } 
        else if (key == "LOG_FILE") {
            log_file = value;
        }
    }
    config_file.close();

    //create load balancer for processing and streaming
    LoadBalancer processing_lb(initial_servers, min_queue_multiplier, max_queue_multiplier, scale_cooldown);
    LoadBalancer streaming_lb(initial_servers, min_queue_multiplier, max_queue_multiplier, scale_cooldown);
    
    //create switch to manage the two load balancers
    Switch sw(&processing_lb, &streaming_lb);
    
    //add blocked ranges to the load balancers
    for (const string& cidr : blocked_ranges) {
        IPRange range{ cidr };
        processing_lb.getBlocker().addBlockedRange(range);
        streaming_lb.getBlocker().addBlockedRange(range);
    }

    int total_requests_generated = initial_servers * 100 * 2;
    int total_requests_blocked   = 0;

    //prefill initial queue
    for (int i = 0; i < total_requests_generated; i++) {
        Request r = generateRandomRequest(0, min_process_time, max_process_time);
        sw.routeRequest(r);
    }

    int starting_queue_size = processing_lb.getQueue().size() + streaming_lb.getQueue().size();

    string log_path = log_file.empty() ? "load_balancer.log" : log_file;
    ofstream log_stream(log_path);
    if (!log_stream.is_open()) {
        cerr << "Error: could not open log file " << log_path << endl;
        return 1;
    }

    string html_path = log_path;
    size_t dot = html_path.rfind('.');
    if (dot != string::npos) {
        html_path = html_path.substr(0, dot) + ".html";
    } else {
        html_path += ".html";
    }
    ofstream html_stream(html_path);
    if (!html_stream.is_open()) {
        cerr << "Error: could not open HTML log file " << html_path << endl;
        return 1;
    }

    html_stream << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n<title>Load Balancer Log</title>\n<style>\n"
                << "body { font-family: sans-serif; margin: 1.5em; background: #f8f9fa; }\n"
                << "h1 { color: #0d6efd; border-bottom: 2px solid #0d6efd; padding-bottom: 0.3em; }\n"
                << "h2 { color: #495057; margin-top: 1.2em; }\n"
                << "pre, .block { background: #fff; padding: 0.8em; border-radius: 6px; border: 1px solid #dee2e6; }\n"
                << ".lb-name { color: #0dcaf0; font-weight: bold; }\n"
                << ".added { color: #198754; font-weight: bold; }\n"
                << ".removed { color: #fd7e14; font-weight: bold; }\n"
                << "p { margin: 0.35em 0; }\n"
                << "</style>\n</head>\n<body>\n";
    html_stream << "<h1>Project 3 - Load Balancer Log</h1>\n\n";
    html_stream << "<h2>Simulation configuration</h2>\n<pre>Clock cycles:        " << simulation_time << "\n"
                << "Initial servers:     " << initial_servers << " (per load balancer)\n"
                << "Task time range:     " << min_process_time << " to " << max_process_time << " cycles\n"
                << "Request probability: " << request_probability << "</pre>\n\n";

    log_stream << "========================================\n";
    log_stream << "Project 3 - Load Balancer Log\n";
    log_stream << "========================================\n\n";

    log_stream << "--- Simulation configuration ---\n";
    log_stream << "Clock cycles:        " << simulation_time << "\n";
    log_stream << "Initial servers:     " << initial_servers << " (per load balancer)\n";
    log_stream << "Task time range:     " << min_process_time << " to " << max_process_time << " cycles\n";
    log_stream << "Request probability: " << request_probability << "\n\n";

    log_stream << "--- Basic logs ---\n";
    log_stream << "Starting queue size: " << starting_queue_size << "\n";
    log_stream << "  (processing LB: " << processing_lb.getQueue().size() << ", streaming LB: " << streaming_lb.getQueue().size() << ")\n";
    log_stream << "Task time range:     " << min_process_time << " to " << max_process_time << " cycles\n\n";

    html_stream << "<h2>Basic logs</h2>\n<pre>Starting queue size: " << starting_queue_size << "\n"
                << "  (processing LB: " << processing_lb.getQueue().size() << ", streaming LB: " << streaming_lb.getQueue().size() << ")\n"
                << "Task time range:     " << min_process_time << " to " << max_process_time << " cycles</pre>\n\n";

    log_stream << "--- Scaling events (server add/remove) ---\n";
    html_stream << "<h2>Scaling events (server add/remove)</h2>\n<div class=\"block\">\n";
    processing_lb.setLogStream(&log_stream, "processing", &html_stream);
    streaming_lb.setLogStream(&log_stream, "streaming", &html_stream);

    for (int i = 0; i < simulation_time; i++) {
        if (request_probability > 0.0 && (rand() / static_cast<double>(RAND_MAX)) < request_probability) {
            total_requests_generated++;
            Request r = generateRandomRequest(i, min_process_time, max_process_time);
            if (processing_lb.getBlocker().isBlocked(r.ip_in)) {
                total_requests_blocked++;
            } else {
                sw.routeRequest(r);
            }
        }
        sw.runCycle(i);
    }

    LoadBalancerStats proc_stats = processing_lb.getStats();
    LoadBalancerStats stream_stats = streaming_lb.getStats();
    int ending_queue_size = processing_lb.getQueue().size() + streaming_lb.getQueue().size();

    html_stream << "</div>\n\n";

    log_stream << "--- End status ---\n";
    log_stream << "Ending queue size:    " << ending_queue_size << "\n";
    log_stream << "  (processing LB: " << processing_lb.getQueue().size() << ", streaming LB: " << streaming_lb.getQueue().size() << ")\n";
    log_stream << "Remaining in queue:  " << ending_queue_size << "\n";
    log_stream << "Active servers (busy): " << processing_lb.getBusyServerCount() + streaming_lb.getBusyServerCount() << "\n";
    log_stream << "Idle servers:        " << (processing_lb.getServerCount() - processing_lb.getBusyServerCount()) + (streaming_lb.getServerCount() - streaming_lb.getBusyServerCount()) << "\n";
    log_stream << "Rejected/discarded:   " << total_requests_blocked << " (blocked by IP)\n\n";

    html_stream << "<h2>End status</h2>\n<pre>Ending queue size:    " << ending_queue_size << "\n"
                << "  (processing LB: " << processing_lb.getQueue().size() << ", streaming LB: " << streaming_lb.getQueue().size() << ")\n"
                << "Remaining in queue:  " << ending_queue_size << "\n"
                << "Active servers (busy): " << processing_lb.getBusyServerCount() + streaming_lb.getBusyServerCount() << "\n"
                << "Idle servers:        " << (processing_lb.getServerCount() - processing_lb.getBusyServerCount()) + (streaming_lb.getServerCount() - streaming_lb.getBusyServerCount()) << "\n"
                << "Rejected/discarded:   " << total_requests_blocked << " (blocked by IP)</pre>\n\n";

    log_stream << "--- Additional information ---\n";
    log_stream << "Total requests generated: " << total_requests_generated << "\n";
    log_stream << "Total requests processed: " << proc_stats.total_requests_processed + stream_stats.total_requests_processed << "\n";
    log_stream << "Peak queue size (combined): " << proc_stats.peak_queue_size + stream_stats.peak_queue_size << "\n";
    log_stream << "Servers added (dynamic):   " << proc_stats.servers_added + stream_stats.servers_added << "\n";
    log_stream << "Servers removed (dynamic): " << proc_stats.servers_removed + stream_stats.servers_removed << "\n";
    log_stream << "Final server count:        " << processing_lb.getServerCount() + streaming_lb.getServerCount() << " (total across both LBs)\n";

    html_stream << "<h2>Additional information</h2>\n<pre>Total requests generated: " << total_requests_generated << "\n"
                << "Total requests processed: " << proc_stats.total_requests_processed + stream_stats.total_requests_processed << "\n"
                << "Peak queue size (combined): " << proc_stats.peak_queue_size + stream_stats.peak_queue_size << "\n"
                << "Servers added (dynamic):   " << proc_stats.servers_added + stream_stats.servers_added << "\n"
                << "Servers removed (dynamic): " << proc_stats.servers_removed + stream_stats.servers_removed << "\n"
                << "Final server count:        " << processing_lb.getServerCount() + streaming_lb.getServerCount() << " (total across both LBs)</pre>\n\n";

    log_stream << "\n========================================\n";
    log_stream << "End of log\n";
    log_stream << "========================================\n";

    html_stream << "<p><strong>End of log</strong></p>\n</body>\n</html>\n";

    log_stream.close();
    html_stream.close();

    return 0;
}

