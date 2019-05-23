package com.denachina.shieldsdk.utils;


public class FilterCPlusUtils {

    public static native void addSensitiveWord(String[] sensitiveWordSet);
    public static native String replaceSensitiveWord(String word);

    static {
        System.loadLibrary("lcm-filter");
    }
}
