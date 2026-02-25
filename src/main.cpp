#include "LoadBalancer.h"
#include "Switch.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

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
    ifstream config_file("lb_config.txt");
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
    LoadBalancer processing_lb(initial_servers, min_queue_multiplier, max_queue_multiplier, scale_cooldown, min_process_time, max_process_time, request_probability);
    LoadBalancer streaming_lb(initial_servers, min_queue_multiplier, max_queue_multiplier, scale_cooldown, min_process_time, max_process_time, request_probability);
    
    //create switch to manage the two load balancers
    Switch sw(&processing_lb, &streaming_lb);
    
    //add blocked ranges to the load balancers
    for (const string& cidr : blocked_ranges) {
        IPRange range{ cidr };
        processing_lb.getBlocker().addBlockedRange(range);
        streaming_lb.getBlocker().addBlockedRange(range);
    }

    //prefill initial queue
    processing_lb.generateInitialQueue();
    streaming_lb.generateInitialQueue();
    //open log file
    //write starting stats
    
    
    //run simulation
    for (int i = 0; i < simulation_time; i++) {
        sw.runCycle();
    }
    //write ending stats
    //close log file

    //free resources
    return 0;
}

