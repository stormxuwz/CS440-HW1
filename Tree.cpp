//
//  Tree.cpp
//  CS440-HW1
//
//  Created by Xu Wenzhao on 9/16/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#include  <iostream>
#include "Tree.h"

using namespace std;

Tree_Node::Tree_Node(int x,int y,double cost) //Initialize The Node
{
    for(int i=0;i<4;i++)
    {
        child[i]=NULL;
    }
    
    position[0]=x;
    position[1]=y;
    parents=NULL;
    path_cost=cost;
    expandingnodenum=0;
}

Tree_Node::Tree_Node()
{
    position[0]=0;
    position[1]=2;
}

/*
int Tree_Node::whetherisgoalstate(int p[2])
{ 
    if (position[0]==p[0] && position[1]==p[1])
    {
        return 1;
    }
    
    else return 0;
}


//Tree_Node::~Tree_Node()
//{
 //   delete [] Tree_Node;
//}

*/

