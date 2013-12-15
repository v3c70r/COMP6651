#include "tools.h"

void idToColor(int id, int maxId, float * rgb )
{
    rgb[0] = 0.0;
    rgb[1] = 0.0;
    rgb[2] = 0.0;
    float step = 3.0/maxId;
    int spliter = maxId/3;

    int fullColor = id/spliter;
    int residual = id%spliter;

    for ( int i=0; i < fullColor; i++)
        rgb[i] = 1.0;

    rgb[fullColor] = step * residual;

}
