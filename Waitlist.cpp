#include "Waitlist.h"

// --------------------------------------------------
// Default Constructor
// --------------------------------------------------
// Required for STL container usage.
// --------------------------------------------------

Waitlist::Waitlist()
    : category(VendorCategory::FOOD)
{
}

// --------------------------------------------------
// Parameterized Constructor
// --------------------------------------------------

Waitlist::Waitlist(const std::string& marketDateId,
                   VendorCategory category)
    : marketDateId(marketDateId),
      category(category)
{
}

// --------------------------------------------------
// enqueue()
// --------------------------------------------------

void Waitlist::enqueue(const std::string& vendorId)
{
    queue.push_back(vendorId);
}

// --------------------------------------------------
// dequeue()
// --------------------------------------------------

std::string Waitlist::dequeue()
{
    if (queue.empty()) {
        return "";
    }

    std::string frontVendor = queue.front();
    queue.erase(queue.begin());

    return frontVendor;
}

// --------------------------------------------------
// remove()
// --------------------------------------------------

void Waitlist::remove(const std::string& vendorId)
{
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        if (*it == vendorId) {
            queue.erase(it);
            break;
        }
    }
}

// --------------------------------------------------
// containsVendor()
// --------------------------------------------------

bool Waitlist::containsVendor(const std::string& vendorId) const
{
    for (const auto& id : queue) {
        if (id == vendorId) {
            return true;
        }
    }
    return false;
}

// --------------------------------------------------
// getPosition()
// --------------------------------------------------

int Waitlist::getPosition(const std::string& vendorId) const
{
    for (size_t i = 0; i < queue.size(); ++i) {
        if (queue[i] == vendorId) {
            return static_cast<int>(i) + 1;  // 1-based index
        }
    }
    return -1;
}

// --------------------------------------------------
// isEmpty()
// --------------------------------------------------

bool Waitlist::isEmpty() const
{
    return queue.empty();
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

std::string Waitlist::getMarketDateId() const
{
    return marketDateId;
}

VendorCategory Waitlist::getCategory() const
{
    return category;
}

std::string Waitlist::peekNextVendorId() const {
    if (queue.empty()) return "";
    return queue.front();
}
std::vector<std::string> Waitlist::getQueueSnapshot() const { return queue; }
