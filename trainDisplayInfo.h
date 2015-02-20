#ifndef TRAINCONTROLS
#define TRAINCONTROLS

#include <QTreeWidget>
#include <QString>

class TrackSegments: public QTreeWidgetItem{

    public:
        TrackSegments();
        void setTrackSegmentNumber(QString segmentNum);
        void setComponentID(int compID);
        void setStatus(QString status);
        int getTrackSegmentNumber();
        int getComponentID();
        QString getStatus();

};

class TrackSwitches: public QTreeWidgetItem{

public:
    TrackSwitches();
    void setTrackSwitchNumber(QString switchNum);
    void setComponentID(int compID);
    void setStatus(QString status);
    int getTrackSwitchNumber();
    QString getComponentID();
    QString getStatus();

};

class Locomotives: public QTreeWidgetItem{

public:
    Locomotives();
    void setLocomotiveNumber(QString locomotiveNum);
    void setComponentID(int compID);
    void setStatus(QString status);
    int getTrackSwitchNumber();
    int getComponentID();
    QString getStatus();


};

#endif // TRAINCONTROLS

