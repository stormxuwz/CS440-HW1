//
//  Filename.cpp
//  CS440-HW1
//
//  Created by Xu Wenzhao on 9/22/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#include "Filename.h"
#include <fstream.h>
#include <iostream>
#include <iomanip.h>
#include "Tree.h"
#include "Map.h"


using namespace std;

ofstream ToFile;
ifstream FromFile;

void ReadFromFile(Tree_Node *Point, MazeMap * Maze)
{
    
}


void WriteToFile(MazeMap *Maze)
{
    
}

void ScreenOutPut(MazeMap *maze)
{
    int length;
    int width;
    
    length=maze->length;
    width=maze->width;
    
    for(int y=0;y<length;y++)
    {
        for(int x=0;x<width+1;x++)
        {
            cout<<maze->findpoint(x, y);
        }
    }
}

