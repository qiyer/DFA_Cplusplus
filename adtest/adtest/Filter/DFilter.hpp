//
//  DFilter.hpp
//  adtest
//
//  Created by Zhi Zhuang on 2020/9/4.
//  Copyright © 2020 Zhi Zhuang. All rights reserved.
//
#include <ostream>
#include <vector>
#include <unordered_map>
struct DWord
{
    std::wstring word;
    int  length;
};

typedef std::unordered_map< std::wstring, std::vector<DWord> > DDictionary;

class DFilter
{
    public:
        void load(const char* fileURL);
        std::string find(std::string& t);
};
