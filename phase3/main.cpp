#include "Train.h"
#include "Station.h"
#include <fstream>
#include <string>
#include <vector>
#include "splitstring.h"
#include <iostream>
#include <GL/freeglut.h>
#include <algorithm>
#include "tools.h"
using namespace std;

#define MAX_TOKENS_PER_LINE 10
#define NUM_OF_TRAINS 50
#define NUM_OF_STATIONS 60
#define DEBUG
const string DELIMITER = ",";

vector<Train> trains;           //set of trains
vector<Train> testTrains;           //set of trains for testing scheduling
vector<Station> stations;       //set of stations


//OpenGL related functions and vars

//Window parameters
//
int height = 800; 
int width = 1024;

int NumTrainToDraw = 0;

int maxTime = 60*20*4;                //Track the time of last arrival

bool isOverLoad(int t, int station)
{
    //TODO: test if it is over capacity at station
    vector<StationType> stops;

    for ( int i = 0; i <= t; i++)                //Put all of the train time into the vector
    {
        if ( trains[i].getDepartureTime(station) != - 1 && trains[i].getOrigin() != station)        //The train do not arrive this station and the station is not the trains origin point
        {
            StationType departure;
            StationType arrival;
            departure.time = trains[i].getDepartureTime(station);
            departure.isDepartureTime = true;
            arrival.time = trains[i].getArrivalTime(station);
            arrival.isDepartureTime = false;
            stops.push_back(departure);
            stops.push_back(arrival);
        }
    }

    sort(stops.begin(), stops.end());       //Sort the time

//#ifdef DEBUG
//    for ( int i=0; i < stops.size(); i++)
//        cout<<stops[i].time<<"\t";
//    cout<<endl;
//#endif

    //Sorted, go over to check capacity
    int load = 0;
    for ( int i = 0; i < stops.size(); i++)
    {
        if (stops[i].isDepartureTime)
            load--;
        else
            load++;
        if (load > stations[station].getCapacity())
            return true;
    }

    return false;
}

int getNextAvailableTime(int recentTime, int t, int station)
{
    //Find next available time for t which arrived at station by time recentTime
}


int adjustSchedule(int t, int s )    //Adjust train t at segment s, where train starts with 0 and s starts with 1. Return the waiting time
{
    int waitingTime = 0;                //In order to trace back, set a waitingTime value to track the waiting time before entering s

    if (trains[t].isWestbound())
    {
        for(int i = 0; i<t; i++)    //for all the trains that are already on the string graph
        {
            if ( !trains[i].isWestbound() )       //Differen directions
            {
                if (trains[t].collision(&trains[i], s))
                {

                    waitingTime += trains[i].getArrivalTime(s+1) - trains[t].getDepartureTime(s+1);
                    trains[t].setDepartureTime(s+1, trains[i].getArrivalTime(s+1), stations);
                    i = 0;      //Need to check all over again
                    //adjustSchedule(t,s);

                }
            }
            else    //if (same direction)
            {
                if (trains[t].rearEnd(&trains[i], s))
                {
                    waitingTime +=  trains[i].getDepartureTime(s+1) + 5 - trains[t].getDepartureTime(s+1);    //Delay for 5 minutes

                    trains[t].setDepartureTime(s+1, trains[i].getDepartureTime(s+1) + 5, stations);

                    i=0;        //Check over again
                }
            }

        }
        //if (s+1 != trains[t].getOrigin())
        //{
        //    if (isOverLoad(s+1))    //Check capcity
        //    {
        //        trains[t].setDepartureTime(s+1, trains[t].getDepartureTime(s+1) - waitingTime, stations);   //reset this segment
        //        trains[t].setDepartureTime(s+2, trains[t].getDepartureTime(s+2) + waitingTime, stations);   //waiting on the previous station
        //        adjustSchedule(t, s+1);
        //        //adjustSchedule(t, s);   //Reconfigure this layer
        //    }

        //}
        //else, just wait on the origin path
            
    }
    else //is eastbound
    {
        for (int i = 0; i < t; i++)     //for each train
        {
            if ( trains[i].isWestbound() )         //different directions
            {
                if ( trains[t].collision(&trains[i], s))        //if collision
                {
                    waitingTime += trains[i].getArrivalTime(s) - trains[t].getDepartureTime(s);
                    trains[t].setDepartureTime(s, trains[i].getArrivalTime(s), stations);
                    i=0;        //go back and check all other trains
                }
            }
            else                                //same direction
            {
                if (trains[t].rearEnd(&trains[i],s))        //if rear end in s
                {
                    waitingTime += trains[i].getDepartureTime(s) + 5 - trains[t].getDepartureTime(s);
                    trains[t].setDepartureTime(s, trains[i].getDepartureTime(s)+5 , stations);
                    i=0;
                }
            }
        }
        //trains[t].setDepartureTime(s, trains[t].getDepartureTime(s)-waitingTime,stations);
        //if (s != trains[t].getOrigin())
        //{
        //    if (isOverLoad(s))    //Check capcity
        //    {
        //        trains[t].setDepartureTime(s, trains[t].getDepartureTime(s) - waitingTime, stations);   //reset this segment
        //        trains[t].setDepartureTime(s-1, trains[t].getDepartureTime(s-1) + waitingTime, stations);   //waiting on the previous station
        //        adjustSchedule(t, s-1);
        //    }

        //}
        //else, just wait on the origin path
    }
    return waitingTime;
    //for(int i=0; i <t ; i++)        //for each train, check for collisions and rearends
    //{
    //    if(trains[t].isWestbound()) //is west bound
    //    {
    //        if(!trains[i].isWestbound())     //Opposite direction
    //        {
    //            if(trains[t].collision(&trains[i], s))       //if collision
    //            {   //update train schedule
    //                waitingTime += trains[i].getArrivalTime(s-1) - trains[t].getDepartureTime(s-1);
    //            }
    //        }
    //        else                            //same direction
    //        {
    //        }
    //    }
    //}
    //TODO: 
    //Check capacity
    //If over capacity
    //  Wait on the previous station
    //  adjustSchedule(t, s+/-1)
}

