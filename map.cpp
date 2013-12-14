//
//  map.cpp
//  Flow
//
//  Created by Ziyun Kong on 12/11/13.
//  Copyright (c) 2013 Ziyun Kong. All rights reserved.
//

#include "map.h"

void Map::setDot(std::string c_, PointT p1_, PointT p2_){
  if (c_.empty()){
    std::cerr << "Error: Color name is empty!"<<std::endl;
    exit(1);
  }
  
  DotsMap.insert(std::pair<std::string, DotPairT*>(c_, new DotPairT(c_, p1_, p2_)));
  Dots.push_back(new DotPairT(c_, p1_, p2_));
  
  return;
}

void Map::setMap(){
  FlowMap = new std::string *[size];
  for (int i = 0; i < size; ++i)
    FlowMap[i] = new std::string[size];

  //initMap = new std::set<std::pair<int, int> >;
  for (std::map<std::string, DotPairT*>::iterator it = DotsMap.begin();
       it != DotsMap.end();
       ++it){
    FlowMap[it->second->loc1.x][it->second->loc1.y] = it->first;
    FlowMap[it->second->loc2.x][it->second->loc2.y] = it->first;
    initMapInfo.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(it->second->loc1.x, it->second->loc1.y),it->first));
    initMapInfo.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(it->second->loc2.x, it->second->loc2.y),it->first));
  }
  return;
}

std::vector<std::map<std::pair<int, int>, std::string> > Map::analyzeFlowMap(){
  bool found = false;
  std::vector<std::map<std::pair<int, int>, std::string> > solution = analyzeDotPair(DotsMap.begin(), initMapInfo, found);
  return solution;
}

bool Map::isCollide(PathNodeT* path, PointT point, std::map<std::pair<int, int>, std::string> currentMap){
  if (point.x < 0 || point.x >= size || point.y < 0 || point.y >= size)
    return true;
  /*
  while (path){
    if (point.x == path->cur.x && point.y == path->cur.y)
      return true;
    path = path->prev;
  }
  
  if (currentMap.find(std::pair<int, int>(point.x, point.y)) != currentMap.end())
    return true;
  */
  if (path->pathMap.find(std::pair<int, int>(point.x, point.y)) != path->pathMap.end())
    return true;
  
  return false;
}

