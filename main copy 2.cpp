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

void depthfirst(Tree_Node *root, MazeMap *maze);
//void breadthfirst(Tree_Node *root, MazeMap *maze);
//void greedysearch(Tree_Node *root,MazeMap *maze);
//void Astarsearch(Tree_Node *root, MazeMap *maze);
void searchsolution(Tree_Node *root, MazeMap *maze,int type,MazeMap *maze0);
//void searchsolution_Pea(Tree_Node *root, MazeMap *maze,int type,MazeMap *maze0);     //search solution for all peas search

bool TheHistoryOfEatingPea(Tree_Node *node,Tree_Node * old);

void report(Tree_Node *node, MazeMap *maze,MazeMap *maze0);

Tree_Node *checkfinalstate(Tree_Node *node,MazeMap *maze,int type)
{
    Tree_Node *FinalNode;
    FinalNode=NULL;
    
    if (type<10)                        //it is a problem to find the outlet of the maze
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
        if(maze->eatenpea==maze->totaapea)
        {
            cout<<"I eat all the peas!!";
            FinalNode=node;
            exit(1);
            return FinalNode;
        }
    return FinalNode;
}

void InitializeRoot(Tree_Node *node,MazeMap *maze)
{
    node->distance=0;
    node->parents=node;
    node->depth=1;
    node->Function=0;
    node->expandingnodenum=0;
    node->path_cost=0;
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
            
        case 4:                 //A Star Search
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
        
//        case 6:                 //eat pea;
//            if () {
//                <#statements#>
//            }
            
        
        case 7:
            
            
        default:
            break;
    }
    return FinalNode;

}

void DealWithRepeat(Tree_Node *node,MazeMap *maze,vector<Tree_Node *> &NodeFrontier,int type)
{
    Tree_Node* tempnode;
    double value_Oldnode,Value_Newnode;
    
    for(int i=0;i<NodeFrontier.size();i++)
    {
        tempnode=NodeFrontier[i];
        if (node->position[0]==tempnode->position[0] && node->position[1]==tempnode->position[1])
        {
            value_Oldnode=tempnode->path_cost;
            Value_Newnode=node->path_cost;
            
            if (Value_Newnode<value_Oldnode) {
                delete NodeFrontier[i];
                NodeFrontier.erase(NodeFrontier.begin()+i);
                break;
            }
        }
    }
}

void DealWithRepeat_Pea(Tree_Node *node,MazeMap *maze,vector<Tree_Node *> &NodeFrontier,vector<Tree_Node *> &Nodelist)
{
    Tree_Node* tempnode;
    double value_Oldnode,Value_Newnode;

    for(int i=0;i<NodeFrontier.size();i++)
    {
        tempnode=NodeFrontier[i];
        if (node->position[0]==tempnode->position[0] && node->position[1]==tempnode->position[1] && node->IEatAPea==tempnode->IEatAPea)
        {
            value_Oldnode=tempnode->path_cost;
            Value_Newnode=node->path_cost;
        
            if (TheHistoryOfEatingPea(node, tempnode))      //return 1 means it is the repeat of history node
            {
                if (Value_Newnode<value_Oldnode)
                {
//                    delete NodeFrontier[i];
                    NodeFrontier.erase(NodeFrontier.begin()+i);
                    break;
                }
            }
            /*
            
            if (Value_Newnode<value_Oldnode)
            {
                delete NodeFrontier[i];
                NodeFrontier.erase(NodeFrontier.begin()+i);
                break;
            }
            */
        }
        
    }
    
    for (int i=0; i<Nodelist.size(); i++)       //check whether it is the repeat of history.
    {
        tempnode=Nodelist[i];
        
        //the position is the same, and the num of peas is the same, then compare the sequence of node
        if (node->position[0]==tempnode->position[0] && node->position[1]==tempnode->position[1]&& node->IEatAPea==tempnode->IEatAPea)           {
            if (TheHistoryOfEatingPea(node, tempnode))      //return 1 means it is the repeat of history node
            {
                NodeFrontier.pop_back();
                delete node;
                node=NULL;
                break;
            }

        }
    }
    
}


                
                
int main(int argc, const char * argv[])
{

    int length[7]={10,18,37,23,7,5,7};
    int width[7]={22,36,37,37,9,20,20};
    int mazetype;
    string filename[7];             //source file
    
    filename[0]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/smallMaze";
    filename[1]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/mediumMaze";
    filename[2]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/bigMaze";
    filename[3]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/openMaze";
    filename[4]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/tinySearch";
    filename[5]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/smallSearch";
    filename[6]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/trickySearch";
    
    mazetype=0;     //

    //creat map
    MazeMap *maze;
    MazeMap *Mazeoriginal;
    
    
    maze=new MazeMap(length[mazetype],width[mazetype],filename[mazetype]);
    Mazeoriginal=new MazeMap(length[mazetype],width[mazetype],filename[mazetype]);
        
    // Initial the Root
    
    Tree_Node *RootNode;
    
    RootNode=maze->root;
    
//      depthfirst(RootNode, maze);
//    breadthfirst(RootNode, maze);
//      greedysearch(RootNode, maze);
//    Astarsearch(RootNode, maze);
    searchsolution(RootNode, maze, 1,Mazeoriginal);      //1: depth;2: breadth;3 ....
//    searchsolution_Pea(RootNode, maze, 3, Mazeoriginal);
    
    
    delete RootNode;
    delete maze;
    return 0;
 }


