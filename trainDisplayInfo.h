#ifndef TRAINCONTROLS
#define TRAINCONTROLS

#include <QTreeWidget>
#include <QString>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>

class TrackSegments: public QTreeWidgetItem, public QGraphicsRectItem {
private:
    QVector<QGraphicsRectItem*> _rects;
    public:
        TrackSegments();
        void setTrackSegmentNumber(QString segmentNum);
        void setComponentID(QString compID);
        void setStatus(QString status);
        int getTrackSegmentNumber();
        QString getComponentID();
        QString getStatus();
        void addRect(QGraphicsRectItem* rect);

};

class TrackSwitches: public QTreeWidgetItem, public QGraphicsRectItem{

public:
    TrackSwitches();
    void setTrackSwitchNumber(QString switchNum);
    void setComponentID(int compID);
    void setStatus(QString status);
    int getTrackSwitchNumber();
    QString getComponentID();
    QString getStatus();

};

class Locomotives: public QTreeWidgetItem, public QGraphicsRectItem{

public:
    Locomotives();
    void setLocomotiveNumber(QString locomotiveNum);
    void setComponentID(int compID);
    void setStatus(QString status);
    int getTrackSwitchNumber();
    QString getComponentID();
    QString getStatus();


};

#endif // TRAINCONTROLS

