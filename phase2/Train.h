#ifndef TRAIN_H
#define TRAIN_H

//Properties of a train
#include <vector>
#include <string>
#include "Station.h"
using namespace std;

class Train
{
private:
    int id;                             //ID of the train
    vector<int> departureTime;         //Departure time
    vector<int> arrivalTime;           //ArrivalTime
    vector<int> pDepartureTime;        //Planed Departure time
    vector<int> pArrivalTime;          //Planed Arrival time
    bool headingWest;                   //Direction
    int origin;
    int destination;
    vector<float> color;
public:
    Train(vector<string> trainData, vector<Station> stationData);

    int getID() const;
    int getDepartureTime(int station) const;
    int getArrivalTime(int station) const;
    int getPDepartureTime(int station) const;
    int getPArrivalTime(int station) const;
    bool isWestbound() const;
    int getOrigin() const;
    int getDestination() const;

    vector<float> getColor() const;

    void setColor(float r, float g, float b);

    //This function is deprecated
    //Check if collision with another train from different direction, return the segment where collision happened
    int collision(Train *t) const;           

    //Check if this train collision with another train in segment 
    bool collision(Train *t, int segment) const;           

    bool rearEnd(Train *t, int segment) const;

    int rearEnd(Train *t) const;            //Check if collision with another train in the same direction

    void setDepartureTime(int station, int value, vector<Station> stationData);
    void setArrivalTime(int station, int value);
  

};

class StationType
{
public:
    bool isDepartureTime;
    int time;
    bool operator < (const StationType &other) const;
    
};


        

        

#endif
