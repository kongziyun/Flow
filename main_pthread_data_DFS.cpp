//
//  main.cpp
//  Flow
//
//  Created by Ziyun Kong on 12/11/13.
//  Copyright (c) 2013 Ziyun Kong. All rights reserved.
//

#include <iomanip>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <pthread.h>
#include <omp.h>
#include <ctime>
#define EMPTY NULL

#define combTosep(a,b,c) \
  b = a % size; c = a / size;

#define sepTocomb(a,b,c)\
  a = b + c * size;


typedef std::map<std::pair<int, int>, std::string> MapInfo;
typedef std::map<int, int> MapIntT;

double factor;
int numThreads;
pthread_t* thread;
int size;
bool found;
omp_lock_t lockFound;
bool *threadUsing;
double elapseTime;

struct PointT {
  int x;
  int y;
  PointT(){}
  PointT(const PointT &p_):x(p_.x), y(p_.y){}
  PointT(int x_, int y_):x(x_), y(y_){}
};

struct DotPairT {
  int colorIdx;
  int loc1;
  int loc2;
  DotPairT(int color_, int p1_, int p2_)
  :colorIdx(color_), loc1(p1_), loc2(p2_){}
};

struct DotT{
  std::string color;
  PointT loc;
  DotT(){}
  DotT(std::string c_, PointT l_):color(c_), loc(l_){}
  DotT(DotT &p_):color(p_.color), loc(p_.loc){}
};

struct PathNodeT {
  int cur;
  int devX;
  int devY;
  MapIntT pathMap;
  PathNodeT(){}
  PathNodeT(int cur_, MapIntT map_, int devX_ = 0, int devY_ = 1): cur(cur_), pathMap(map_), devX(devX_), devY(devY_){}
};

struct mapInfoT {
  std::set<std::pair<int, int> > uniqueMap;
  std::vector<DotT> colorDotMap;
};

struct intNodeT{
  int curLoc;
  std::vector<int> path;
  intNodeT(int l_, std::vector<int> p_):curLoc(l_), path(p_){}
};

struct pthreadInput {
  int tid;
  //std::map<std::string, DotPairT*>::iterator map_it;
  int colorIdx;
  MapIntT mapInfo;
  pthreadInput(int t_, int i_, MapIntT m_): tid(t_), colorIdx(i_), mapInfo(m_){}
};


std::vector<DotPairT*> Dots;
std::map<int, DotPairT*> DotsMap;

std::vector<std::string> colorList;
MapIntT initMapInfo;

MapIntT* solution;

void* analyzeDotPair(void* input);

int getNextId();
/*
inline void combTosep(int comb, int &x, int &y){
  x = comb % size;
  y = comb / size;
  return;
}

inline void sepTocomb(int &comb, int x, int y){
  comb = x + y * size;
  return;
}
*/

bool isCollide(PathNodeT* path, int pointX, int pointY){
//  int pointX, pointY;
  //combTosep(point, pointX, pointY);
  
  if (pointX < 0 || pointX >= size || pointY < 0 || pointY >= size)
    return true;
  int comb;
  sepTocomb(comb, pointX, pointY);
  if (path->pathMap.find(comb) != path->pathMap.end())
    return true;
  
  return false;
}

void printMap(MapIntT* map){
  int comb;
  for (int i = 0 ; i < size; ++i){
    for (int j = 0; j < size; ++j){
      sepTocomb(comb, j, i);
      std::cout <<"[";
      if (map->find(comb) != map->end()){
        std::cout.width(10);
        std::cout<<std::left<< colorList[(*map)[comb]]<<"]";
      }
      else{
        std::cout.width(10);
        std::cout<<std::left<<" "<<"]";
      }
    }
    std::cout << "\n";
  }
  std::cout << "\n";

}

