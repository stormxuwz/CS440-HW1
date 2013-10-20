//
//  main.cpp
//  CS440-HW1
//
//  Created by Xu Wenzhao on 9/16/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#include <iostream>
#include <string>
#include "Tree.h"
#include "Map.h"
#include "Filename.h"
#include <vector>
#include <list>
using namespace std;

void searchsolution(Tree_Node *root, MazeMap *maze,int type,MazeMap *maze0);            //search solutio of maze
void searchsolution_Pea(Tree_Node *root, MazeMap *maze,int type,MazeMap *maze0);        //search solution for all peas search

bool TheHistoryOfEatingPea(Tree_Node *node,Tree_Node * old);                            //determing whether the Tree node and old has the same history of eating peas. i.e. whether these two nodes have the same peas eaten before.

void report(Tree_Node *node, MazeMap *maze,MazeMap *maze0);                             //file output
    

Tree_Node *checkfinalstate(Tree_Node *node,MazeMap *maze,int type)                      //Check whether it is the Goal state. 
{
    Tree_Node *FinalNode;
    FinalNode=NULL;
    
    //Change the ">" to "<" to switch to maze problem.
    
     if (type>20)                        //  it is a problem to find the outlet of the maze
      {
        if (node->position[0]==maze->goal[0]&&node->position[1]==maze->goal[1])
        {
            cout<<"I found the goal!!"<<endl<<endl;
            maze->setpoint(node->parents->position, 'A');
            FinalNode=node;
            ScreenOutPut(maze);
            cout<<endl;
 //           exit(1);
            return FinalNode;
        }
    }
    else                                //it is a problem to eat all peas;
        if(node->IEatAPea==maze->totaapea)
        {
            cout<<"I eat all the peas!!";
            FinalNode=node;
 //           exit(1);
            return FinalNode;
        }

    return FinalNode;
}

void InitializeRoot(Tree_Node *node,MazeMap *maze)                                      //The initialization process of root node
{
    node->distance=0.000;
    node->parents=node;
    node->depth=1;
    node->Function=0;
    node->expandingnodenum=0;
    node->path_cost=0;
    node->IEatAPea=0;
//    node->PeaList.push_back(node->position);
}

Tree_Node *TheNodeToExpand(vector<Tree_Node *> &NodeFrontier,int type,int &IndexInFrontier,MazeMap *maze)
{
    
    double value_potential = 0.0;                //The variable to be compared to determine which node to expand based on certain strategy
    double value_temp=0.0;
    Tree_Node *FinalNode;
    
    switch (type) {
        case 1:                 //Depth search
            value_potential=-NodeFrontier[0]->depth;
            
            for (int i=0;i<NodeFrontier.size(); i++)
            {
                value_temp=-NodeFrontier[i]->depth;
                
                if(value_temp<value_potential||value_temp==value_potential)          //expand the least
                {
                    value_potential=value_temp;
                    FinalNode=NodeFrontier[i];
                    IndexInFrontier=i;
                }
            }
            break;
        
        case 2:                 //Breadth search
            value_potential=NodeFrontier[0]->depth;
            
            for (int i=0;i<NodeFrontier.size(); i++)
            {
                value_temp=NodeFrontier[i]->depth;
                
                if(value_temp<value_potential||value_temp==value_potential)          //expand the least
                {
                    value_potential=value_temp;
                    FinalNode=NodeFrontier[i];
                    IndexInFrontier=i;
                }
            }
            break;
            
        case 3:                 //Greedy search
            value_potential=NodeFrontier[0]->distance;
            
            for (int i=0;i<NodeFrontier.size(); i++)
            {
                value_temp=NodeFrontier[i]->distance;
                
                if(value_temp<value_potential||value_temp==value_potential)
                {
                    value_potential=value_temp;
                    FinalNode=NodeFrontier[i];
                    IndexInFrontier=i;
                }
            }
            break;
            
        case 4:                 //A Star Search  function= path cost+ Manhattan distance
            value_potential=NodeFrontier[0]->Function;
            
            for (int i=0;i<NodeFrontier.size(); i++)
            {
                value_temp=NodeFrontier[i]->Function;
                
                if(value_temp<value_potential||value_temp==value_potential)
                {
                    value_potential=value_temp;
                    FinalNode=NodeFrontier[i];
                    IndexInFrontier=i;
                }
            }
            break;
            
        case 5:                 //uniform strategy
            
            value_potential=NodeFrontier[0]->path_cost;
            
            for (int i=0;i<NodeFrontier.size(); i++)
            {
                value_temp=NodeFrontier[i]->path_cost;
                
                if(value_temp<value_potential||value_temp==value_potential)
                {
                    value_potential=value_temp;
                    FinalNode=NodeFrontier[i];
                    IndexInFrontier=i;
                }
            }
            
            break;
        
        case 6:                //A star search (1):  heuristic function= # of remaining targets
            value_potential=NodeFrontier[0]->path_cost+maze->totaapea-NodeFrontier[0]->IEatAPea;
            
            for (int i=0; i<NodeFrontier.size(); i++) {
                value_temp=NodeFrontier[i]->path_cost+maze->totaapea-NodeFrontier[i]->IEatAPea;
                if(value_temp<value_potential || value_temp==value_potential)
                {
                    value_potential=value_temp;
                    FinalNode=NodeFrontier[i];
                    IndexInFrontier=i;
                }
            }
            break;
            
        case 7: //A star search (2):heuristic function=distance=minimum Manhattan distance or
                //A star search (3):heuristic function=distance=maximum Manhattan distance
            
            value_potential=NodeFrontier[0]->path_cost+NodeFrontier[0]->distance;
            
            for (int i=0; i<NodeFrontier.size(); i++) {
                value_temp=NodeFrontier[i]->path_cost+NodeFrontier[i]->distance;
                if(value_temp<value_potential || value_temp==value_potential)
                {
                    value_potential=value_temp;
                    FinalNode=NodeFrontier[i];
                    IndexInFrontier=i;
                }
            }
            break;
        
        case 8: //A star search (4): heuristic function=minimum Manhattan distance+ number of remaining targets -1
            
            value_potential=NodeFrontier[0]->path_cost+NodeFrontier[0]->distance+maze->totaapea-NodeFrontier[0]->IEatAPea-1;
            
            for (int i=0; i<NodeFrontier.size(); i++) {
                value_temp=NodeFrontier[i]->path_cost+NodeFrontier[i]->distance+maze->totaapea-NodeFrontier[i]->IEatAPea-1;
                if(value_temp<value_potential || value_temp==value_potential)
                {
                    value_potential=value_temp;
                    FinalNode=NodeFrontier[i];
                    IndexInFrontier=i;
                }
            }
            break;
    
            
        default:
            break;
    }
    return FinalNode;

}

