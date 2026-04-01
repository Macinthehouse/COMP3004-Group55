#ifndef NOTIFICATIONREPOSITORY_H
#define NOTIFICATIONREPOSITORY_H

#include <string>
#include <vector>

#include "Notification.h"

class NotificationRepository
{
public:
    std::vector<Notification> loadNotificationsForVendor(const std::string& vendorId);

    bool addNotification(const std::string& vendorId,
                         const std::string& message);

    bool clearNotificationsForVendor(const std::string& vendorId);
};

#endif // NOTIFICATIONREPOSITORY_H
