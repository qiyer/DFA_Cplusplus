//
//  DFilter.hpp
//  SheildDemoCPlusPlus
//
//  Created by Zhi Zhuang on 2019/4/13.
//  Copyright © 2019 Zhi Zhuang. All rights reserved.
//

#include <unordered_map>
#include <ostream>
struct DDicChild
{
    void* child;
    bool  isLast;
};

typedef std::unordered_map<wchar_t,DDicChild > DDictionary;

class DFilter
{
private:
    DDictionary * filters = nullptr;
    DDictionary* pool = nullptr;
    DDictionary* getNewDictionary();
    int numNew = 0;
    
    void doLoad(std::wstring & char_wstr);
    
public:
    bool isInit = false;

    void load(const std::vector<std::string>& words);
    std::string find(std::string& t);
//    std::vector<std::wstring> load(std::string & words);
//    void load(const std::vector<std::wstring>& words);
};

