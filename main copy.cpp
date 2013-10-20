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
void breadthfirst(Tree_Node *root, MazeMap *maze);
void greedysearch(Tree_Node *root,MazeMap *maze);
void Astarsearch(Tree_Node *root, MazeMap *maze,MazeMap *maze0);

void report(Tree_Node *node, MazeMap *maze,MazeMap *maze0);

void InitializeRoot(Tree_Node *node,MazeMap *maze);

Tree_Node *TheNodeToExpand(Tree_Node *node, vector<Tree_Node *>);

void DealWithRepeat(Tree_Node *node,MazeMap *maze);


int main(int argc, const char * argv[])
{

    int length[4]={10,0,37,0};
    int width[4]={22,0,37,0};
    int mazetype;
        string filename[4];             //source file
    
    filename[0]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/smallMaze";
    filename[1]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/mediumMaze";
    filename[2]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/bigMaze";
    filename[3]="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/openMaze";
        
    
    mazetype=2;     //

    //creat map
    MazeMap *maze;
    MazeMap *Mazeoriginal;
    
    
    maze=new MazeMap(length[mazetype],width[mazetype],filename[mazetype]);
    Mazeoriginal=new MazeMap(length[mazetype],width[mazetype],filename[mazetype]);
        
    // Initial the Root
    
    Tree_Node *RootNode;
    
    RootNode=maze->root;
    
//    depthfirst(RootNode, maze);
//    breadthfirst(RootNode, maze);
//    greedysearch(RootNode, maze);
    Astarsearch(RootNode, maze,Mazeoriginal);
    
    
    delete RootNode;
    delete maze;
    return 0;
 }

void depthfirst(Tree_Node *root,MazeMap *maze)
{
    int x,y;
    Tree_Node *Pointer_Node;
    Tree_Node *temp_Node;
    
    int totaltime=0;
    
    Pointer_Node=root;
    Pointer_Node->parents=Pointer_Node;                 //the parent of RootNode is RootNode
    
    x=root->position[0];
    y=root->position[1];
    

    //depth search
    while (totaltime<10000000)
    {
        
        if(Pointer_Node->position[0]==maze->goal[0] && Pointer_Node->position[1]==maze->goal[1])
        {
            cout<<"I found the outlet"<<endl;
            ScreenOutPut(maze);
            delete Pointer_Node;
            delete temp_Node;
            exit(1);
        }
        //setpoint as pathcost
        
        maze->ExpandNode(Pointer_Node);            //expand the node;
        maze->setpoint(Pointer_Node->position,'A');     //mark that this point has been explored with 'A'
        
 //       ScreenOutPut(maze);
 //       cout<<endl<<endl;;
        
        //choose one of the ways to go
        for(int i=0;i<4;i++)
        {
            maze->ExpandedNode+=1;
            
            if (Pointer_Node->child[i]!=NULL)               
            {
                temp_Node=Pointer_Node;  
                Pointer_Node=Pointer_Node->child[i];
                temp_Node->expandingnodenum=i;
                break;
            }
            if(i==3)                                            //if all of the child is NUll, meaning it is the deadend
            {
                maze->setpoint(Pointer_Node->position,'D');     //mark that this point has been explored with 'D'
//                ScreenOutPut(maze);
//                cout<<endl<<endl;;
                
                delete Pointer_Node;
                Pointer_Node=temp_Node;
                temp_Node=Pointer_Node->parents;
                Pointer_Node->child[Pointer_Node->expandingnodenum]=NULL;
                
            }
        }
                
 //       ScreenOutPut(maze);
    }
    delete Pointer_Node;
    delete temp_Node;                       //
}

void breadthfirst(Tree_Node *root, MazeMap *maze)
{
    int x,y;
    vector<Tree_Node*> NodeFrontier;
    vector<Tree_Node*> NodeFrontier_Next;
    
    Tree_Node *Pointer_Node;
    
    int totaltime=0;
    
    Pointer_Node=root;
    Pointer_Node->parents=Pointer_Node;                 //the parent of RootNode is RootNode
    
    x=root->position[0];
    y=root->position[1];
    
    NodeFrontier.push_back(root);
    
    //breadth search
    while (totaltime<10000000)
    {
        
        for(int i=0;i<NodeFrontier.size();i++)
        {
            Pointer_Node=NodeFrontier[i];
            
            if(Pointer_Node->position[0]==maze->goal[0] && Pointer_Node->position[1]==maze->goal[1])
            {
                cout<<"I found the outlet"<<endl;;
                ScreenOutPut(maze);
                exit(1);
            }
            
            maze->ExpandNode(Pointer_Node);            //expand the node;
            maze->setpoint(Pointer_Node->position,'A');     //mark that this point has been explored with 'A'

            for(int i=0;i<4;i++)
            {
                //remember the parent node;
                
                if (Pointer_Node->child[i]!=NULL)
                {
                    NodeFrontier_Next.push_back(Pointer_Node->child[i]);
                }
            }
            
        }
        NodeFrontier=NodeFrontier_Next;
        NodeFrontier_Next.clear();
  
 //       ScreenOutPut(maze);
 //       cout<<endl<<endl;;
        
    }
}


