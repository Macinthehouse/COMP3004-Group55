QT       += widgets

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
    Vendor.cpp \
    Booking.cpp \
    Notification.cpp \
    MarketDate.cpp \
    Waitlist.cpp

# --------------------------------------------------
# HEADERS
# --------------------------------------------------

HEADERS += \
    BookingController.h \
    WaitlistController.h \
    InMemoryStorageManager.h \
    Vendor.h \
    Booking.h \
    Notification.h \
    MarketDate.h \
    Waitlist.h
