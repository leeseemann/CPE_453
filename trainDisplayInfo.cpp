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

void TrackSegments::setComponentID(int compID){
    QString id = QString::number(compID);
    setText(1, id);
    setTextAlignment(1, Qt::AlignHCenter);
}

void TrackSegments::setStatus(QString status){



    //SET STATUS COLORS FOR TrackSegments************
    if(status=="Active")
    {
        setText(2, status);

        QColor color;
        color.setGreen(255);
        setBackgroundColor(2, color);

    }

    else if(status=="Inactive")
    {
        setText(2, status);

        QColor color;
        color.setRed(255);
        setBackgroundColor(2, color);
    }

    else if(status=="Occupied")
    {
        setText(2, status);

        QColor color;
        color.setRgb(255,255,0);
        setBackgroundColor(2, color);
    }

    else
        setText(2,"Error: Incorrect Status Input");

    setTextAlignment(2, Qt::AlignHCenter);
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
   if(status=="Through")
   {
       setText(2, status);

       QColor color;
       color.setGreen(255);
       setBackgroundColor(2, color);
   }

   else if(status=="Bypass")
   {
       setText(2, status);

       QColor color;
       color.setRed(255);
       setBackgroundColor(2, color);
   }

   else
       setText(2,"Error: Incorrect Status Input");

   setTextAlignment(2, Qt::AlignHCenter);

}


Locomotives::Locomotives(){

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
}


