//
//  Map.cpp
//  CS440-HW1
//
//  Created by Xu Wenzhao on 9/18/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#include <string.h>
#include "Map.h"
#include <fstream>
#include <iomanip.h>
#include <math.h>

using namespace std;

MazeMap::MazeMap(int length_maze,int width_maze,string filename)
{
 
    char point=0;
    length=length_maze;
    width=width_maze;
    
    mapvalue=new char[length*(width+1)];
      
    //read_date
    
    ifstream fin;
    ofstream fout;
    string filenameout,filenamein;
    totaapea=0;
    
    filenamein=filename+".lay";
    
    outname=filename+"_result.txt";
    inname=filenamein;
    
    fin.open(filenamein.c_str(),ios::in);
    fout.open("/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW1/CS440-HW1/map.txt",ios::out);
    
    if (!fin)
    {
        cout<<"No file found"<<endl;
        exit(0);
    }
    
    int fileposition[2];
    
    for(int y=0;y<length;y++)
    {
        for(int x=0;x<width+1;x++)
        {
            //read

            point=fin.get();

            fileposition[0]=x;
            fileposition[1]=y;
            setpoint(fileposition, point);
            
            if (point=='P')             //if the point is . then it is the start
            {
                start[0]=x;
                start[1]=y;
            }            
            if (point=='.')             //if the point is P, then it is the goal
            {
                goal[0]=x;
                goal[1]=y;
                totaapea+=1;            //in pea eating problem, count the total pea in the maze;
                goal_pea=new int[2];
                goal_pea[0]=x;
                goal_pea[1]=y;
                PeaPosition.push_back(goal_pea);        //store the position of pea in the maze
                
            }
        }
    }
    
    fin.close();
    
    for (int y=0; y<length; y++) {
        for(int x=0;x<width+1;x++)
            fout<<*(mapvalue+(width+1)*y+x);
    }

    fout.close();
    
    root=new Tree_Node(start[0],start[1],0);
    ExpandedNode=0;

}

char MazeMap::findpoint(int x,int y)
{
    char value;
    value=*(mapvalue+(width+1)*y+x);
    return value;
}


void MazeMap::setpoint(int *p,char x)
{
    *(mapvalue+(width+1)*(*(p+1))+(*p))=x;
}

void MazeMap::ExpandNode(Tree_Node *point)
{
    char value='c';
    int nearpoint[4][2];    //the adjecant points of a Node
    
    if (point->expandingnodenum<1)                //the node hasn't been expand, expand it.
    {
        for (int i=0; i<4; i++)
        {
            for (int j=0; j<2; j++)
            {
                nearpoint[i][j]=point->position[j]; //assign the four nearpoint with the original position
            }
        }
        
        nearpoint[0][1]=nearpoint[0][1]-1;      //up point
        nearpoint[1][1]=nearpoint[1][1]+1;      //down point
        nearpoint[2][0]=nearpoint[2][0]+1;      //right point
        nearpoint[3][0]=nearpoint[3][0]-1;      //left point
        
        for (int i=0; i<4; i++)
        {
            value=findpoint(nearpoint[i][0],nearpoint[i][1]);
            
            if (value==' '||value=='.')                                         //if the near point is a availble way to go
            {

                point->child[i]=new Tree_Node(nearpoint[i][0],nearpoint[i][1],0);     //expand the new node
                
                point->child[i]->parents=point;
                point->child[i]->depth=point->depth+1;
                point->expandingnodenum+=1;             //set the number of available children
                
                SetPathCost(point->child[i]);
                SetDistToGoal(point->child[i]);
                SetFuncValue(point->child[i]);
            }
            
            
        }
    }
    
}

void MazeMap::ExpandNode_Pea(Tree_Node *point)
{
    char value='c';
    int nearpoint[4][2];    //the adjecant points of a Node
    
    
    if (point->expandingnodenum<1)                //the node hasn't been expand, expand it.
    {
        for (int i=0; i<4; i++)
        {
            for (int j=0; j<2; j++)
            {
                nearpoint[i][j]=point->position[j]; //assign the four nearpoint with the original position
            }
        }
        
        nearpoint[0][1]=nearpoint[0][1]-1;      //up point
        nearpoint[1][1]=nearpoint[1][1]+1;      //down point
        nearpoint[2][0]=nearpoint[2][0]+1;      //right point
        nearpoint[3][0]=nearpoint[3][0]-1;      //left point
        
        for (int i=0; i<4; i++)
        {
            value=findpoint(nearpoint[i][0],nearpoint[i][1]);
            
            if (value==' '||(value=='.'|| value=='P'))                                         //if the near point is availble way to go
            {
                
                point->child[i]=new Tree_Node(nearpoint[i][0],nearpoint[i][1],0);     //expand the new node
                
                point->child[i]->parents=point;
                point->child[i]->depth=point->depth+1;
                
                point->child[i]->PeaList=point->PeaList;                            //inherent the pea list from parents
                point->child[i]->IEatAPea=point->IEatAPea;                          //inherent the num of pea eaten from parents
                
                point->expandingnodenum+=1;             //set the number of available children
                
                SetPathCost(point->child[i]);

                
                if(value=='.')
                {
                    EatingPea(point->child[i]);
                }
                
                point->child[i]->distance=FutureDistance(point->child[i]);
                SetFuncValue(point->child[i]);
                
            }
        }
    }
    
}

void MazeMap::SetDistToGoal(Tree_Node *point)
{
    int delx,dely;
    delx=abs(goal[0]-point->position[0]);
    dely=abs(goal[1]-point->position[1]);
    
    point->distance=delx+dely;
}

void MazeMap::SetPathCost(Tree_Node *point)
{
    double stepcost=1;
    
//    stepcost=pow(2.0,double(point->position[0]));
//     stepcost=pow(0.5,double(point->position[0]));
    
    point->path_cost=point->parents->path_cost+stepcost;
}

void MazeMap::SetFuncValue(Tree_Node *point)
{
    point->Function=point->path_cost+point->distance;
}


void MazeMap::EatingPea(Tree_Node *point)
{
    int Tempx,Tempy;
    bool Newpea=1;
    //check whether it is a old pea

    for(int i=0;i<(point->PeaList.size());i++)
    {
        Tempx=point->PeaList[i][0];
        Tempy=point->PeaList[i][1];
        
        if (point->position[0]==Tempx && point->position[1]==Tempy)   //This pea has been eaten
        {
            Newpea=0;
            break;
        }
    }
    
    if(Newpea==1)
    {
        point->IEatAPea+=1;                                     //This branch eat one more pea
        point->PeaList.push_back(point->position);              //Push a new node into the pea list
    }
}

double MazeMap::FutureDistance(Tree_Node *node)             //this is a function to return the distance to a nearest new peas or other distance
{
    double mindistance,temp;
    double maxdistance=0;
    bool newpea=1;
    mindistance=1000000;
    temp=999;
    
    for (int i=0; i<PeaPosition.size(); i++) {
        newpea=1;
        for (int j=0; j<node->PeaList.size(); j++) {
            if (PeaPosition[i][0]==node->PeaList[j][0] && PeaPosition[i][1]==node->PeaList[j][1]) {
                newpea=0;
                break;
            }
        }
        if(newpea==1)
        {
            temp=abs(PeaPosition[i][0]-node->position[0])+abs(PeaPosition[i][1]-node->position[1]);
        
            if (temp<mindistance)
                mindistance=temp;
            if (temp>maxdistance)
                maxdistance=temp;
        }
    }
//      return  maxdistance-mindistance;
//        return maxdistance;
        return mindistance;
}


