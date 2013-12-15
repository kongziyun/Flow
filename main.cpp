//
//  main.cpp
//  Flow
//
//  Created by Ziyun Kong on 12/11/13.
//  Copyright (c) 2013 Ziyun Kong. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "map.h"

int main(int argc, const char * argv[])
{
  int size;

  std::cin >> size;
  Map flowMap(size);
  PointT a;
  PointT b;
  std::string color;
//  std::cout << "Please input color(\"quit\" if no more dot pairs): ";
  std::cin >> color;
  while (color != "quit"){
//    std::cout << "Dot1 x: ";
    std::cin >> a.x;
//    std::cout << "Dot1 y: ";
    std::cin >> a.y;
//    std::cout << "Dot2 x: ";
    std::cin >> b.x;
    //std::cout << "Dot2 y: ";
    std::cin >> b.y;
    flowMap.setDot(color, a, b);
    //std::cout << "Please input color(\"quit\" if no more dot pairs): ";
    std::cin >> color;
  }
  flowMap.setMap();
  double time = omp_get_wtime();
  
  flowMap.generatePaths();
  //flowMap.printPath("red");
  //std::vector<std::map<std::pair<int, int>, std::string> > solution = flowMap.analyzeFlowMap();
  std::vector<std::map<std::pair<int, int>, std::string> > solution = flowMap.analyzePaths();
  time = omp_get_wtime() - time;
  std::cout<<std::endl;
  
  std::cout << solution.size() <<" and time is:"<<time<< std::endl;
  if (solution.size())
  for (int i = 0 ; i < size; ++i){
    for (int j = 0; j < size; ++j){
      std::cout <<"[";
      std::cout.width(10);
      std::cout<<std::left<< solution[0][std::pair<int, int>(j, i)]<<"]";
    }
    std::cout << std::endl;
  }
  
    return 0;
}

