package com.riseapps.marusyaobjloader;

import com.riseapps.marusyaobjloader.model.ResultModel;

import java.io.File;
import java.io.FileNotFoundException;

public interface MarusyaObjLoader {
    ResultModel load(File objPath, File mtlPath, boolean flipTextureCoordinates) throws FileNotFoundException;

    void enableLog();

    void disableLog();
}