bool searchPath(int idx, MapIntT *curMap){
  int startComb = DotsMap[idx]->loc1;
  int endComb = DotsMap[idx]->loc2;
  int endX;
  int endY;
  combTosep(endComb, endX, endY)

  bool map[size][size];
  for (int i = 0; i < size; ++i)
    for (int j = 0; j < size; ++j)
      map[i][j] = false;
  
  int x, y;
  for (MapIntT::iterator it = curMap->begin();
       it != curMap->end();
       ++it){
    combTosep(it->first, x, y)
    map[x][y] = true;
  }
  map[endX][endY] = false;
  std::queue<int> BFS;
  MapIntT tmpMap;

  int curPointX;
  int curPointY;
  int curPoint;
  
  int nextPointX;
  int nextPointY;
  int nextPoint;
  
  //MapInfo* tmpSolution;
  
  //curPathNode.pathMap = *curMap;

  BFS.push(startComb);
  while (!BFS.empty()){
    curPoint = BFS.front();
    combTosep(curPoint, curPointX, curPointY);
    
    BFS.pop();
    /*
     if ((tmpMap.size() - curMapInfo.size()) >= (size * size - curMapInfo.size()) * factor){
     delete curPathNode;
     break;
     }
     */
    
    nextPointX = curPointX - 1;
    nextPointY = curPointY;
    
    if (nextPointX == endX && nextPointY == endY){
      return true;
      
    }
    else{
      if (nextPointX >= 0 && nextPointX < size &&
          nextPointY >= 0 && nextPointY < size && !map[nextPointX][nextPointY])
      {
        map[nextPointX][nextPointY] = true;
        sepTocomb(nextPoint, nextPointX, nextPointY)
        BFS.push(nextPoint);
      }
    }
    
    
    nextPointX = curPointX + 1;
    nextPointY = curPointY;
    if (nextPointX == endX && nextPointY == endY){
      return true;
      
    }
    else{
      if (nextPointX >= 0 && nextPointX < size &&
          nextPointY >= 0 && nextPointY < size && !map[nextPointX][nextPointY])
      {
        map[nextPointX][nextPointY] = true;
        sepTocomb(nextPoint, nextPointX, nextPointY)
        BFS.push(nextPoint);
      }
    }
    
    nextPointX = curPointX;
    nextPointY = curPointY - 1;
    if (nextPointX == endX && nextPointY == endY){
      return true;
      
    }
    else{
      if (nextPointX >= 0 && nextPointX < size &&
          nextPointY >= 0 && nextPointY < size && !map[nextPointX][nextPointY])
      {
        map[nextPointX][nextPointY] = true;
        sepTocomb(nextPoint, nextPointX, nextPointY)
        BFS.push(nextPoint);
      }
    }
    
    nextPointX = curPointX;
    nextPointY = curPointY + 1;
    if (nextPointX == endX && nextPointY == endY){
      return true;
      
    }
    else{
      if (nextPointX >= 0 && nextPointX < size &&
          nextPointY >= 0 && nextPointY < size && !map[nextPointX][nextPointY])
      {
        map[nextPointX][nextPointY] = true;
        sepTocomb(nextPoint, nextPointX, nextPointY)
        BFS.push(nextPoint);
      }
    }
  }
  return false;
}

void printSolution(){
  elapseTime = omp_get_wtime() - elapseTime;
  std::cout<<std::endl;
  int comb;
  if (solution){
    std::cout <<"time is:"<<elapseTime<< std::endl;
    for (int i = 0 ; i < size; ++i){
      for (int j = 0; j < size; ++j){
        sepTocomb(comb, j, i);
        std::cout <<"[";
        std::cout.width(10);
        std::cout<<std::left<< colorList[(*solution)[comb]]<<"]";
      }
      std::cout << std::endl;
    }
  }
  exit(0);
}


int getNextId(){
  for (int i = 0; i < numThreads;++i)
    if (!threadUsing[i])
      return i;
  return numThreads;
}

void setDot(std::string c_, int p1_, int p2_){
  if (c_.empty()){
    std::cerr << "Error: Color name is empty!"<<std::endl;
    exit(1);
  }
  
  Dots.push_back(new DotPairT(colorList.size(), p1_, p2_));
  colorList.push_back(c_);
  return;
}

