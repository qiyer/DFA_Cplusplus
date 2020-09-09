//
//  DFilter.cpp
//  adtest
//
//  Created by Zhi Zhuang on 2020/9/4.
//  Copyright © 2020 Zhi Zhuang. All rights reserved.
//

#include "DFilter.hpp"
#include <iostream>
#include <codecvt>
#include <regex>
#include <fstream>
#include <cassert>
using namespace std;

wstring to_wchar_t( string str )
{ return wstring_convert< codecvt_utf8<wchar_t>, wchar_t >{}.from_bytes(str); }

std::string to_utf8( std::wstring wstr )
{ return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(wstr); }

wregex const pattern(L"[^\uff10-\uff19\\u0030-\\u0039\uff41-\uff5a\\u0061-\\u007a\uff21-\uff3a\\u0041-\\u005a\u4e00-\u9fa5\u0f00-\u0fff\\u002b\n]");
//判断是否是除去 中文、英文、数字、藏文以外的特殊字符
vector<int> getValidCharacters(wstring & wstr){
    wsmatch match;
    vector<int> results;
    wstring::const_iterator iterStart = wstr.begin();
    wstring::const_iterator iterEnd = wstr.end();
    wstring temp;
    int location = -1;
    while (regex_search(iterStart, iterEnd, match, pattern))
    {
        //更新搜索起始位置,搜索剩下的字符串
        temp = match[0];
        location = location + (int)match.position() + 1;
        results.insert(results.begin(),location);
        iterStart = match[0].second;    //更新搜索起始位置,搜索剩下的字符串
    }
    return results;
}

DDictionary * filters = nullptr;

/**
*  load 为加载字库方法
*
*  @param fileURL 字库地址
*
*/
void DFilter::load(const char* fileURL)
{
    ifstream infile;
    infile.open(fileURL);   //将文件流对象与文件连接起来
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行
    filters = new DDictionary();

    string str;
    while(getline(infile,str))
    {
        DWord word;
        word.word = to_wchar_t(str);;
        wstring wch = word.word.substr(0,1);
        word.length = (int) word.word.size();
        
        void (^insertBlock)() = ^() {
            vector<DWord> dVector;
            dVector.push_back(word);
            filters->insert(make_pair(wch, dVector));
        };
        
        if( filters->size() == 0 ){
            insertBlock();
        }else{
            auto vector = filters->find(wch);
            if (vector == filters->end()) {
                insertBlock();
            }else{
                (*vector).second.insert((*vector).second.begin(), word);
            }
        }
    }
    infile.close();
}

/**
*  find 为查询字符串是否有敏感词，并且返回用***处理后的结果
*
*  @param t 要查询的字符串
*
*  @return 返回用***处理后的字符串
*/
string DFilter::find(string& t){
    //lowerWS:转为小写字符串   originWS：原始字符串
    wstring  lowerWS,originWS = to_wchar_t(t);
    lowerWS = originWS;
    //转为小写字符串
    transform(lowerWS.begin(), lowerWS.end(), lowerWS.begin(), towlower);
    vector<int> specials = getValidCharacters(lowerWS);
    int size = (int)originWS.size();
    for(int i = 0; i < size; i++)
    {
        wstring word = lowerWS.substr(i,1);
        auto result = filters->find(word);
        if (result != filters->end())
        {
            vector<DWord> dVector = result->second;
            for(int j = 0; j < dVector.size(); j++)
            {
                DWord dWord = dVector[j];
                int num = dWord.length;
                if((i+num) <= size)
                {
                    wstring fragment = L"";
                    int hitCount = 0;
                    if (specials.size()==0)
                    {
                        fragment = lowerWS.substr(i,num) ;
                    }else{
                        //有特殊字符
                        int specialCount = (int)specials.size() - 1;
                        int lastIndex = specialCount;
                        for (int n = specialCount; n >= 0; n--)
                        {
                            int beginIndex = i + num;
                            int specialIndex = specials[n];
                            if (i > specialIndex) {
                                lastIndex = specialIndex;
                                specialCount--;
                                specials.pop_back();
                                continue;
                            }
                            if (beginIndex > (specialIndex-hitCount) )
                            {
                                hitCount++;
                                if (n == specialCount) {
                                    fragment = fragment.append(lowerWS.substr(i,specialIndex-i));
                                }else {
                                    fragment = fragment.append(lowerWS.substr(lastIndex+1, specialIndex - lastIndex-1));
                                }
                                lastIndex = specialIndex;
                                //解决最后一个特殊字符的问题
                                if(n==0){
                                    fragment = fragment.append(lowerWS.substr(lastIndex+1, i+num - lastIndex+hitCount-1));
                                }
                            }else{
                                if (hitCount>0) {
                                    fragment = fragment.append(lowerWS.substr(lastIndex+1, i+num - lastIndex+hitCount-1));
                                }else{
                                    fragment = lowerWS.substr(i,num);
                                }
                                break;
                            }
                        }
                    }
                    if (fragment == dWord.word) {
                        std::wstring stars(num+hitCount,'*');
                        originWS.replace(i, num+hitCount, stars);
                        i = i+num-1;
                        hitCount = 0;
                        break;
                    }
                }
            }
        }
    }
    return to_utf8(originWS);
}