void DealWithRepeat(Tree_Node *node,MazeMap *maze,vector<Tree_Node *> &NodeFrontier,int type)
{
    Tree_Node* tempnode;
    double value_Oldnode,Value_Newnode;
    
    for(int i=0;i<NodeFrontier.size()-1;i++)
    {
        tempnode=NodeFrontier[i];
        if (node->position[0]==tempnode->position[0] && node->position[1]==tempnode->position[1])
        {
            value_Oldnode=tempnode->path_cost;
            Value_Newnode=node->path_cost;
            
            if (Value_Newnode<value_Oldnode) {
                
                delete NodeFrontier[i];
                NodeFrontier.erase(NodeFrontier.begin()+i);

            }
            else
            {
                NodeFrontier.pop_back();
                delete node;
                node=NULL;
            }
            
            break;
        }
    }
}

void DealWithRepeat_Pea(Tree_Node *node,MazeMap *maze,vector<Tree_Node *> &NodeFrontier,vector<Tree_Node *> &Nodelist)
{
    Tree_Node* tempnode;
    double value_Oldnode,Value_Newnode;
    bool Repeat=0;

    for(int i=0;i<NodeFrontier.size()-1;i++)      //check whether it is the repeat of node in node frontier. 
    {
        tempnode=NodeFrontier[i];
        if (node->position[0]==tempnode->position[0] && node->position[1]==tempnode->position[1] && node->IEatAPea==tempnode->IEatAPea)
        {
            value_Oldnode=tempnode->path_cost;
            Value_Newnode=node->path_cost;
        
            if (TheHistoryOfEatingPea(node, tempnode)==0)      //return 0 means it is the repeat of history node
            {
                if (Value_Newnode<value_Oldnode)
                {
//                    delete NodeFrontier[i];
                    NodeFrontier.erase(NodeFrontier.begin()+i);
                    Repeat=1;
                   
                }
                else
                {
                    NodeFrontier.pop_back();
                    delete node;
                    node=NULL;
                    Repeat=1;
                }
                
                break;
            }

        }
        
    }
    
    if (Repeat==0)
    {
        for (int i=0; i<Nodelist.size(); i++)       //check whether it is the repeat of history.
        {
            tempnode=Nodelist[i];
            
            //the position is the same, and the num of peas is the same, then compare the sequence of node
            if (node->position[0]==tempnode->position[0] && node->position[1]==tempnode->position[1]&& node->IEatAPea==tempnode->IEatAPea)           {
                if (TheHistoryOfEatingPea(node, tempnode)==0)      //return 0 means it is the repeat of history node
                {
                    NodeFrontier.pop_back();
                    delete node;
                    node=NULL;
                    break;
                }

            }
        }
    }
}


                
                
