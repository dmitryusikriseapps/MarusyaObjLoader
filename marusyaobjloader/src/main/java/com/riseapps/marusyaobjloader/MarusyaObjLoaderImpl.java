package com.riseapps.marusyaobjloader;

import com.riseapps.marusyaobjloader.model.ResultModel;

import java.io.File;
import java.io.FileNotFoundException;

public class MarusyaObjLoaderImpl implements MarusyaObjLoader {
    static {
        System.loadLibrary("obj_loader_jni");
    }

    @Override
    public ResultModel load(File objPath, File mtlPath, boolean flipTextureCoordinates) throws FileNotFoundException {
        if (objPath == null) {
            throw new FileNotFoundException("The obj file is null");
        } else if (mtlPath == null) {
            throw new FileNotFoundException("The mtl file is null");
        } else if (!objPath.exists()) {
            throw new FileNotFoundException(String.format("%s not found", objPath.getAbsolutePath()));
        } else if (!mtlPath.exists()) {
            throw new FileNotFoundException(String.format("%s not found", mtlPath.getAbsolutePath()));
        }

        return load(objPath.getAbsolutePath(), mtlPath.getAbsolutePath(), flipTextureCoordinates);
    }

    @Override
    public void enableLog() {
        enableLogJNI();
    }

    @Override
    public void disableLog() {
        disableLogJNI();
    }

    private native ResultModel load(String objPath, String mtlPath, boolean flipTextureCoordinates);

    private native void enableLogJNI();

    private native void disableLogJNI();
}