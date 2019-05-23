//
// Created by Zhi Zhuang on 2019/4/30.
//
#include "com_denachina_shieldsdk_utils_FilterCPlusUtils.h"
#include "DFilter.h"

DFilter dFilter;
/*
 * Class:     com_denachina_shieldsdk_utils_FilterCPlusUtils
 * Method:    addSensitiveWord
 * Signature: (Ljava/util/Set;)V
 */
JNIEXPORT void JNICALL Java_com_denachina_shieldsdk_utils_FilterCPlusUtils_addSensitiveWord
  (JNIEnv *env, jobject object, jobjectArray stringArray)
  {
       jsize size = env->GetArrayLength(stringArray);
       std::vector<std::string> wordVector;
       jstring obj;
       for(int i=0;i<size;i++)
       {
           obj = (jstring)env->GetObjectArrayElement(stringArray,i);
           std::string str = (std::string)env->GetStringUTFChars(obj,NULL);//得到字符串
           wordVector.push_back(str);
           env->DeleteLocalRef( obj);
       }
       dFilter.load(wordVector);
  };

/*
 * Class:     com_denachina_shieldsdk_utils_FilterCPlusUtils
 * Method:    replaceSensitiveWord
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_denachina_shieldsdk_utils_FilterCPlusUtils_replaceSensitiveWord
  (JNIEnv *env, jclass jc, jstring jstr){

      std::string str = (std::string)env->GetStringUTFChars(jstr,0);//得到字符串
      std::string result = dFilter.isInit? dFilter.find(str) :"DFilter isn't init!";
      env->DeleteLocalRef( jstr);
      return env->NewStringUTF(result.c_str());
  };