void report(Tree_Node *pointer, MazeMap *maze,MazeMap *maze0)
{
    ofstream fout;
    fout.open(maze->outname.c_str(),ios::out);
    
    fout<<"The expanded node in this maze is: "<<maze->ExpandedNode<<endl;
    fout<<"The maximum tree depth search is: "<<endl;;
    fout<<"the maximum size of the frontier is: "<<endl;
    fout<<"the path cost is: "<<pointer->path_cost<<endl;
  
    while (pointer!=pointer->parents) {
        maze0->setpoint(pointer->position, '.');
        pointer=pointer->parents;
        ScreenOutPut(maze0);
    }
    
    ScreenOutPut(maze0);

    for (int y=0; y<maze0->length; y++) {
        for(int x=0;x<maze0->width+1;x++)
        {
            fout<<maze0->findpoint(x, y);
        }
    }
    
    
    //read a clear map

    fout.close();
}

void depthfirst(Tree_Node *root, MazeMap *maze)
{
    int time=0;
    int index=0;
    
    vector<Tree_Node *> NodeFrontier;
    Tree_Node *Root;
    Tree_Node *NodeToExpand;

    Root=maze->root;

    
    InitializeRoot(Root, maze);         //initialize the root
    NodeFrontier.push_back(root);       //initialize the node frontier
    
    while (time<1000000)
    {
        NodeToExpand=TheNodeToExpand(NodeFrontier, 1,index,maze);
        maze->ExpandedNode+=1;
        maze->ExpandNode(NodeToExpand);
        
        for (int i=0; i<4; i++)
        {
            if(NodeToExpand->child[i]!=0)
            {
                NodeFrontier.push_back(NodeToExpand->child[i]);
                NodeToExpand->expandingnodenum=i;
                maze->setpoint(NodeToExpand->child[i]->position, 'A');
                break;
            }
            
            if (i==3)       //Such Node is the end of a branch
            {
                NodeToExpand->parents->child[NodeToExpand->parents->expandingnodenum]=NULL;
                maze->setpoint(NodeToExpand->position, 'D');
                delete NodeToExpand;
                NodeFrontier.pop_back();
            }
        }
        ScreenOutPut(maze);
    }    
}

/*
void breadthfirst(Tree_Node *root, MazeMap *maze)
{
    int time=0;
    int index=0;
    int num_of_newnode=0;
    int type=2;
    
    vector<Tree_Node *> NodeFrontier;
    Tree_Node *Root;
    Tree_Node *NodeToExpand;
    //    Tree_Node *Temp_Node;
    Root=maze->root;
    
    
    InitializeRoot(Root, maze);         //initialize the root
    NodeFrontier.push_back(root);       //initialize the node frontier
    
    while (time<100000) {
        
        num_of_newnode=0;
        
        NodeToExpand=TheNodeToExpand(NodeFrontier, 2,index,maze);
        
        
        maze->ExpandedNode+=1;
        maze->ExpandNode(NodeToExpand);
        
        for (int i=0; i<4; i++)
        {
            if(NodeToExpand->child[i]!=0)
            {
                checkfinalstate(NodeToExpand->child[i], maze,type);
                NodeFrontier.push_back(NodeToExpand->child[i]);
                maze->setpoint(NodeToExpand->child[i]->position, 'A');
                num_of_newnode+=1;
            }
        }

            NodeFrontier.erase(NodeFrontier.begin()+index);             
        
        ScreenOutPut(maze);
        cout<<endl;
    }
}

void greedysearch(Tree_Node *root, MazeMap *maze)
{
    int time=0;
    int index=0;
    int num_of_newnode=0;
    int type=3;
    
    vector<Tree_Node *> NodeFrontier;
    Tree_Node *Root;
    Tree_Node *NodeToExpand;
    //    Tree_Node *Temp_Node;
    Root=maze->root;
    
    
    InitializeRoot(Root, maze);         //initialize the root
    NodeFrontier.push_back(root);       //initialize the node frontier
    
    while (time<100000) {
        
        num_of_newnode=0;
        
        NodeToExpand=TheNodeToExpand(NodeFrontier, 3,index,maze);
        
        maze->ExpandedNode+=1;
        maze->ExpandNode(NodeToExpand);
        
        for (int i=0; i<4; i++)
        {
            if(NodeToExpand->child[i]!=0)
            {
                checkfinalstate(NodeToExpand->child[i], maze,type);
                NodeFrontier.push_back(NodeToExpand->child[i]);
                maze->setpoint(NodeToExpand->child[i]->position, 'A');
                num_of_newnode+=1;
            }
        }

        NodeFrontier.erase(NodeFrontier.begin()+index);
        
        ScreenOutPut(maze);
        cout<<endl;
    }
}


void Astarsearch(Tree_Node *root, MazeMap *maze)
{
    int time=0;
    int index=0;
    int num_of_newnode=0;
    int type=4;
    
    vector<Tree_Node *> NodeFrontier;
    Tree_Node *Root;
    Tree_Node *NodeToExpand;
    //    Tree_Node *Temp_Node;
    Root=maze->root;
    
    
    InitializeRoot(Root, maze);         //initialize the root
    NodeFrontier.push_back(root);       //initialize the node frontier
    
    while (time<100000) {
        
        num_of_newnode=0;
        
        NodeToExpand=TheNodeToExpand(NodeFrontier,4,index,maze);
        
        
        maze->ExpandedNode+=1;
        maze->ExpandNode(NodeToExpand);
        
        for (int i=0; i<4; i++)
        {
            if(NodeToExpand->child[i]!=0)
            {
                checkfinalstate(NodeToExpand->child[i], maze,type);
                NodeFrontier.push_back(NodeToExpand->child[i]);
                maze->setpoint(NodeToExpand->child[i]->position, 'A');
                num_of_newnode+=1;
            }
        }

        NodeFrontier.erase(NodeFrontier.begin()+index);
        
        ScreenOutPut(maze);
        cout<<endl;
    }
}

*/

