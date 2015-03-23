#include "trainDisplayInfo.h"
#include <iostream>
#include <QList>
#include <QBrush>
#include <QColor>


TrackSegments::TrackSegments(){

}

//SET COLUMN ITEMS FOR TrackSegments***************************
void TrackSegments::setTrackSegmentNumber(QString segmentNum){
    setText(0, segmentNum);
}

void TrackSegments::setComponentID(QString compID){
    setText(1, compID);
    setTextAlignment(1, Qt::AlignHCenter);
}

void TrackSegments::setStatus(QString status){



    //SET STATUS COLORS FOR TrackSegments************
    if(status=="TRUE")
    {
        setText(2, "Active");

        //set background color status indicator for QTreeWidgetItem
        QColor color;
        color.setGreen(255);
        setBackgroundColor(2, color);

        //set background color status indicator for QGraphicsRectItem
        //color.setAlpha(50); ADD LATER WHEN POSITION IS FIXED
        QBrush trackSegmentGraphicsBrush(color);
        setBrush(trackSegmentGraphicsBrush);

        for(unsigned i = 0; i < _rects.size(); ++i) {
            _rects[i]->setBrush(trackSegmentGraphicsBrush);
        }

    }

     else if( status=="FALSE")
    {
        setText(2, "Inactive");

        //set background color status indicator for QTreeWidgetItem
        QColor color;
        color.setRed(255);
        setBackgroundColor(2, color);

        //set background color status indicator for QGraphicsRectItem
        QBrush trackSegmentGraphicsBrush(color);
        setBrush(trackSegmentGraphicsBrush);

        for(unsigned i = 0; i < _rects.size(); ++i) {
            _rects[i]->setBrush(trackSegmentGraphicsBrush);
        }

    }

    else if(status=="Occupied")
    {
        setText(2, status);

        //set background color status indicator for QTreeWidgetItem
        QColor color;
        color.setRgb(255,255,0);
        setBackgroundColor(2, color);

        //set background color status indicator for QGraphicsRectItem
        QBrush trackSegmentGraphicsBrush(color);
        setBrush(trackSegmentGraphicsBrush);

        for(unsigned i = 0; i < _rects.size(); ++i) {
            _rects[i]->setBrush(trackSegmentGraphicsBrush);
        }
    }

    else
    {
        setText(2,"Error: Incorrect Status Input");
        QColor color;
        color.setBlue(255);
        setBackgroundColor(2, color);
        QBrush trackSegmentGraphicsBrush(color);
        setBrush(trackSegmentGraphicsBrush);

        for(unsigned i = 0; i < _rects.size(); ++i) {
            _rects[i]->setBrush(trackSegmentGraphicsBrush);
        }
    }

    setTextAlignment(2, Qt::AlignHCenter);
}
QString TrackSegments::getComponentID()
{
    return text(1);
}

QString TrackSegments::getStatus()
{
    return text(2);
}

void TrackSegments::addRect(QGraphicsRectItem *rect) {
    _rects.push_back(rect);
}

TrackSwitches::TrackSwitches(){

}

//SET COLUMN ITEMS FOR TrackSwitches***************************
    void TrackSwitches::setTrackSwitchNumber(QString switchNum){
    setText(0, switchNum);
}

void TrackSwitches::setComponentID(int compID){
    QString id = QString::number(compID);
    setText(1, id);
    setTextAlignment(1, Qt::AlignHCenter);
}

void TrackSwitches::setStatus(QString status){


   //SET STATUS COLORS FOR TrackSwitches************
   if(status=="Pass")
   {
       setText(2, status);

       //set background color status indicator for QTreeWidgetItem
       QColor color;
      color.setRgb(200,140,237);
       setBackgroundColor(2, color);

       //set background color status indicator for QGraphicsRectItem
       QBrush trackSwitchGraphicsBrush(color);
       setBrush(trackSwitchGraphicsBrush);



   }

   else if(status=="Bypass")
   {
       setText(2, status);

       //set background color status indicator for QTreeWidgetItem
       QColor color;
        color.setRgb(79,80,82);
       setBackgroundColor(2, color);

       //set background color status indicator for QGraphicsRectItem
       QBrush trackSwitchGraphicsBrush(color);
       setBrush(trackSwitchGraphicsBrush);


   }

   else
       setText(2,"Error: Incorrect Status Input");

   setTextAlignment(2, Qt::AlignHCenter);

}

QString TrackSwitches::getComponentID()
{
    return text(1);
}

QString TrackSwitches::getStatus()
{
    return text(2);
}


//Locomotives::Locomotives(): QGraphicsPixmapItem(QPixmap(":/locoImage.png")) {

//}

Locomotives::Locomotives() {

}

//SET COLUMN ITEMS FOR Locomotives***************************
void Locomotives::setLocomotiveNumber(QString locomotiveNum){
    setText(0, locomotiveNum);
}

void Locomotives::setComponentID(int compID){
    QString id = QString::number(compID);
    setText(1, id);
    setTextAlignment(1, Qt::AlignHCenter);
}

void Locomotives::setStatus(QString status){
     setText(2, status);
     setTextAlignment(2, Qt::AlignHCenter);

      //set QGraphicsPixmapItem
}
QString Locomotives::getComponentID()
{
    return text(1);
}

QString Locomotives::getStatus()
{
    return text(2);
}


