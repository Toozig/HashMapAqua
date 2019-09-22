//#include <iostream>
//#include "HashMap.hpp"
//
//int main()
//{
//    HashMap<int,long> point3d = HashMap<int,long>();
//    point3d.insert(1,1);
//    point3d.insert(17,2);
//    std::cout<< point3d.containKey(1)<< std::endl;
//    point3d.insert(2,2);
//    point3d.insert(3,3);
//    auto end = point3d.end();
//    point3d.erase(1);
//    long sds = point3d[1]; //undeined
//    point3d[5] = 5;
//    point3d.clear();
//    auto begin = point3d.begin();
//    HashMap<int,long> ddd = point3d;
//    ddd == point3d;
//    ++begin;
//    ++begin;
//    ++begin;
//    ++begin;
//    bool g = end == begin;
////        arrayWrapper<int> point3d = arrayWrapper<int>(3);
////        point3d[0] = 1;
////        point3d[1] = 2;
////        point3d[2] = 3;
//
//    auto i = point3d.begin();
//    auto e = point3d.end();
//    for (; i != e; ++i)
//    {
//        std::cout<< i->first <<' ' << i->second << std::endl;
//    }
//
////        std::cout << std::endl;
//
////        std::vector<double> vec;
//////        std::copy(point3d.begin(), point3d.end(), std::back_inserter(vec));
////
////        for (double & i : vec)
////        {
////            std::cout << i << " ";
////        }
////
////        std::cout << std::endl;
//    return 0;
//}