std::vector<std::map<std::pair<int, int>, std::string> > Map::analyzeDotPair(std::map<std::string, DotPairT*>::iterator map_it, std::map<std::pair<int, int>, std::string> curMapInfo, bool &found){
  if (found){
    std::vector<std::map<std::pair<int, int>, std::string> > something;
    return something;
  }

  std::vector<PathNodeT*>* pathList = new std::vector<PathNodeT*>;
  
  PointT start = map_it->second->loc1;
  PointT end = map_it->second->loc2;
  
  std::queue<PathNodeT*> BFS;
  std::map<std::pair<int, int>, std::string> tmpMap;
  PathNodeT* curPathNode = new PathNodeT;
  PointT curPoint;
  PointT nextPoint;
  
  curPathNode->prev = NULL;
  curPathNode->cur = start;
  curPathNode->pathMap = curMapInfo;
  
  BFS.push(curPathNode);
  
  while (!BFS.empty()){
    curPathNode = BFS.front();
    curPoint = curPathNode->cur;
    tmpMap = curPathNode->pathMap;
    BFS.pop();
    /*
    if (curPoint.x == end.x && curPoint.y == end.y){
      pathList->push_back(curPathNode);
      continue;
    }
    */
    nextPoint =PointT(curPoint.x - 1, curPoint.y);
    if (nextPoint.x == end.x && nextPoint.y == end.y){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), map_it->first));
      pathList->push_back(new PathNodeT(curPathNode, nextPoint, tmpMap));
      continue;
    }
    if (!isCollide(curPathNode, nextPoint, curMapInfo)){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), map_it->first));
      BFS.push(new PathNodeT(curPathNode, nextPoint, tmpMap));
    }
    
    
    nextPoint =PointT(curPoint.x + 1, curPoint.y);
    if (nextPoint.x == end.x && nextPoint.y == end.y){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), map_it->first));
      pathList->push_back(new PathNodeT(curPathNode, nextPoint, tmpMap));
      continue;
    }
    if (!isCollide(curPathNode, nextPoint, curMapInfo)){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), map_it->first));
      BFS.push(new PathNodeT(curPathNode, nextPoint, tmpMap));
    }
    
    nextPoint =PointT(curPoint.x, curPoint.y - 1);
    if (nextPoint.x == end.x && nextPoint.y == end.y){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), map_it->first));
      pathList->push_back(new PathNodeT(curPathNode, nextPoint, tmpMap));
      continue;
    }
    if (!isCollide(curPathNode, nextPoint, curMapInfo)){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), map_it->first));
      BFS.push(new PathNodeT(curPathNode, nextPoint, tmpMap));
    }
    
    nextPoint =PointT(curPoint.x, curPoint.y + 1);
    if (nextPoint.x == end.x && nextPoint.y == end.y){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), map_it->first));
      pathList->push_back(new PathNodeT(curPathNode, nextPoint, tmpMap));
      continue;
    }
    if (!isCollide(curPathNode, nextPoint, curMapInfo)){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), map_it->first));
      BFS.push(new PathNodeT(curPathNode, nextPoint, tmpMap));
    }
    
  }
  // Add one path to map and analyze next dot pair
  std::map<std::pair<int, int>, std::string> nextMapInfo;
  
  std::vector<std::map<std::pair<int, int>, std::string> > solutions;
  std::vector<std::map<std::pair<int, int>, std::string> > tmpSolutions;
  PathNodeT* tmp;
  for (std::vector<PathNodeT*>::iterator it = pathList->begin(); it != pathList->end(); ++it) {
    nextMapInfo = (*it)->pathMap;
    /*
    tmp = *it;
    while(tmp){
      nextMapInfo.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(tmp->cur.x, tmp->cur.y), map_it->first));
      //nextColorMap.push_back(DotT(map_it->first, tmp->cur));
      tmp = tmp->prev;
    }*/
    std::map<std::string, DotPairT*>::iterator tmpIt = map_it;
    ++tmpIt;
    if (tmpIt == DotsMap.end()){
      solutions.push_back(nextMapInfo);
      /* Modified */
      found = true;
      return solutions;
      /* Modified */
    }
    else{
      tmpSolutions = analyzeDotPair(tmpIt, nextMapInfo, found);// Modified
      /* Modified */
      if (found){
        return tmpSolutions;
      }
        solutions.insert(solutions.end(), tmpSolutions.begin(), tmpSolutions.end());
    }
  }
  //std::cout<<"pass one "<<map_it->first<<std::endl;
  
  return solutions;
}

