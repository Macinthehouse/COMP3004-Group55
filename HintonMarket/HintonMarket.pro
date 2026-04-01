QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../Booking.cpp \
    ../BookingController.cpp \
    ../ComplianceDocument.cpp \
    ../DashboardController.cpp \
    ../InMemoryStorageManager.cpp \
    ../LoginController.cpp \
    ../MarketDate.cpp \
    ../MarketSchedule.cpp \
    ../MarketOperator.cpp \
    ../Notification.cpp \
    ../NotificationRepository.cpp \
    ../SystemAdministrator.cpp \
    ../User.cpp \
    ../Vendor.cpp \
    ../Waitlist.cpp \
    ../WaitlistController.cpp \
    ../DatabaseManager.cpp \
    ../DatabaseInitializer.cpp \
    ../UserRepository.cpp \
    ../BookingRepository.cpp \
    ../WaitlistRepository.cpp \
    ../MarketScheduleRepository.cpp \
    ../ComplianceDocumentRepository.cpp \
    BookingDialog.cpp \
    MarketScheduleUI.cpp \
    StartupUI.cpp \
    VendorDashboardUI.cpp \
    WaitlistDialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../Booking.h \
    ../BookingController.h \
    ../ComplianceDocument.h \
    ../DashboardController.h \
    ../InMemoryStorageManager.h \
    ../LoginController.h \
    ../MarketDate.h \
    ../MarketSchedule.h \
    ../MarketOperator.h \
    ../Notification.h \
    ../NotificationRepository.h \
    ../SystemAdministrator.h \
    ../User.h \
    ../Vendor.h \
    ../VendorCategory.h \
    ../Waitlist.h \
    ../WaitlistController.h \
    ../DatabaseManager.h \
    ../DatabaseInitializer.h \
    ../UserRepository.h \
    ../BookingRepository.h \
    ../WaitlistRepository.h \
    ../MarketScheduleRepository.h \
    ../ComplianceDocumentRepository.h \
    BookingDialog.h \
    MarketScheduleUI.h \
    StartupUI.h \
    VendorDashboardUI.h \
    WaitlistDialog.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
