//
//  Tree.h
//  CS440-HW1
//
//  Created by Xu Wenzhao on 9/16/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#ifndef __CS440_HW1__Tree__
#define __CS440_HW1__Tree__

#include <iostream>
#include <vector>

using namespace std;


class Tree_Node
{
public:
    //Node Data
    int position[2];
    double path_cost;                                //the cost of reaching this node 
    int expandingnodenum;                           //the number of node that is currently expanding
    double distance;
    double Function;
    int depth;
    int IEatAPea;                                   //Whether this node has eaten a pea when created;
    vector<int *> PeaList;
    
    
    //Children and Parents
    Tree_Node *parents;
    Tree_Node *child[4];
    
    //functions
    Tree_Node();
    Tree_Node(int x,int y,double cost);             //construct function with position and related pathcost

    
    
//    ~Tree_Node();
//    int whetherisgoalstate(int x,int y);
    

};




    


#endif /* defined(__CS440_HW1__Tree__) */