void Map::generatePathsForPair(std::string color){
  std::vector<PathNodeT*>* pathList = new std::vector<PathNodeT*>;
  
  PointT start = DotsMap[color]->loc1;
  PointT end = DotsMap[color]->loc2;
  
  std::queue<PathNodeT*> BFS;
  std::map<std::pair<int, int>, std::string> tmpMap;
  PathNodeT* curPathNode = new PathNodeT;
  PointT curPoint;
  PointT nextPoint;
  
  curPathNode->prev = NULL;
  curPathNode->cur = start;
  curPathNode->pathMap = initMapInfo;
  
  BFS.push(curPathNode);
  
  while (!BFS.empty()){
    curPathNode = BFS.front();
    curPoint = curPathNode->cur;
    tmpMap = curPathNode->pathMap;
    BFS.pop();
    /*
     if (curPoint.x == end.x && curPoint.y == end.y){
     pathList->push_back(curPathNode);
     continue;
     }
     */
    nextPoint =PointT(curPoint.x - 1, curPoint.y);
    if (nextPoint.x == end.x && nextPoint.y == end.y){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), color));
      pathList->push_back(new PathNodeT(curPathNode, nextPoint, tmpMap));
      continue;
    }
    if (!isCollide(curPathNode, nextPoint, initMapInfo)){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), color));
      BFS.push(new PathNodeT(curPathNode, nextPoint, tmpMap));
    }
    
    
    nextPoint =PointT(curPoint.x + 1, curPoint.y);
    if (nextPoint.x == end.x && nextPoint.y == end.y){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), color));
      pathList->push_back(new PathNodeT(curPathNode, nextPoint, tmpMap));
      continue;
    }
    if (!isCollide(curPathNode, nextPoint, initMapInfo)){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), color));
      BFS.push(new PathNodeT(curPathNode, nextPoint, tmpMap));
    }
    
    nextPoint =PointT(curPoint.x, curPoint.y - 1);
    if (nextPoint.x == end.x && nextPoint.y == end.y){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), color));
      pathList->push_back(new PathNodeT(curPathNode, nextPoint, tmpMap));
      continue;
    }
    if (!isCollide(curPathNode, nextPoint, initMapInfo)){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), color));
      BFS.push(new PathNodeT(curPathNode, nextPoint, tmpMap));
    }
    
    nextPoint =PointT(curPoint.x, curPoint.y + 1);
    if (nextPoint.x == end.x && nextPoint.y == end.y){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), color));
      pathList->push_back(new PathNodeT(curPathNode, nextPoint, tmpMap));
      continue;
    }
    if (!isCollide(curPathNode, nextPoint, initMapInfo)){
      tmpMap = curPathNode->pathMap;
      tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(nextPoint.x, nextPoint.y), color));
      BFS.push(new PathNodeT(curPathNode, nextPoint, tmpMap));
    }
  }
  
  std::vector<MapInfo> tmp;
  
  for (std::vector<PathNodeT*>::iterator it = pathList->begin();
       it != pathList->end();
       ++it)
    tmp.push_back((*it)->pathMap);
  
  possiblePaths[color] = pathList;
  
  return;
}

void Map::generatePaths(){
  for (std::map<std::string, DotPairT*>::iterator it = DotsMap.begin();
       it != DotsMap.end();
       ++it) {
    generatePathsForPair(it->first);
    std::cout<<"pass one "<<it->first<<"\t"<<possiblePaths[it->first]->size()<<std::endl;
  }
}
std::vector<MapInfo> Map::analyzePathsHelper(std::map<std::string, DotPairT*>::iterator map_it, MapInfo curMap){
  // Exception control;
  //std::cout<<"Analyze for "<<map_it->first<<std::endl;
  // Actual work;
  PathNodeT* tmp;
  std::vector<MapInfo> solutions;
  std::vector<MapInfo> tmpSol;
  for (std::vector<PathNodeT*>::iterator it = possiblePaths[map_it->first]->begin();
       it != possiblePaths[map_it->first]->end();
       ++it){
    tmp = (*it);
    bool collide = false;
    MapInfo tmpMap = curMap;
    while(tmp){
      if (tmpMap.find(std::pair<int, int>(tmp->cur.x, tmp->cur.y)) != tmpMap.end()){
        collide = true;
        break;
      }
      else{
        tmpMap.insert(std::pair<std::pair<int, int>, std::string>(std::pair<int, int>(tmp->cur.x, tmp->cur.y), map_it->first));
        tmp = tmp->prev;
      }
    }
    if (!collide){
      std::map<std::string, DotPairT*>::iterator tmpIt = map_it;
      ++tmpIt;
      if (tmpIt == DotsMap.end())
        solutions.push_back(tmpMap);
      else{
        tmpSol = analyzePathsHelper(tmpIt, tmpMap);
        solutions.insert(solutions.end(), tmpSol.begin(), tmpSol.end());
      }
    }
  
  }
  
  return solutions;
}

std::vector<MapInfo> Map::analyzePaths(){
  MapInfo newMap;
  return analyzePathsHelper(DotsMap.begin(), newMap);
}

void Map::printPath(std::string color){
  for (std::vector<PathNodeT*>::iterator it = possiblePaths[color]->begin();
       it != possiblePaths[color]->end();
       ++it){
    std::cout<<std::endl;
    MapInfo solution = (*it)->pathMap;
    for (int i = 0 ; i < size; ++i){
      for (int j = 0; j < size; ++j){
        std::cout <<"[";
        std::cout.width(10);
        std::cout<<std::left<< solution[std::pair<int, int>(j, i)]<<"]";
      }
      std::cout << std::endl;
    }
    std::cout<<std::endl;
  }
}