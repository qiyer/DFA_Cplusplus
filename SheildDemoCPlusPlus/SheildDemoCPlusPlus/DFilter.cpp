//
//  DFilter.cpp
//  SheildDemoCPlusPlus
//
//  Created by Zhi Zhuang on 2019/4/13.
//  Copyright © 2019 Zhi Zhuang. All rights reserved.
//

#include "DFilter.hpp"
#include <codecvt>
#include <regex>

//#include <iterator>
//#include <regex>
//#include <algorithm>
//#include <locale>
//#include <ctype.h>


//std::u16string to_utf16( std::string str )
//{ return std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.from_bytes(str); }
//
//std::string to_utf8( std::u16string str16 )
//{ return std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.to_bytes(str16); }
//
//std::u32string to_utf32( std::string str )
//{ return std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.from_bytes(str); }
//
//std::string to_utf8( std::u32string str32 )
//{ return std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.to_bytes(str32); }

std::wstring to_wchar_t( std::string str )
{ return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.from_bytes(str); }

std::string to_utf8( std::wstring wstr )
{ return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(wstr); }

std::wregex const pattern(L"[^\uff10-\uff19\\u0030-\\u0039\uff41-\uff5a\\u0061-\\u007a\uff21-\uff3a\\u0041-\\u005a\u4e00-\u9fa5\u0f00-\u0fff\\u002b\n]");
//判断是否是除去 中文、英文、数字、藏文以外的特殊字符
bool isValid_wstr(std::wstring & wstr){
    return std::regex_search(wstr, pattern);
}

std::string DFilter::find(std::string &t){
    //special_ws:特殊字符   lower_ws:转为小写字符串   origin_ws：原始字符串
    std::wstring  special_ws,lower_ws,origin_ws = to_wchar_t(t);
    lower_ws = origin_ws.append(L".");
    //转为小写字符串
    std::transform(lower_ws.begin(), lower_ws.end(), lower_ws.begin(), towlower);
    for (int i=0; i<lower_ws.size(); i++) {
        // 分词  ，比如：黑火药是谁 -> 火药是谁 -> 药是谁 -> 是谁 -> 谁
        std::wstring word = lower_ws.substr(i);
        //currentNum:当前num    beginSpecialNum：开始是特殊字符数量   realNum:真实命中数量  w_size：word的字符长度
        int currentNum = 0 ,beginSpecialNum = 0, realNum = 0, w_size = (int)word.size();
        bool isBeginSpace = true;
        DDictionary* node = filters;
        for (auto c : word) {
            //截取单个字符字符串，用于验证是否是特殊字符
            special_ws = word.substr(currentNum,1);
            auto first = node->find(c);
            //如果是特殊字符，就跳过不要命中
            if(currentNum != (w_size-1) && isValid_wstr(special_ws)){
                ++currentNum;
                if(isBeginSpace) ++beginSpecialNum;
            }else{
                if (first == node->end()) {
                    //判断是否 命中一个词，最大匹配，进行*字符替换,并且 跳出循环，进行下一个分词
                    if (currentNum > 0 && realNum > 0) {
                        std::wstring stars(realNum-beginSpecialNum,'*');
                        origin_ws = origin_ws.replace(i+beginSpecialNum, realNum-beginSpecialNum, stars);
                        i = i+realNum -1;
                    } break;
                }
                isBeginSpace = false;
                ++currentNum;
                realNum = (*first).second.isLast ? currentNum : realNum;
                node    = (DDictionary*)(*first).second.child;
            }
        }
    }
    origin_ws = origin_ws.substr(0,origin_ws.size()-1);
    return to_utf8(origin_ws);
}

void DFilter::load(const std::vector<std::string>& words)
{
    // 一次性开辟305000个连续DDictionary，为了提升性能以及垃圾回收
    pool = new DDictionary[305000];
    filters = getNewDictionary();
    //进行建树
    for (auto w : words) {
        std::wstring char_wstr = to_wchar_t(w);
        doLoad(char_wstr);
    }
    isInit = true;
}

void DFilter::doLoad(std::wstring & char_wstr){
    DDictionary * node = filters;
    int n = 0 , num = (int)char_wstr.size();
    DDicChild second;
    for (auto c : char_wstr) {
        auto first = node->find(c);
        second.isLast = (num==++n);
        if (first == node->end()) {
            second.child = getNewDictionary();
            node->insert(std::make_pair(c, second));
        }else{
            if (num==n) (*first).second.isLast = true;
            second.child = (DDictionary*)(*first).second.child;
        }
        node = reinterpret_cast<DDictionary *>(node->operator[](c).child);
    }
}
// 从队列里获取一个新DDictionary,鉴于demo里面词较多，大概是305000节点。如果换了其它词库，节点需要重新设定。
DDictionary* DFilter::getNewDictionary()
{
    return (++numNew < 305000) ? &pool[numNew-1]:new DDictionary();
};


//void DFilter::load(const std::vector<std::wstring>& words)
//{
//    filters = new DDictionary;
//    for (auto w : words) {
//        doLoad(w);
//    }
//    isInit = true;
//}

// std::wstring版本
//std::vector<std::wstring> ws_split(const std::wstring& in, const std::wstring& delim) {
//    std::wregex re{ delim };
//    return std::vector<std::wstring> {
//        std::wsregex_token_iterator(in.begin(), in.end(), re, -1),
//        std::wsregex_token_iterator()
//    };
//}
//
//std::vector<std::wstring> DFilter::load(std::string & words){
//    std::wstring  words_ws = to_wchar_t(words);
//    std::vector<std::wstring> words_list = ws_split(words_ws,L",");
//    return words_list;
//}


