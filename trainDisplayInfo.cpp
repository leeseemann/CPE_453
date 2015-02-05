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
}

void TrackSegments::setStatus(QString status){
    setText(2, status);


    //SET STATUS COLORS FOR TrackSegments************
    if(status=="Active")
    {
        QColor color;
        color.setGreen(255);
        setBackgroundColor(2, color);

    }

    else if(status=="Inactive")
    {
        QColor color;
        color.setRed(255);
        setBackgroundColor(2, color);
    }

    else if(status=="Occupied")
    {
        QColor color;
        color.setRgb(255,255,0);
        setBackgroundColor(2, color);
    }

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
}

void TrackSwitches::setStatus(QString status){
   setText(2, status);

   //SET STATUS COLORS FOR TrackSwitches************
   if(status=="Through")
   {
       QColor color;
       color.setGreen(255);
       setBackgroundColor(2, color);
   }

   else if(status=="Bypass")
   {
       QColor color;
       color.setRed(255);
       setBackgroundColor(2, color);
   }

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
}

void Locomotives::setStatus(QString status){
     setText(2, status);
}