void searchsolution(Tree_Node *root, MazeMap *maze,int type,MazeMap *maze0)     //search solution for non-depth search
{
    int time=0;
    int index=0;
    
    vector<Tree_Node *> NodeFrontier;
    Tree_Node *Root;
    Tree_Node *NodeToExpand;
    Tree_Node *LastNode;

    Root=maze->root;
    
    
    InitializeRoot(Root, maze);         //initialize the root
    NodeFrontier.push_back(root);       //initialize the node frontier
    
    while (time<100000) {
        time+=1;
        maze->ExpandedNode+=1;
        NodeToExpand=TheNodeToExpand(NodeFrontier,type,index,maze);
        NodeFrontier.erase(NodeFrontier.begin()+index);                     //delete the parent node;

        maze->ExpandedNode+=1;
        maze->ExpandNode(NodeToExpand);
        
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
                    report(NodeToExpand, maze, maze0);
                    exit(1);
                }
                    
            }
        }
        maze->setpoint(NodeToExpand->position, 'A');            //mark the Node as explored on the map

        ScreenOutPut(maze);
        cout<<endl;
    }
}

void searchsolution_Pea(Tree_Node *root, MazeMap *maze,int type,MazeMap *maze0)     //search solution for all peas search
{
    int time=0;
    int index=0;
    
    vector<Tree_Node *> Nodelist;
    vector<Tree_Node *> NodeFrontier;
    Tree_Node *Root;
    Tree_Node *NodeToExpand;
    Tree_Node *LastNode;
    
    Root=maze->root;
    
    InitializeRoot(Root, maze);         //initialize the root
    NodeFrontier.push_back(root);       //initialize the node frontier
    
    while (time<100000) {
        time+=1;
        maze->ExpandedNode+=1;
        NodeToExpand=TheNodeToExpand(NodeFrontier,type,index,maze);
        
        NodeFrontier.erase(NodeFrontier.begin()+index);         //delete the parents node from the frontier
        
        maze->ExpandedNode+=1;
        maze->ExpandNode(NodeToExpand);
        
        for (int i=0; i<4; i++)
        {
            if(NodeToExpand->child[i]!=0)
            {
                LastNode=checkfinalstate(NodeToExpand->child[i], maze,type);
                
                if(!LastNode)
                {
                    NodeFrontier.push_back(NodeToExpand->child[i]);
                    //Check whether this node has already been eaten
                    DealWithRepeat_Pea(NodeToExpand->child[i], maze, NodeFrontier,Nodelist);                    
                }
                
                else
                {
                    report(NodeToExpand, maze, maze0);
                    exit(1);
                }
                
            }
        }
        
//        maze->setpoint(NodeToExpand->position,' ');
        Nodelist.push_back(NodeToExpand);                       //push the node into the explored set
        //  delete NodeToExpand;
//        NodeFrontier.erase(NodeFrontier.begin()+index);         //delete the parents node from the frontier
        
        ScreenOutPut(maze);
        cout<<endl;
    }
}


bool TheHistoryOfEatingPea(Tree_Node *node,Tree_Node *old)  //1: it is a repeat; 0 it is not a repeat
{
    
    for (int i=0; i<node->PeaList.size(); i++)
    {
        for (int j=0; j<old->PeaList.size(); j++) {
            if (node->PeaList[i][0]!=old->PeaList[j][0] &&node->PeaList[i][1]!=old->PeaList[j][1])
            {
                return 0;
            }
        }
    }
    return 1;
    
}