int main(int argc, const char * argv[])
{
    // the length and width of maze coresponding to the maze type.
    int length[7]={10,18,37,23,7,5,7};
    int width[7]={22,36,37,37,9,20,20};
    
    int mazetype;                           //the type of maze    
    string filename[7];                     //source file
    
    // the file source
    filename[0]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/smallMaze";
    filename[1]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/mediumMaze";
    filename[2]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/bigMaze";
    filename[3]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/openMaze";
    filename[4]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/tinySearch";
    filename[5]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/smallSearch";
    filename[6]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/trickySearch";
    
    mazetype=6;

    //creat map
    MazeMap *maze;
    MazeMap *Mazeoriginal;
    
    maze=new MazeMap(length[mazetype],width[mazetype],filename[mazetype]);           //initialize the maze
    Mazeoriginal=new MazeMap(length[mazetype],width[mazetype],filename[mazetype]);  //create a clear map for output
        
    // Initial the Root
    
    maze->eatenpea=0;

    Tree_Node *RootNode;
    
    RootNode=maze->root;
    
//    searchsolution(RootNode, maze, 1,Mazeoriginal);      //1: depth;2: breadth;3 greedy search; 4 and 6,7,8: A* ;5: uniform search
    searchsolution_Pea(RootNode, maze, 7, Mazeoriginal);
    
    
    delete RootNode;
    delete maze;
    return 0;
 }


void report(Tree_Node *pointer, MazeMap *maze,MazeMap *maze0,int maximumfrontier,int deepestdepth)
{

    ofstream fout;
    fout.open(maze->outname.c_str(),ios::out);
    char order;
    
    fout<<"The expanded node in this maze is: "<<maze->ExpandedNode<<endl;
    fout<<"The maximum tree depth search is: "<<deepestdepth<<endl;;
    fout<<"the maximum size of the frontier is: "<<maximumfrontier<<endl;
    fout<<"the path cost is: "<<pointer->path_cost<<endl;
    
 
//    fout<<"("<<pointer->position[0]<<", "<<pointer->position[1]<<":"<<pointer->IEatAPea<<")"<<endl;     //output
    
    while (pointer!=pointer->parents) {
  //      maze0->setpoint(pointer->position, '.');
        
        if(pointer->IEatAPea>pointer->parents->IEatAPea)        //just output the pea's position.
        {
            fout<<"("<<pointer->position[0]<<", "<<pointer->position[1]<<":"<<pointer->IEatAPea<<")"<<endl;
                switch (pointer->IEatAPea) {
                    case 10:
                        order='A';
                        break;
                    case 11:
                        order='B';
                        break;
                    case 12:
                        order='C';
                        break;
                    case 13:
                        order='D';
                        break;
                    case 14:
                        order='E';
                        break;
                    case 15:
                        order='F';
                        break;
                    case 16:
                        order='G';
                        break;
                    case 17:
                        order='H';
                        break;
                    case 18:
                        order='I';
                        break;
                    case 19:
                        order='J';
                        break;
                        
                    case 1:
                        order='1';
                        break;
                    case 2:
                        order='2';
                        break;
                    case 3:
                        order='3';
                        break;
                    case 4:
                        order='4';
                        break;
                    case 5:
                        order='5';
                        break;
                    case 6:
                        order='6';
                        break;
                    case 7:
                        order='7';
                        break;
                    case 8:
                        order='8';
                        break;
                    case 9:
                        order='9';
                        break;
                    default:
                        break;
                }



            maze0->setpoint(pointer->position, order);
        }
        pointer=pointer->parents;
        
        ScreenOutPut(maze0);
    }
    
    fout<<"("<<pointer->position[0]<<", "<<pointer->position[1]<<":"<<pointer->IEatAPea<<")"<<endl;
    ScreenOutPut(maze0);

    //print the visual solution
    for (int y=0; y<maze0->length; y++) {
        for(int x=0;x<maze0->width+1;x++)
        {
            fout<<maze0->findpoint(x, y);
        }
    }
    

    fout.close();
}


