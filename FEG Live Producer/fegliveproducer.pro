QT += network gui widgets script
target = fegliveproducer

SOURCES = addlowerthirddialog.cpp \
atemcontrol.cpp \
backgroundprogramselectwidget.cpp \
broadcastmanager.cpp \
casparconnection.cpp \
configurationparser.cpp \
fegliveproducer.cpp \
jsonserializer.cpp \
lowerthird.cpp \
lowerthirdsselectwidget.cpp \
lowerthirdstext.cpp \
lowerthirdstextselectwidget.cpp \
main.cpp \
optionsmenu.cpp \
recordmanager.cpp \
saverecord.cpp \
selectdata.cpp \
songfileparser.cpp \
startdialog.cpp

HEADERS = addlowerthirddialog.h \
atemcontrol.h \
backgroundprogramselectwidget.h \
broadcastmanager.h \
casparconnection.h \
configurationparser.h \
fegliveproducer.h \
jsonserializer.h \
lowerthird.h \
lowerthirdsselectwidget.h \
lowerthirdstext.h \
lowerthirdstextselectwidget.h \
optionsmenu.h \
recordmanager.h \
saverecord.h \
selectdata.h \
songfileparser.h \
startdialog.h \
TimeType.h \
usertypeserializer.h

FORMS = addlowerthirddialog.ui \
fegliveproducer.ui \
optionsmenu.ui \
saverecord.ui \
startdialog.ui


RESOURCES = fegliveproducer.qrc

INCLUDEPATH += ../qatemcontrol
SOURCES += ../qatemcontrol/qatemconnection.cpp
HEADERS += ../qatemcontrol/qatemconnection.h