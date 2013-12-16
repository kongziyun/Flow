//
//  main.cpp
//  Flow
//
//  Created by Ziyun Kong on 12/11/13.
//  Copyright (c) 2013 Ziyun Kong. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <omp.h>
#include "map.h"
double factor;
int numThreads;
pthread_t* thread;

//int main(int argc, const char * argv[])
{
  int size;

  std::cin >> size;
  Map flowMap(size);
  PointT a;
  PointT b;
/*
#pragma omp parallel
  {
    numThreads = omp_get_num_threads();
//    std::cout << omp_get_num_threads()<<std::endl;
  }
 */
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
  
  factor = std::atof(argv[1]);
  numThreads = std::atoi(argv[2]) - 1;
  thread = new pthread_t[numThreads];
  double time = omp_get_wtime();
  //flowMap.generatePaths();
  //flowMap.printPath("red");
  flowMap.analyzeFlowMap();
  //std::vector<std::map<std::pair<int, int>, std::string> > solution = flowMap.analyzePaths();
 time = omp_get_wtime() - time;
  std::cout<<std::endl;
  if (solution){
    std::cout <<"time is:"<<time<< std::endl;
    for (int i = 0 ; i < size; ++i){
      for (int j = 0; j < size; ++j){
        std::cout <<"[";
        std::cout.width(10);
        std::cout<<std::left<< (*solution)[std::pair<int, int>(j, i)]<<"]";
      }
      std::cout << std::endl;
    }
  }
    return 0;
  pthread_exit(NULL);
}

