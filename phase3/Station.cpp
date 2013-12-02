#include "Station.h"
#include <vector>
#include <string>
#include <cstdlib>
using std::vector;
using std::string;
using std::stoi;

Station::Station( vector<string>  stationData)
{
    yPos =0 ;
    load = 0;       //initialize station's load with 0
    maxLoad = 0;
    //TODO:Get station info from a line of station data
    timeSpan = stoi(stationData[3]);
    id = stoi(stationData[0]);
    switch(id)
    {
        case 1: capacity = 40;
                 break;
        case 20: 
        case 40: capacity = 4;
                 break;
        case 5: 
        case 10: 
        case 21: 
        case 39: 
        case 50: 
        case 55: capacity = 3;
                 break;
        default: capacity = 2;
                 break;
    }

}

int Station::getID() const { return id;}
int Station::getTimeSpan() const {return timeSpan;}
int Station::getCapacity() const{ return capacity;}
int Station::getLoad() const { return load;}
int Station::getyPos() const {return yPos;}
int Station::getWidth() const {return width;}
void Station::setWidth(int w) { width = w;}
void Station::setyPos(int y) {yPos = y;}

void Station::addLoad() 
{load++; 
    if(maxLoad <= load) 
        maxLoad = load;
}         //Add a trian to station

void Station::removeLoad() {load--;}      //Remove a train from station
bool Station::isOverCapacity() { 
    return (load > capacity);
}

