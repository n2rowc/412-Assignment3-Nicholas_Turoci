/**
 * @file IPBlocker.h
 * @brief Firewall / IP-blocking component for the load balancer.
 */

#ifndef IPBLOCKER_H
#define IPBLOCKER_H

#include <string>
#include <vector>

/**
 * @struct IPRange
 * @brief Represents an IP range to block (e.g. "192.168.1.0/24" or a single IP).
 */
struct IPRange {
    /** @brief CIDR string (e.g. "192.168.1.0/24") or exact IP. */
    std::string cidr;
};

/**
 * @class IPBlocker
 * @brief Simple firewall / DOS-prevention component based on IP ranges.
 */
class IPBlocker {
public:
    /**
     * @brief Add a new blocked IP range.
     * @param range The IP range (CIDR or exact IP) to block.
     */
    void addBlockedRange(const IPRange& range);

    /**
     * @brief Check if the given IP should be rejected.
     * @param ip The IP address string to check.
     * @return true if the IP is in a blocked range.
     */
    bool isBlocked(const std::string& ip) const;

private:
    std::vector<IPRange> blocked_ranges_;
};

#endif // IPBLOCKER_H
