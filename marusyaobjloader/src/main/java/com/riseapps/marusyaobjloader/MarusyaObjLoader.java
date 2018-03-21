package com.riseapps.marusyaobjloader;

import com.riseapps.marusyaobjloader.model.GeometryDataModel;
import com.riseapps.marusyaobjloader.model.MeshModel;

import java.io.File;
import java.io.FileNotFoundException;

public interface MarusyaObjLoader {
    MeshModel loadMesh(File objPath, boolean normalizeVertices, boolean flipTextureCoordinates) throws FileNotFoundException;

    GeometryDataModel loadGeometryData(File objPath, boolean normalizeVertices, boolean flipTextureCoordinates) throws FileNotFoundException;
}