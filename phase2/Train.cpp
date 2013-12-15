

#include "Train.h"
#include <string>
#include <cstdlib>
#include "Station.h"
#include "splitstring.h"
#include <algorithm>
using std::string;
using std::stoi;

Train::Train(vector<string> trainData, vector<Station> stationData)
{
    //TODO: 
    // 1. Read parse data
    // 2. Set initial time according to station data
    id = stoi(trainData[0]);
    departureTime.assign(61, -1);
    arrivalTime.assign(61, -1);
    pDepartureTime.assign(61, -1);
    pArrivalTime.assign(61, -1);

    //parse origin
    splitstring depStr(trainData[3]);
    vector<string> depVec = depStr.split(' ');
    origin = stoi(depVec[1]);

    //parse destination
    splitstring desStr(trainData[4]);
    vector<string> desVec = desStr.split(' ');
    destination = stoi(desVec[1]);

    //parse direction
    if (!trainData[2].compare("E"))
        headingWest = false;
    else
        headingWest = true;

    //Parse departure time
    splitstring timeString(trainData[1]);
    vector<string> timeVector = timeString.split(':');
    int depT = stoi(timeVector[0]) * 60 + stoi(timeVector[1]); 


    if (headingWest)
    {
        departureTime[origin-1] = pArrivalTime[origin-1] = 
         departureTime[origin-1] = arrivalTime[origin-1] = depT;      //Departure time 
        for (int i = origin-2; i >=destination-1; i--) 
            departureTime[i] = arrivalTime[i] = pDepartureTime[i] = pArrivalTime[i] = stationData[i].getTimeSpan() + arrivalTime[i+1];
    }
    else
    {
        departureTime[origin-1] = pArrivalTime[origin-1] = 
         departureTime[origin-1] = arrivalTime[origin-1] = depT;      //Departure time
        for (int i = origin; i < destination; i++)
            departureTime[i] = arrivalTime[i] = pDepartureTime[i] = pArrivalTime[i] = 
                stationData[i-1].getTimeSpan() + arrivalTime[i-1];
    }
        

    
    //for(int i = origin; i >= destination-1; i--)
    //    departureTime[i] = arrivalTime[i] = pDepartureTime[i] = pArrivalTime[i] = 
    //        stationData[i].getTimeSpan() + arrivalTime[i+1];
    //
    //Set color
    color.push_back(1.0);
    color.push_back(0.0);
    color.push_back(0.0);

        
}

//Code for getting properties of train
//
int Train::getID() const 
{ return id;}

int Train::getDepartureTime(int station) const
{
    return departureTime[station-1];
}

int Train::getArrivalTime(int station) const
{
    return arrivalTime[station-1];
}

int Train::getPArrivalTime(int station) const
{
    return pArrivalTime[station-1];
}

int Train::getPDepartureTime(int station) const
{
    return pDepartureTime[station-1];
}

int Train::getOrigin() const
{
    return origin;
}

int Train::getDestination() const
{
    return destination;
}

bool Train::isWestbound() const
{
    return headingWest;
}

void Train::setDepartureTime(int station, int value, vector<Station> stationData)
{
    //1. Change departure time of current station
    departureTime[station-1] = value;

    //TODO: 
    //2. Update decending points.
    //
    if (headingWest)
    {
        for (int i = station-2; i >=destination-1; i--) 
            departureTime[i] = arrivalTime[i] = stationData[i].getTimeSpan() + departureTime[i+1];
    }
    else  //if eastbound
    {
        for (int i = station; i < destination; i++)
            departureTime[i] = arrivalTime[i] =  stationData[i-1].getTimeSpan() + departureTime[i-1];
    }
 
}

void Train::setArrivalTime(int station, int value)
{
    arrivalTime[station-1] = value;
}

