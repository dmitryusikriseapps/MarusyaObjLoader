package com.riseapps.marusyaobjloader;

import com.riseapps.marusyaobjloader.model.GeometryDataModel;
import com.riseapps.marusyaobjloader.model.MeshModel;

import java.io.File;
import java.io.FileNotFoundException;

public class MarusyaObjLoaderImpl implements MarusyaObjLoader {
    static {
        System.loadLibrary("obj_loader_jni");
    }

    @Override
    public MeshModel loadMesh(File objPath, boolean normalizeVertices, boolean flipTextureCoordinates) throws FileNotFoundException {
        if (objPath == null) {
            throw new FileNotFoundException("The file is null");
        } else if (!objPath.exists()) {
            throw new FileNotFoundException(String.format("%s not found", objPath.getAbsolutePath()));
        }

        return loadMesh(objPath.getAbsolutePath(), normalizeVertices, flipTextureCoordinates);
    }

    @Override
    public GeometryDataModel loadGeometryData(File objPath, boolean normalizeVertices, boolean flipTextureCoordinates) throws FileNotFoundException {
        if (objPath == null) {
            throw new FileNotFoundException("The file is null.");
        } else if (!objPath.exists()) {
            throw new FileNotFoundException(String.format("%s not found", objPath.getAbsolutePath()));
        }

        return loadGeometryData(objPath.getAbsolutePath(), normalizeVertices, flipTextureCoordinates);
    }

    private native MeshModel loadMesh(String objPath, boolean normalizeVertices, boolean flipTextureCoordinates);

    private native GeometryDataModel loadGeometryData(String objPath, boolean normalizeVertices, boolean flipTextureCoordinates);
}