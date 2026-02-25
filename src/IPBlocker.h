#ifndef IPBLOCKER_H
#define IPBLOCKER_H

#include <string>
#include <vector>

// Represents an IP range to block, e.g. \"192.168.1.0/24\".
struct IPRange {
    std::string cidr;
};

// Simple firewall / DOS-prevention component based on IP ranges.
class IPBlocker {
public:
    // Add a new blocked IP range.
    void addBlockedRange(const IPRange& range);

    // True if the given IP should be rejected.
    bool isBlocked(const std::string& ip) const;

private:
    std::vector<IPRange> blocked_ranges_;
};

#endif // IPBLOCKER_H