void setMap(){
  for (int i = 0; i < Dots.size(); ++i){
    initMapInfo.insert(std::pair<int, int>(Dots[i]->loc1, Dots[i]->colorIdx));
    initMapInfo.insert(std::pair<int, int>(Dots[i]->loc2, Dots[i]->colorIdx));
    DotsMap.insert(std::pair<int, DotPairT*>(Dots[i]->colorIdx, Dots[i]));
  }
  return;
}



void* analyzeDotPair(void* input){
  struct pthreadInput * data = (struct pthreadInput *)input;
  
  int startComb = DotsMap[data->colorIdx]->loc1;
  int endComb = DotsMap[data->colorIdx]->loc2;
  int endX;
  int endY;
  combTosep(endComb, endX, endY)
  
  std::stack<PathNodeT> DFS;
  MapIntT tmpMap;
  PathNodeT curPathNode;
  int curPointX;
  int curPointY;
  int curPoint;
  
  int nextPointX;
  int nextPointY;
  int nextPoint;

  
  curPathNode.cur = startComb;
  curPathNode.pathMap = data->mapInfo;
  curPathNode.devX = 0;
  curPathNode.devY = 1;
  int tmpIdx = data->colorIdx;
  ++tmpIdx;
  int count = 0;
  bool insert;
  DFS.push(curPathNode);
  while (!DFS.empty() && !found){
    curPathNode = DFS.top();
    curPoint = curPathNode.cur;
    tmpMap = curPathNode.pathMap;
    combTosep(curPoint, curPointX, curPointY);
    
    DFS.pop();
    /*
     if ((tmpMap.size() - curMapInfo.size()) >= (size * size - curMapInfo.size()) * factor){
     delete curPathNode;
     break;
     }
     */
    int rc;
    
    nextPointX = curPointX - 1;
    nextPointY = curPointY;

    if (nextPointX == endX && nextPointY == endY){
      std::cout<<"found \t"<<colorList[data->colorIdx]<<"\t"<<++count<<std::endl;
      sepTocomb(nextPoint, nextPointX, nextPointY)
      tmpMap = curPathNode.pathMap;
      tmpMap.insert(std::pair<int, int>(std::pair<int, int>(nextPoint, data->colorIdx)));
      if (tmpIdx == colorList.size()){
        if (tmpMap.size() == size * size){
          found = true;
          solution =  new MapIntT(tmpMap);
          
          printSolution();
          
          if (data->tid != numThreads){
            delete data;
            pthread_exit(NULL);
          }
          else{
            delete data;
            return NULL;
          }
        }
      }
      else{
        int nId = getNextId();
        if (nId != numThreads){
          rc = pthread_create(&thread[nId], NULL, analyzeDotPair, (void*) (new pthreadInput(nId, tmpIdx, tmpMap)));
          threadUsing[data->tid] = true;
        }
        else
          analyzeDotPair((void*) (new pthreadInput(nId, tmpIdx, tmpMap)));
        
        if (found){
          if (data->tid != numThreads){
            threadUsing[data->tid] = false;
            delete data;
            
            pthread_exit(NULL);
          }
          else{
            delete data;
            return NULL;
          }
        }
      }
    }
    else{
      if (!isCollide(&curPathNode, nextPointX, nextPointY)){
        sepTocomb(nextPoint, nextPointX, nextPointY)
        tmpMap = curPathNode.pathMap;
        tmpMap.insert(std::pair<int, int>(std::pair<int, int>(nextPoint, data->colorIdx)));
        insert = true;
        for (int i = data->colorIdx + 1; i < colorList.size(); ++i)
          if (!searchPath( i, &tmpMap)){
            insert = false;
            break;
          }
        if (insert){
          DFS.push(PathNodeT(nextPoint, tmpMap));
          printMap(&tmpMap);

        }
        else
          std::cout<<colorList[data->colorIdx]<<"\tbad insertion\t"<<DFS.size()<<"\n";

      }
    }
    
   
    nextPointX = curPointX + 1;
    nextPointY = curPointY;
    if (nextPointX == endX && nextPointY == endY){
      std::cout<<"found \t"<<colorList[data->colorIdx]<<"\t"<<++count<<std::endl;

      sepTocomb(nextPoint, nextPointX, nextPointY)
      tmpMap = curPathNode.pathMap;
      tmpMap.insert(std::pair<int, int>(std::pair<int, int>(nextPoint, data->colorIdx)));
      if (tmpIdx == colorList.size()){
        if (tmpMap.size() == size * size){
          found = true;
          solution =  new MapIntT(tmpMap);
          
          printSolution();
          
          if (data->tid != numThreads){
            delete data;
            pthread_exit(NULL);
          }
          else{
            delete data;
            return NULL;
          }
        }
      }
      else{
        int nId = getNextId();
        if (nId != numThreads){
          rc = pthread_create(&thread[nId], NULL, analyzeDotPair, (void*) (new pthreadInput(nId, tmpIdx, tmpMap)));
          threadUsing[data->tid] = true;
        }
        else
          analyzeDotPair((void*) (new pthreadInput(nId, tmpIdx, tmpMap)));
        
        if (found){
          if (data->tid != numThreads){
            threadUsing[data->tid] = false;
            delete data;
            
            pthread_exit(NULL);
          }
          else{
            delete data;
            return NULL;
          }
        }
      }
    }
    else{
      if (!isCollide(&curPathNode, nextPointX, nextPointY)){
        sepTocomb(nextPoint, nextPointX, nextPointY)
        tmpMap = curPathNode.pathMap;
        tmpMap.insert(std::pair<int, int>(std::pair<int, int>(nextPoint, data->colorIdx)));
        insert = true;
        for (int i = data->colorIdx + 1; i < colorList.size(); ++i)
          if (!searchPath( i, &tmpMap)){
            insert = false;
            break;
          }
        if (insert){
          DFS.push(PathNodeT(nextPoint, tmpMap));
          printMap(&tmpMap);

        }
        else
          std::cout<<colorList[data->colorIdx]<<"\tbad insertion\t"<<DFS.size()<<"\n";

      }
    }
    
    
    nextPointX = curPointX;
    nextPointY = curPointY - 1;
    if (nextPointX == endX && nextPointY == endY){
      std::cout<<"found \t"<<colorList[data->colorIdx]<<"\t"<<++count<<std::endl;

      sepTocomb(nextPoint, nextPointX, nextPointY)
      tmpMap = curPathNode.pathMap;
      tmpMap.insert(std::pair<int, int>(std::pair<int, int>(nextPoint, data->colorIdx)));
      if (tmpIdx == colorList.size()){
        if (tmpMap.size() == size * size){
          found = true;
          solution =  new MapIntT(tmpMap);
          
          printSolution();
          
          if (data->tid != numThreads){
            delete data;
            pthread_exit(NULL);
          }
          else{
            delete data;
            return NULL;
          }
        }
      }
      else{
        int nId = getNextId();
        if (nId != numThreads){
          rc = pthread_create(&thread[nId], NULL, analyzeDotPair, (void*) (new pthreadInput(nId, tmpIdx, tmpMap)));
          threadUsing[data->tid] = true;
        }
        else
          analyzeDotPair((void*) (new pthreadInput(nId, tmpIdx, tmpMap)));
        
        if (found){
          if (data->tid != numThreads){
            threadUsing[data->tid] = false;
            delete data;
            
            pthread_exit(NULL);
          }
          else{
            delete data;
            return NULL;
          }
        }
      }
    }
    else{
      if (!isCollide(&curPathNode, nextPointX, nextPointY)){
        sepTocomb(nextPoint, nextPointX, nextPointY)
        tmpMap = curPathNode.pathMap;
        tmpMap.insert(std::pair<int, int>(std::pair<int, int>(nextPoint, data->colorIdx)));
        insert = true;
        for (int i = data->colorIdx + 1; i < colorList.size(); ++i)
          if (!searchPath( i, &tmpMap)){
            insert = false;
            break;
          }
        if (insert){
          DFS.push(PathNodeT(nextPoint, tmpMap));
          printMap(&tmpMap);
        }
        else
          std::cout<<colorList[data->colorIdx]<<"\tbad insertion\t"<<DFS.size()<<"\n";

      }
    }
    
    
    nextPointX = curPointX;
    nextPointY = curPointY + 1;
    if (nextPointX == endX && nextPointY == endY){
      std::cout<<"found \t"<<colorList[data->colorIdx]<<"\t"<<++count<<std::endl;

      sepTocomb(nextPoint, nextPointX, nextPointY)
      tmpMap = curPathNode.pathMap;
      tmpMap.insert(std::pair<int, int>(std::pair<int, int>(nextPoint, data->colorIdx)));
      if (tmpIdx == colorList.size()){
        if (tmpMap.size() == size * size){
          found = true;
          solution =  new MapIntT(tmpMap);
          
          printSolution();
          
          if (data->tid != numThreads){
            delete data;
            pthread_exit(NULL);
          }
          else{
            delete data;
            return NULL;
          }
        }
      }
      else{
        int nId = getNextId();
        if (nId != numThreads){
          rc = pthread_create(&thread[nId], NULL, analyzeDotPair, (void*) (new pthreadInput(nId, tmpIdx, tmpMap)));
          threadUsing[data->tid] = true;
        }
        else
          analyzeDotPair((void*) (new pthreadInput(nId, tmpIdx, tmpMap)));
        
        if (found){
          if (data->tid != numThreads){
            threadUsing[data->tid] = false;
            delete data;
            
            pthread_exit(NULL);
          }
          else{
            delete data;
            return NULL;
          }
        }
      }
    }
    else{
      if (!isCollide(&curPathNode, nextPointX, nextPointY)){
        sepTocomb(nextPoint, nextPointX, nextPointY)
        tmpMap = curPathNode.pathMap;
        tmpMap.insert(std::pair<int, int>(std::pair<int, int>(nextPoint, data->colorIdx)));
        insert = true;
        for (int i = data->colorIdx + 1; i < colorList.size(); ++i)
          if (!searchPath( i, &tmpMap)){
            insert = false;
            break;
          }
        if (insert){
          DFS.push(PathNodeT(nextPoint, tmpMap));
          printMap(&tmpMap);

        }
        else
          std::cout<<colorList[data->colorIdx]<<"\tbad insertion\t"<<DFS.size()<<"\n";
      }
    }
  }
  
  // Add one path to map and analyze next dot pair
  //solution = NULL;
  if (data->tid != numThreads){
    threadUsing[data->tid] = false;
    delete data;
    pthread_exit(NULL);
  }
  else{
    delete data;
    return NULL;
  }
}

