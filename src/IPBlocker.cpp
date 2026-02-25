#include "IPBlocker.h"

using namespace std;

    // Append a blocked range definition 
    // ###.###.#/24" or a single IP only
void IPBlocker::addBlockedRange(const IPRange& range) {
    blocked_ranges_.push_back(range);
}

bool IPBlocker::isBlocked(const string& ip) const {
    for (const auto& range : blocked_ranges_) {
        const string& cidr = range.cidr;
        size_t slashPos = cidr.find('/');

        //no slash = exact ip
        if (slashPos == std::string::npos) {
            if (ip == cidr) {
                return true;
            }
        } else {
            //slash = /24 range
            string baseIp = cidr.substr(0, slashPos);
            string mask   = cidr.substr(slashPos + 1);

            if (mask == "24") {
                // for 24, use first three octets as prefix
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

