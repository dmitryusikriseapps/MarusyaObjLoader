package com.riseapps.marusyaobjloader.model;

import com.riseapps.marusyaobjloader.model.mesh.MeshModel;

public class ShapeModel {
    private String name;
    private MeshModel meshModel;

    public ShapeModel() {
    }

    public String getName() {
        return name;
    }

    public MeshModel getMeshModel() {
        return meshModel;
    }


    @Override
    public String toString() {
        return "ShapeModel{" +
                "name='" + name + '\'' +
                ", meshModel=" + meshModel.toString() +
                '}';
    }
}