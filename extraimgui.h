#ifndef EXTRAIMGUI_H
#define EXTRAIMGUI_H

#include "imgui.h"

/* 
    draw the text within the rectangular area between (x, y) and (x + w, y + h)
    which means this function can read '\n' in the string and type '\n' automatically

    s: display string
    x, y: the coordinate of the top left corner
    w, h: the width and height of the rectangular area
    th: the height of each text line
*/
void DrawTextStringWithinArea(char *s, double x, double y, double w, double h, double th);

#endif