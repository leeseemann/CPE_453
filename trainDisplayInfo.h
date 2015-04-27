#ifndef TRAINCONTROLS
#define TRAINCONTROLS

#include <QTreeWidget>
#include <QString>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>

class TrackSegments: public QTreeWidgetItem, public QGraphicsRectItem {
private:
    QVector<QGraphicsRectItem*> _rects;
      bool PathFlag;
    public:
        TrackSegments();
        void setTrackSegmentNumber(QString segmentNum);
        void setComponentID(QString compID);
        void setStatus(QString status);
        void setPath(int pathID);
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
    void setStatus(/*QString*/ int status);
    int getTrackSwitchNumber();
    QString getComponentID();
    int getStatus();

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

