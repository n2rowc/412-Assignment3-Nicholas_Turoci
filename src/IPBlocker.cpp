/**
 * @file IPBlocker.cpp
 * @brief Implementation of the IPBlocker class.
 */

#include "IPBlocker.h"

using namespace std;

void IPBlocker::addBlockedRange(const IPRange& range) {
    blocked_ranges_.push_back(range);
}

bool IPBlocker::isBlocked(const string& ip) const {
    for (const auto& range : blocked_ranges_) {
        const string& cidr = range.cidr;
        size_t slashPos = cidr.find('/');

        if (slashPos == std::string::npos) {
            if (ip == cidr) {
                return true;
            }
        } else {
            string baseIp = cidr.substr(0, slashPos);
            string mask   = cidr.substr(slashPos + 1);

            if (mask == "24") {
                size_t lastDot = baseIp.rfind('.');
                if (lastDot != string::npos) {
                    string prefix = baseIp.substr(0, lastDot + 1); 
                    if (ip.rfind(prefix, 0) == 0) { 
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
