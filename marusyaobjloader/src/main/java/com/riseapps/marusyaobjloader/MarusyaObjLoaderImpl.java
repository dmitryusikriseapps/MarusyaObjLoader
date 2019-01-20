package com.riseapps.marusyaobjloader;

import com.riseapps.marusyaobjloader.model.ResultModel;

import java.io.File;
import java.io.FileNotFoundException;

public class MarusyaObjLoaderImpl implements MarusyaObjLoader {
    static {
        System.loadLibrary("obj_loader_jni");
    }

    @Override
    public ResultModel load(File obj, boolean flipTextureCoordinates) throws FileNotFoundException {
        if (obj == null) {
            throw new FileNotFoundException("The obj file is null");
        } else if (!obj.exists()) {
            throw new FileNotFoundException(String.format("%s not found", obj.getAbsolutePath()));
        }

        return nativeLoad(obj.getAbsolutePath(), "", 1.0f, flipTextureCoordinates);
    }

    @Override
    public ResultModel load(File obj, File mtl, boolean flipTextureCoordinates) throws FileNotFoundException {
        if (obj == null) {
            throw new FileNotFoundException("The obj file is null");
        } else if (mtl == null) {
            throw new FileNotFoundException("The mtl file is null");
        } else if (!obj.exists()) {
            throw new FileNotFoundException(String.format("%s not found", obj.getAbsolutePath()));
        } else if (!mtl.exists()) {
            throw new FileNotFoundException(String.format("%s not found", mtl.getAbsolutePath()));
        }

        return nativeLoad(obj.getAbsolutePath(), mtl.getAbsolutePath(), 1.0f, flipTextureCoordinates);
    }

    @Override
    public ResultModel load(File obj, float normalizeCoefficient, boolean flipTextureCoordinates) throws FileNotFoundException {
        if (obj == null) {
            throw new FileNotFoundException("The obj file is null");
        }  else if (!obj.exists()) {
            throw new FileNotFoundException(String.format("%s not found", obj.getAbsolutePath()));
        }

        return nativeLoad(obj.getAbsolutePath(), "", normalizeCoefficient, flipTextureCoordinates);
    }

    @Override
    public ResultModel load(File obj, File mtl, float normalizeCoefficient, boolean flipTextureCoordinates) throws FileNotFoundException {
        if (obj == null) {
            throw new FileNotFoundException("The obj file is null");
        } else if (mtl == null) {
            throw new FileNotFoundException("The mtl file is null");
        } else if (!obj.exists()) {
            throw new FileNotFoundException(String.format("%s not found", obj.getAbsolutePath()));
        } else if (!mtl.exists()) {
            throw new FileNotFoundException(String.format("%s not found", mtl.getAbsolutePath()));
        }

        return nativeLoad(obj.getAbsolutePath(), mtl.getAbsolutePath(), normalizeCoefficient, flipTextureCoordinates);
    }

    @Override
    public void enableLog() {
        nativeEnableLog();
    }

    @Override
    public void disableLog() {
        nativeDisableLog();
    }

    private native ResultModel nativeLoad(String objPath,
                                          String mtlPath,
                                          float normalizeCoefficient,
                                          boolean flipTextureCoordinates);

    private native void nativeEnableLog();

    private native void nativeDisableLog();
}