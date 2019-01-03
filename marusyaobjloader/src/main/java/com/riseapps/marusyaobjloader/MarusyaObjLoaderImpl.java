package com.riseapps.marusyaobjloader;

import com.riseapps.marusyaobjloader.model.ResultModel;

import java.io.File;
import java.io.FileNotFoundException;

public class MarusyaObjLoaderImpl implements MarusyaObjLoader {
    static {
        System.loadLibrary("obj_loader_jni");
    }

    @Override
    public ResultModel load(File objPath, boolean flipTextureCoordinates, boolean printLog) throws FileNotFoundException {
        if (objPath == null) {
            throw new FileNotFoundException("The file is null");
        } else if (!objPath.exists()) {
            throw new FileNotFoundException(String.format("%s not found", objPath.getAbsolutePath()));
        }

        return load(objPath.getAbsolutePath(), flipTextureCoordinates, printLog);
    }

    @Override
    public void enableLog() {
        enableLogJNI();
    }

    @Override
    public void disableLog() {
        disableLogJNI();
    }

    private native ResultModel load(String objPath, boolean flipTextureCoordinates, boolean printLog);

    private native void enableLogJNI();

    private native void disableLogJNI();
}