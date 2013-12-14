//
//  map.h
//  Flow
//
//  Created by Ziyun Kong on 12/11/13.
//  Copyright (c) 2013 Ziyun Kong. All rights reserved.
//

#ifndef __Flow__map__
#define __Flow__map__

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
//#include <omp.h>
#define EMPTY NULL

typedef std::map<std::pair<int, int>, std::string> MapInfo;

struct PointT {
  int x;
  int y;
  PointT(){}
  PointT(const PointT &p_):x(p_.x), y(p_.y){}
  PointT(int x_, int y_):x(x_), y(y_){}
};

struct DotPairT {
  std::string color;
  PointT loc1;
  PointT loc2;
  DotPairT(std::string color_, PointT p1_, PointT p2_)
  :color(color_), loc1(p1_), loc2(p2_){}
};

struct DotT{
  std::string color;
  PointT loc;
  DotT(){}
  DotT(std::string c_, PointT l_):color(c_), loc(l_){}
  DotT(DotT &p_):color(p_.color), loc(p_.loc){}
};

struct PathNodeT {
  PathNodeT* prev;
  PointT cur;
  std::map<std::pair<int, int>, std::string> pathMap;
  PathNodeT(){}
  PathNodeT(PathNodeT* prev_, PointT cur_, std::map<std::pair<int, int>, std::string> map_): prev(prev_), cur(cur_), pathMap(map_){}
};

struct mapInfoT {
  std::set<std::pair<int, int> > uniqueMap;
  std::vector<DotT> colorDotMap;
};

class Map{
  int size;
  std::vector<DotPairT*> Dots;
  std::map<std::string, DotPairT*> DotsMap;
  std::string **FlowMap;
//  std::set<std::pair<int, int> > currentMap;
//  std::set<std::pair<int, int> > prevMap;
//  mapInfoT initMap;
  //std::map<std::string, std::vector<MapInfo> > possiblePaths;
  std::map<std::string, std::vector<PathNodeT*>*> possiblePaths;
  std::map<std::pair<int, int>, std::string> initMapInfo;
  
  void generatePathsForPair(std::string);
  std::vector<MapInfo> analyzePathsHelper(std::map<std::string, DotPairT*>::iterator, MapInfo);
  std::vector<std::map<std::pair<int, int>, std::string> > analyzeDotPair(std::map<std::string, DotPairT*>::iterator, std::map<std::pair<int, int>, std::string>, bool &found);
  bool isCollide(PathNodeT*, PointT, std::map<std::pair<int, int>, std::string>);
public:
  Map(){}
  Map(int size_):size(size_){}
  //std::map<std::string, std::vector<PathNodeT*>*> getPaths(){return possiblePaths;}
  
  void printPath(std::string);
  
  void setDot(std::string color, PointT p1, PointT p2);
  void setMap();
  
  std::vector<std::map<std::pair<int, int>, std::string> > analyzeFlowMap();

  void generatePaths();

  std::vector<MapInfo> analyzePaths();
};
#endif /* defined(__Flow__map__) */