void init()
{
    //OpenGL initialize functions goes here
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   //backgroud color is white

    glutInitContextVersion(4, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);

    cout<<"Vendor: "<<glGetString(GL_VENDOR)<<endl;
    cout<<"Version: "<<glGetString(GL_VERSION)<<endl;
    cout<<"GLSL: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<endl;

}

void handleKeyPress(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_RIGHT:
            NumTrainToDraw ++;
            if (NumTrainToDraw > NUM_OF_TRAINS)
                NumTrainToDraw--;
            break;
        case GLUT_KEY_LEFT:
            NumTrainToDraw --;
            if (NumTrainToDraw < 0)
                NumTrainToDraw++;
            break;
        default:
            break;
    }
}
void changeSize(int w, int h)
{
    //call while window changed
    width = w;
    height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
}


void drawStations()
{
    int ySpan = 5;
    int xSpan = 10;
    int step = (height-ySpan)/stations.size();
    //TODO: Draw stations as well as it's data
    glColor3d(0,0,0);
    glBegin(GL_LINES);
    for(unsigned i=0; i<stations.size(); i++)
    {
        stations[i].setyPos(step*i+ySpan);
        stations[i].setWidth(width-xSpan-xSpan);
        glVertex2d(xSpan,step*i+ySpan);
        glVertex2d(width-xSpan,step*i+ySpan);

    }
    glEnd();
    //Print characters
    for(unsigned i=0; i<stations.size(); i++)
    {
        void *font = GLUT_BITMAP_8_BY_13;
        char text[32];
        sprintf(text, "(%d)",  stations[i].getID());
        glRasterPos2d(0, stations[i].getyPos());
        for(int j=0; text[j] != '\0'; j++)
        {
            glutBitmapCharacter(font, text[j]);
        }

        if (stations[i].getLoad()> stations[i].getCapacity())
            glColor3f(1.0, 0.0, 0.0);
        else
            glColor3f(0.0, 0.0, 0.0);
        sprintf(text, "%d/%d",  stations[i].getLoad(),stations[i].getCapacity());
        glRasterPos2d(width-40, stations[i].getyPos());
        for(int j=0; text[j] != '\0'; j++)
        {
            glutBitmapCharacter(font, text[j]);
        }
    }
}

void drawTrain(int id)
{
    //TODO: Draw the train with id
    glBegin(GL_LINE_STRIP);
    //glColor3f(colors[0] ,colors[1],colors[2]);
    vector<float> color = trains[id-1].getColor();
    glColor3f(color[0], color[1], color[2]);
    if(!trains[id-1].isWestbound())
    {
        for(int j = trains[id-1].getOrigin(); j<trains[id-1].getDestination(); j++)
        {
            glVertex2d(10 + trains[id-1].getArrivalTime(j) * stations[j-1].getWidth()/maxTime, stations[j-1].getyPos());
            glVertex2d(10 + trains[id-1].getDepartureTime(j) * stations[j-1].getWidth()/maxTime, stations[j-1].getyPos());
            glVertex2d(10 + trains[id-1].getArrivalTime(j+1) * stations[j].getWidth()/maxTime, stations[j].getyPos());
        }
    }
    else
    {
        for(int j = trains[id-1].getOrigin(); j > trains[id-1].getDestination(); j--)
        {
            glVertex2d(10 + trains[id-1].getArrivalTime(j) * stations[j-1].getWidth()/maxTime, stations[j-1].getyPos());
            glVertex2d(10 + trains[id-1].getDepartureTime(j) * stations[j-1].getWidth()/maxTime, stations[j-1].getyPos());
            glVertex2d(10 + trains[id-1].getArrivalTime(j-1) * stations[j-2].getWidth()/maxTime, stations[j-2].getyPos());
            
        }

    }
    glEnd();
}

