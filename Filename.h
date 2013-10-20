//
//  Filename.h
//  CS440-HW1
//
//  Created by Xu Wenzhao on 9/22/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#ifndef __CS440_HW1__Filename__
#define __CS440_HW1__Filename__

#include <iostream>
#include "Filename.h"
#include <fstream.h>
#include "Tree.h"
#include "Map.h"


using namespace std;



void ReadFromFile(Tree_Node *Point, MazeMap * Maze);
void WriteToFile(MazeMap *Maze);
void ScreenOutPut(MazeMap *maze);




#endif /* defined(__CS440_HW1__Filename__) */