void searchsolution(Tree_Node *root, MazeMap *maze,int type,MazeMap *maze0)     //search solution for maze search 
{
    int time=0;
    int index=0;
    int maximumFrontier;
    int deepestdepth=1;
    
    vector<Tree_Node *> NodeFrontier;
    Tree_Node *Root;
    Tree_Node *NodeToExpand;
    Tree_Node *LastNode;

    Root=maze->root;
    
    
    InitializeRoot(Root, maze);         //initialize the root
    NodeFrontier.push_back(root);       //initialize the node frontier
    maximumFrontier=int(NodeFrontier.size());
    
    while (time<1000000) {
        time+=1;


        NodeToExpand=TheNodeToExpand(NodeFrontier,type,index,maze);         //determine the node to expand
        NodeFrontier.erase(NodeFrontier.begin()+index);                     //delete the parent node

        maze->ExpandedNode+=1;
        maze->ExpandNode(NodeToExpand);                                     //expand the node
        
        for (int i=0; i<4; i++)
        {
            if(NodeToExpand->child[i]!=0)
            {
                LastNode=checkfinalstate(NodeToExpand->child[i], maze,type);
                if(!LastNode)
                {
                    NodeFrontier.push_back(NodeToExpand->child[i]);
                    DealWithRepeat(NodeToExpand->child[i], maze, NodeFrontier,type);
                }
                
                else
                {
                    report(NodeToExpand->child[i], maze, maze0,maximumFrontier,deepestdepth);
                    time=1000000;
                    break;
                    //exit(1);
                    
                }
                    
            }
        }
        maze->setpoint(NodeToExpand->position, 'A');            //mark the Node as explored on the map
        
        if(NodeFrontier.size()>maximumFrontier)
            maximumFrontier=int(NodeFrontier.size());
        
        
        for (int i=0; i<NodeFrontier.size(); i++) {
            if (NodeFrontier[i]->depth>deepestdepth) {
                deepestdepth=NodeFrontier[i]->depth;
            }
        }
        

//        ScreenOutPut(maze);
 //       cout<<endl;
    }
}

void searchsolution_Pea(Tree_Node *root, MazeMap *maze,int type,MazeMap *maze0)     //search solution for all peas search
{
    int time=0;
    int index=0;
    int maximumFrontier;                                //the maximum number of the size of node frontier. 
    int deepestdepth;                                   //the maximum depth of the tree 
    
    vector<Tree_Node *> Nodelist;                       //the vector to store the historical information, i.e. expanded node
    vector<Tree_Node *> NodeFrontier;                   //Node frontier
    Tree_Node *Root;
    Tree_Node *NodeToExpand;                            //the Node that will be expanded
    Tree_Node *LastNode;
    
    Root=maze->root;
    
    InitializeRoot(Root, maze);                         //initialize the root
    NodeFrontier.push_back(root);                       //initialize the node frontier with the root node
    maximumFrontier=int(NodeFrontier.size());           
    deepestdepth=1;
    
    while (time<1000000) {
        time+=1;
 
        NodeToExpand=TheNodeToExpand(NodeFrontier,type,index,maze);     // determine which node to expand
 
        cout<<time<<","<<maximumFrontier<<endl;                         //For debugging

       /*//   debugging use
        
        ScreenOutPut(maze);
        cout<<endl;
        
        for(int i=0;i<NodeFrontier.size();i++)
        {
            cout<<"("<<NodeFrontier[i]->position[0]<<","<<NodeFrontier[i]->position[1]<<":"<<NodeFrontier[i]->IEatAPea<<");";
        }
        
        cout<<endl;
         //*/
        
        Nodelist.push_back(NodeToExpand);                       //push the node into the explored set;
        NodeFrontier.erase(NodeFrontier.begin()+index);         //delete the parents node from the frontier
        
        maze->ExpandedNode+=1;
        maze->ExpandNode_Pea(NodeToExpand);                     //expand the node;
        
        for (int i=0; i<4; i++)
        {
            if(NodeToExpand->child[i]!=0)
            {
                LastNode=checkfinalstate(NodeToExpand->child[i], maze,type);
                
                if(!LastNode)
                {
                    NodeFrontier.push_back(NodeToExpand->child[i]);
                    DealWithRepeat_Pea(NodeToExpand->child[i], maze, NodeFrontier,Nodelist);                                                       
                }
                
                else
                {
                    report(NodeToExpand->child[i], maze, maze0,maximumFrontier,deepestdepth);
                    time=1000000;
                    break;
                   // exit(1);
                }
                
            }
        }
        cout<<endl;
        
        //Calculate the largest number of size of node frontier
        if(NodeFrontier.size()>maximumFrontier)
            maximumFrontier=int(NodeFrontier.size());
        
        //Calculate the largest depth in the node frontier.
        for (int i=0; i<NodeFrontier.size(); i++) {
            if (NodeFrontier[i]->depth>deepestdepth) {
                deepestdepth=NodeFrontier[i]->depth;
            }
        }
    
    }

}


bool TheHistoryOfEatingPea(Tree_Node *node,Tree_Node *old)  //1: it is not a repeat; 0: it is totally a repeat
{
    int NumOfOldPea=0;
    for (int i=0; i<node->PeaList.size(); i++)
    {
        for (int j=0; j<old->PeaList.size(); j++) {
            if (node->PeaList[i][0]==old->PeaList[j][0] && node->PeaList[i][1]==old->PeaList[j][1]) 
            {
                NumOfOldPea+=1;                     //one pea is eaten by both the new node and the history node
            }
        }
    }
    if(NumOfOldPea==node->PeaList.size())          
    {
        return 0;                                    //All the node eaten by new node is the same with the old node. It is a repeat.
    }
    else
        return 1;
}