void setSize(int s){
  size = s;
}
void setThreadNum(int a){
  threadUsing = new bool[a];
  for (int i = 0; i < a; ++i)
    threadUsing[i] = false;
}


int main(int argc, const char * argv[])
{
  int sizeInput;

  std::cin >> sizeInput;
  setSize(sizeInput);
  PointT a;
  PointT b;
  int aComb;
  int bComb;
  solution = NULL;
/*
#pragma omp parallel
  {
    numThreads = omp_get_num_threads();
//    std::cout << omp_get_num_threads()<<std::endl;
  }
 */
  std::string color;
  std::cin >> color;
  while (color != "quit"){
    std::cin >> a.x;
    std::cin >> a.y;
    std::cin >> b.x;
    std::cin >> b.y;
    sepTocomb(aComb, a.x, a.y)
    sepTocomb(bComb, b.x, b.y)
    setDot(color, aComb, bComb);
    std::cin >> color;
  }
  setMap();
  
  

  factor = std::atof(argv[1]);
  numThreads = std::atoi(argv[2]) - 1;
  thread = new pthread_t[numThreads];
  setThreadNum(numThreads);
  elapseTime = omp_get_wtime();
  //flowMap.generatePaths();
  //flowMap.printPath("red");
  //analyzeFlowMap();
  //std::vector<std::map<std::pair<int, int>, std::string> > solution = flowMap.analyzePaths();
  analyzeDotPair((void*) (new pthreadInput(numThreads, 0, initMapInfo)));

//    return 0;
  pthread_exit(NULL);
}

