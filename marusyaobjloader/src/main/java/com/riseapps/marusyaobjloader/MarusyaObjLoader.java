package com.riseapps.marusyaobjloader;

import com.riseapps.marusyaobjloader.model.ResultModel;

import java.io.File;
import java.io.FileNotFoundException;

public interface MarusyaObjLoader {
    ResultModel load(File obj, boolean flipTextureCoordinates) throws FileNotFoundException;

    ResultModel load(File obj, File mtl, boolean flipTextureCoordinates) throws FileNotFoundException;

    ResultModel load(File obj, float normalizeCoefficient, boolean flipTextureCoordinates) throws FileNotFoundException;

    ResultModel load(File obj, File mtl, float normalizeCoefficient, boolean flipTextureCoordinates) throws FileNotFoundException;

    void enableLog();

    void disableLog();
}