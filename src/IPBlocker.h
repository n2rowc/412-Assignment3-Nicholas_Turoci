#ifndef IPBLOCKER_H
#define IPBLOCKER_H

#include <string>
#include <vector>

struct IPRange {
    std::string cidr;
};

class IPBlocker {
public:
    void addBlockedRange(const IPRange& range);

    void loadFromConfig(const std::string& filename);

    bool isBlocked(const std::string& ip) const;

private:
    std::vector<IPRange> blocked_ranges_;
};

#endif


