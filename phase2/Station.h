#ifndef STATION_H
#define STATION_H

#include <vector>
#include <string>
using std::vector;
using std::string;
//This is also a definition of segments. The segment was defined by its starting station
//Definition of Stations
class Station
{
private:
    int id;
    int timeSpan;          //Train speed during the segment
    int capacity;
    int load;
    int maxLoad;
    int yPos;           //for drawing
    int width;          //width for drawing
public:
    Station( vector<string> stationData);
    Station(int i, int t, int c):id(i), timeSpan(t), capacity(c) {load=0;}
    int getTimeSpan() const;
    int getCapacity() const;
    int getLoad() const;
    int getID() const;

    //functions for drawing
    int getyPos() const;        
    int getWidth() const;

    void setWidth(int w);
    void setyPos(int y);

    void addLoad();         //Add a trian to station
    void removeLoad();      //Remove a train from station

    bool isOverCapacity();    //Check if the station is overcapacity

};

#endif
