//
//  Map.h
//  CS440-HW1
//
//  Created by Xu Wenzhao on 9/18/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#ifndef __CS440_HW1__Map__
#define __CS440_HW1__Map__

#include <iostream>
#include <string.h>
#include <vector>
#include "Tree.h"

using namespace std;

class MazeMap {
    
public:
   
    int length,width;                   //the length and width of the Maze
    char *mapvalue;                     //the value of each point in the maze
    Tree_Node *root;                    //the root of the tree, i.e. the starting point;
    
    int start[2];                       //the starting point
    int goal[2];                        //the goal point
    
    int * goal_pea;                     //the position for pea in the maze;
    
    int ExpandedNode;                   //the total number of expaned node
    
    int eatenpea;                       //the total number of pea that has been eaten.
    int totaapea;                       //total number of pea that should be eaten. There is a typo here. 
    vector<int *> PeaPosition;          //the Position of Pea in the Maze
    
    string outname,inname;              //file input and output name;

    MazeMap(int length,int width, string filename);    //initialization,including reading data and iniliaze the start and goal;
    
    char findpoint(int x,int y);            //return the value given x and y
    void ExpandNode(Tree_Node * point);     //Expand Node in the maze problem
    void ExpandNode_Pea(Tree_Node *point);  //Expand Node in the pea eating problem
    void setpoint(int *p,char x);           //set the value of point p with char x
    void SetDistToGoal(Tree_Node * point);  //set the value of distance to node point
    void SetFuncValue(Tree_Node * point);   //set the value of function to node point
    void SetPathCost(Tree_Node *point);     //set the value of path cost to node point
    void EatingPea(Tree_Node *point);       //Judging whether this pea has being eaten before.
    double FutureDistance(Tree_Node *node);  //return the distance to the nearest pea;


};

#endif /* defined(__CS440_HW1__Map__) */