void display()
{
    //function to render each frame
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(1,1,1);

    drawStations();

    //Debug
    for(int i=0; i < NumTrainToDraw; i++)
        drawTrain(trains[i].getID());

    
    

    glutSwapBuffers();
}

void timerFunction(int value)       
    //force it to refresh, no need of postRedisplay function any more
{
    //change values here
    
    glutPostRedisplay();
    glutTimerFunc(10,timerFunction,1);
}





int initialize()
{
    //Read data from file and initialize trains and stations

    //Initialize Trians
    string fileName("NetworkProject6651.csv");
    ifstream fin;

    fin.open(fileName.c_str());
    if(!fin.good())
        return 1;   //exit if file not found

    string buff;                                //buffer

    std::getline(fin, buff);                   //Eliminate the first line
    while(std::getline(fin, buff))
    {
        splitstring s(buff);
        vector<string> stationInfo = s.split(',');
        stationInfo[stationInfo.size()-1].erase(stationInfo[stationInfo.size()-1].size() - 1);      //remove last '\r'

        Station myStation(stationInfo);
        stations.push_back(myStation);

    }
    Station lastStation(61, 0, 40);
    stations.push_back(lastStation);

    fin.close();
    
    //Initialize train data

    fileName = "TrainScheduleProject6651.csv";
    fin.open(fileName.c_str());
    if(!fin.good())
        return 1;   //exit if file not found


    std::getline(fin, buff);                   //Eliminate the first line
    while(std::getline(fin, buff))
    {
        splitstring s(buff);
        vector<string> trainInfo = s.split(',');
        trainInfo[trainInfo.size()-1].erase(trainInfo[trainInfo.size()-1].size() - 1);      //remove last '\r'

        Train myTrain(trainInfo, stations);
        trains.push_back(myTrain);

    }
    fin.close();
    testTrains = trains;

    return 0;
}

int main(int argc, char **argv)
{
    initialize();



    //Adjust schedule
    for ( int i = 1; i < trains.size(); i++)
        if (trains[i].isWestbound())
            for ( int j = trains[i].getOrigin()-1; j>= trains[i].getDestination(); j--)
            {

                int waitingTime = adjustSchedule(i, j);

                cout<<i<<"\t"<<j<<"\t"<<waitingTime<<"\n";

                if ( j+1 != trains[i].getOrigin() )
                {
                    if (isOverLoad(i, j+1))
                    {
                        trains[i].setDepartureTime( j + 1, trains[i].getDepartureTime( j+1 ) - waitingTime, stations);
                        if (waitingTime == 0) waitingTime = 1;
                        trains[i].setDepartureTime( j + 2, trains[i].getDepartureTime( j+2 ) + waitingTime, stations);
                        j = j + 2;

                    }
                }
                    
            }
        else
            for ( int j = trains[i].getOrigin(); j < trains[i].getDestination(); j++)
            {
                int waitingTime = adjustSchedule(i, j);
                
                cout<<i<<"\t"<<j<<"\t"<<waitingTime<<"\n";

                if ( j != trains[i].getOrigin() )
                {
                    if (isOverLoad(i, j))
                    {
                        trains[i].setDepartureTime( j , trains[i].getDepartureTime( j ) - waitingTime, stations);
                        if (waitingTime == 0) waitingTime = 1;
                        trains[i].setDepartureTime( j - 1, trains[i].getDepartureTime( j - 1 ) + waitingTime, stations);
                        j = j - 2;
                    }
                }
            }

    
#ifdef DEBUG

    //for (int i = 1; i < stations.size(); i++)
    //if(isOverLoad(i))
    //    cout<<i<<endl;

    //for ( int i = 0; i < trains.size(); i++)
    //    for ( int j = i+1; j < trains.size(); j++)
    //        if (trains[i].getArrivalTime(28) == trains[j].getArrivalTime(28))
    //            cout<<i<<"\t"<<j<<endl;

    //for ( int i=0; i< stations.size(); i++)
    //    cout<<trains[28].getDepartureTime(i+1)<<"\t"<<trains[29].getDepartureTime(i+1)<<endl;

    
    

#endif

    
    //Init Opengl
    glutInit(&argc, argv);
    //Print contex info

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	//double buffer
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("String Graph");
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    init();
    glutTimerFunc(10, timerFunction, 1);
    glutSpecialFunc(handleKeyPress);
    glutMainLoop();
    return 0;
}