int Train::rearEnd(Train *t) const
{
    //TODO: Looking for a segment which two trains are smaller than safty distance
    if (this->isWestbound())        //This train is west bound
    {
        if (t->isWestbound())   // The other train is also west bound
        {
            for (int i=this->getOrigin(); i > this->getDestination(); i--)
            {
                if (t->getDepartureTime(i) != -1)
                    if(abs(t->getDepartureTime(i) - this->getDepartureTime(i)) < 5)
                            return i-1;
            }
                    
            return 0;
        }
        else
            return 0;
    }
    else //if this is eastbound
    {
        if(!t->isWestbound())
        {
            for (int i=this->getOrigin(); i < this->getDestination(); i++)
            {
                if (t->getDepartureTime(i) != -1)
                    if(abs(t->getDepartureTime(i) - this->getDepartureTime(i)) < 5)
                            return i-1;
            }
            return 0;
        }
        else 
            return 0;
    }

}


bool Train::rearEnd(Train *t, int segment) const
{
    //Detect if this train rear end with train t in segment 
    if (this->isWestbound())        //if west bound
    {
        if(t->isWestbound())
            return (abs(t->getDepartureTime(segment+1) - this->getDepartureTime(segment+1)) < 5);
        else
            return false;
    }
    else        //if east bound
    {
        if(!t->isWestbound())
            return (abs(t->getDepartureTime(segment) - this->getDepartureTime(segment)) < 5);
        else
            return false;
    }


}

int Train::collision(Train *t) const
{
    //Check if two trans are different direction
    //Return -1 if two trains are of same direction
    //Return 0 if no collision detected
    //Return the id of segment if collision is detected
    
    //TODO:Narrow searching interval

    if (this->isWestbound() != t->isWestbound())
    {
        if ((t->getDestination() - this->getDestination()) * (t->getOrigin() - this->getOrigin()) >=0 )             //Tow trains share no same stations
        return 0;

        if(this->isWestbound())     //If this train is westbound
        {
            //TODO: Go along the siding to check for collision
            for (int i = this->getOrigin(); i > this->getDestination(); i--)
            {
                if(t->getDepartureTime(i) == -1 ||
                        t->getDepartureTime(i-1) == -1)       //if not train 
                    continue;

                if( (this->getDepartureTime(i) - t->getArrivalTime(i)) * (this->getArrivalTime(i-1) - t->getDepartureTime(i-1)) < 0)
                        return i-1;
            }

            return 0;
            
        }
        else // if goes eastbound
        {
            for (int i=this->getOrigin(); i < this->getDestination(); i++)
            {
                if (t->getDepartureTime(i) == -1 )//||
                        //t->getDepartureTime(i+1) == -1)
                    continue;

                if ((this->getDepartureTime(i) - t->getArrivalTime(i)) * (this->getArrivalTime(i+1) - t->getDepartureTime(i+1)) < 0 )
                    return i;
            }
                        
            return 0;
        }
    }
    else
        return -1;
        
}

bool Train::collision(Train *t, int segment) const
{

    // detect of this train and another train collision in segemnt

    if(this->getDepartureTime(segment) == -1 ||
            this ->getDepartureTime(segment+1) == -1 ||
            t->getDepartureTime(segment) == -1 ||
            t->getDepartureTime(segment+1) == -1)       //If no trains are in this segment
        return false;

    if (this->isWestbound() && !t->isWestbound())     //different directions
        return ( (this->getArrivalTime(segment) - t->getDepartureTime(segment)) *
                (this->getDepartureTime(segment+1) - t->getArrivalTime(segment+1)) < 0);

    else if (!this->isWestbound() && t->isWestbound())
        return ((this->getDepartureTime(segment) - t->getArrivalTime(segment)) * 
                (this->getArrivalTime(segment+1) - t->getDepartureTime(segment+1)) < 0);



    return false;

}



vector<float> Train::getColor() const { return color; }

void Train::setColor(float r, float g, float b)
{
    color[0] = r; color[1] = g; color[2] = b;
}



bool StationType::operator<(const StationType &other) const
{
    {
        if (this->time != other.time)
            return (this->time < other.time);
        else
            return (!this->isDepartureTime && other.isDepartureTime);  //Put arrival time at first
    }
}
    



