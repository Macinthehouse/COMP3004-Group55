QT += core gui widgets

CONFIG   += c++17
CONFIG   -= app_bundle

TEMPLATE = app
TARGET = HintonMarket

# --------------------------------------------------
# SOURCES
# --------------------------------------------------

SOURCES += \
    main.cpp \
    BookingController.cpp \
    WaitlistController.cpp \
    InMemoryStorageManager.cpp \
    User.cpp \
    Vendor.cpp \
    MarketOperator.cpp \
    SystemAdministrator.cpp \
    ComplianceDocument.cpp \
    Booking.cpp \
    Notification.cpp \
    MarketDate.cpp \
    Waitlist.cpp \
    StartupUI.cpp \
    VendorDashboardUI.cpp \
    BookingUI.cpp

# --------------------------------------------------
# HEADERS
# --------------------------------------------------

HEADERS += \
    BookingController.h \
    WaitlistController.h \
    InMemoryStorageManager.h \
    User.h \
    Vendor.h \
    MarketOperator.h \
    SystemAdministrator.h \
    ComplianceDocument.h \
    VendorCategory.h \
    Booking.h \
    Notification.h \
    MarketDate.h \
    Waitlist.h \
    StartupUI.h \
    VendorDashboardUI.h \
    BookingUI.h