void greedysearch(Tree_Node *root, MazeMap *maze)
{
    int x,y;
    vector<Tree_Node*> NodeFrontier;
    vector<Tree_Node*> NodeFrontier_Next;
    
    Tree_Node *PotentialExpandNode;                     //the node whose distance is smallest.
    Tree_Node *Pointer_Node;
    
    int totaltime=0;
    
    //initialize the root
    Pointer_Node=root;
    maze->SetDistToGoal(Pointer_Node);                  //set the distance of root node
    Pointer_Node->parents=Pointer_Node;                 //the parent of RootNode is RootNode
    
    
    //at start,the potential expand node is root node.
    PotentialExpandNode=Pointer_Node;
    
    x=root->position[0];
    y=root->position[1];
    
    NodeFrontier.push_back(root);
    
    //Greedy Search
    while (totaltime<10000000)
    {
        
        //Find the node to expand
        PotentialExpandNode=NodeFrontier[0];
        
        for(int i=0;i<NodeFrontier.size();i++)
        {
            Pointer_Node=NodeFrontier[i];
            maze->setpoint(Pointer_Node->position,'A');     //mark that this point has been explored with 'A'\
            
            if(Pointer_Node->position[0]==maze->goal[0] && Pointer_Node->position[1]==maze->goal[1])
            {
                cout<<"I found the outlet"<<endl;;
                ScreenOutPut(maze);
                exit(1);
            }
            
            
            if (Pointer_Node->distance<PotentialExpandNode->distance) {
                PotentialExpandNode=Pointer_Node;
            }
            
        
        }
        
//        ScreenOutPut(maze);
//        cout<<endl<<endl;;
        
        //expand node and create New frontier
        for(int i=0;i<NodeFrontier.size();i++)
        {
            Pointer_Node=NodeFrontier[i];
            
            if (Pointer_Node==PotentialExpandNode)       //
            {
                maze->ExpandNode(Pointer_Node);                             //expand the node;    
                for (int i=0; i<4; i++)
                {
                    if(Pointer_Node->child[i]!=NULL)
                    {
                        NodeFrontier_Next.push_back(Pointer_Node->child[i]);              //push the childnode into the next vector
                    }
                }

            }
            else
                NodeFrontier_Next.push_back(Pointer_Node);

        }
        NodeFrontier=NodeFrontier_Next;
        NodeFrontier_Next.clear(); 
    }
 }

void Astarsearch(Tree_Node *root, MazeMap *maze,MazeMap *maze0)
{
    int x,y;
    vector<Tree_Node*> NodeFrontier;                    //the current Node Frontier.
    vector<Tree_Node*> NodeFrontier_Next;               //the next Node Frontier.
    
    Tree_Node *PotentialExpandNode;                     //the node whose distance is smallest.
    Tree_Node *Pointer_Node;
    
    int totaltime=0;
    
    //initialize the root
    Pointer_Node=root;
    maze->SetDistToGoal(Pointer_Node);
    maze->SetFuncValue(Pointer_Node);
    Pointer_Node->parents=Pointer_Node;                 //the parent of RootNode is RootNode
    
    
    //at start,the potential expand node is root node.
    PotentialExpandNode=Pointer_Node;
    
    x=root->position[0];
    y=root->position[1];
    
    NodeFrontier.push_back(root);
    
    //Astar Search
    while (totaltime<10000000)
    {
        //Find the node to expand
        PotentialExpandNode=NodeFrontier[0];
        
        for(int i=0;i<NodeFrontier.size();i++)
        {
            Pointer_Node=NodeFrontier[i];
                       
            if(Pointer_Node->position[0]==maze->goal[0] && Pointer_Node->position[1]==maze->goal[1])
            {
                cout<<"I found the outlet"<<endl;
                ScreenOutPut(maze);
                report(Pointer_Node,maze,maze0);
                exit(1);
            }
            
            maze->setpoint(Pointer_Node->position,'+');     //mark that this point has been explored with 'A'\

            if (Pointer_Node->Function<PotentialExpandNode->Function) {
                PotentialExpandNode=Pointer_Node;
            }
        }
        
                
//        ScreenOutPut(maze);             //output current state.
//        cout<<endl<<endl;;
        
        //expand node and create New frontier
        for(int i=0;i<NodeFrontier.size();i++)
        {
            Pointer_Node=NodeFrontier[i];
            
            if (Pointer_Node==PotentialExpandNode)       //
            {
                maze->ExpandNode(Pointer_Node);                             //expand the node;
                for (int i=0; i<4; i++)
                {
                    if(Pointer_Node->child[i]!=NULL)
                    {
                        NodeFrontier_Next.push_back(Pointer_Node->child[i]);              //push the childnode into the next vector
                    }
                }
                
            }
            
            else
                NodeFrontier_Next.push_back(Pointer_Node);
            
        }
        
        NodeFrontier=NodeFrontier_Next;
        NodeFrontier_Next.clear();
    }
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
    }
    
    for (int y=0; y<maze0->length; y++) {
        for(int x=0;x<maze0->width+1;x++)
            fout<<maze0->findpoint(x, y);
    }
    
       
    
    ScreenOutPut(maze0);
    
    //read a clear map

    fout.close();
}

