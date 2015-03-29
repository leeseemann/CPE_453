#include "trainDisplayInfo.h"
#include <iostream>
#include <QList>
#include <QBrush>
#include <QColor>


TrackSegments::TrackSegments(){

}

//SET COLUMN ITEMS FOR TrackSegments***************************
void TrackSegments::setTrackSegmentNumber(QString segmentNum){
    setText(0, segmentNum); // set track segment number
}

void TrackSegments::setComponentID(QString compID){
    setText(1, compID); // set track segment id
    setTextAlignment(1, Qt::AlignHCenter);
}

void TrackSegments::setStatus(QString status){



    //SET STATUS COLORS FOR TrackSegments************
    if(status=="1")
    {
        setText(2, "Active"); // set track segment status

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

     else if( status=="0")
    {
        setText(2, "Inactive"); // set track segment status

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

    else // check for invalid status
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
    return text(1); // retrieve track segment id
}

QString TrackSegments::getStatus()
{
    return text(2); // retrieve track segment status
}

void TrackSegments::addRect(QGraphicsRectItem *rect) {
    _rects.push_back(rect);
}

TrackSwitches::TrackSwitches(){

}

//SET COLUMN ITEMS FOR TrackSwitches***************************
    void TrackSwitches::setTrackSwitchNumber(QString switchNum){
    setText(0, switchNum); // set track switch number
}

void TrackSwitches::setComponentID(int compID){
    QString id = QString::number(compID);
    setText(1, id); // set track switch id
    setTextAlignment(1, Qt::AlignHCenter);
}

void TrackSwitches::setStatus(QString status){


   //SET STATUS COLORS FOR TrackSwitches************
   if(status=="Pass")
   {
       setText(2, status); // set track switch status

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
       setText(2, status); // set track switch status

       //set background color status indicator for QTreeWidgetItem
       QColor color;
        color.setRgb(79,80,82);
       setBackgroundColor(2, color);

       //set background color status indicator for QGraphicsRectItem
       QBrush trackSwitchGraphicsBrush(color);
       setBrush(trackSwitchGraphicsBrush);


   }

   else // check for invalid status
       setText(2,"Error: Incorrect Status Input");

   setTextAlignment(2, Qt::AlignHCenter);

}

QString TrackSwitches::getComponentID()
{
    return text(1); // retrieve switch id
}

QString TrackSwitches::getStatus()
{
    return text(2); // retreive switch status
}


//Locomotives::Locomotives(): QGraphicsPixmapItem(QPixmap(":/locoImage.png")) {

//}

Locomotives::Locomotives() {

}

//SET COLUMN ITEMS FOR Locomotives***************************
void Locomotives::setLocomotiveNumber(QString locomotiveNum){
    setText(0, locomotiveNum); // set train number
}

void Locomotives::setComponentID(int compID){
    QString id = QString::number(compID);
    setText(1, id); // set train id
    setTextAlignment(1, Qt::AlignHCenter);
}

void Locomotives::setStatus(QString status){
     setText(2, status); // set train status
     setTextAlignment(2, Qt::AlignHCenter);

      //set QGraphicsPixmapItem
}
QString Locomotives::getComponentID()
{
    return text(1); // retrieve train id
}

QString Locomotives::getStatus()
{
    return text(2); // retrieve train status
}


