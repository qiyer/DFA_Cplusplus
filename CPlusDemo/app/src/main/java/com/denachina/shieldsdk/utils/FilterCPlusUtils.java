package com.denachina.shieldsdk.utils;

import java.util.Set;

public class FilterCPlusUtils {

    public static native void addSensitiveWord(Set<String> sensitiveWordSet);
    public static native String replaceSensitiveWord(String word);

    static {
        System.loadLibrary("lcm-filter");
    }
}
