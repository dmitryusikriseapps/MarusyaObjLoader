package com.riseapps.marusyaobjloader.model;

import com.riseapps.marusyaobjloader.model.material.MaterialModel;
import com.riseapps.marusyaobjloader.model.mesh.ShapeModel;

import java.util.Arrays;

public class ResultModel {
    private ShapeModel[] shapeModels;
    private MaterialModel[] materialModels;
    private long verticesSize;
    private long normalsSize;
    private long texcoordsSize;
    private long colorsSize;
    private String warn;
    private String error;

    public ResultModel() {
    }

    public ShapeModel[] getShapeModels() {
        return shapeModels;
    }

    public MaterialModel[] getMaterialModels() {
        return materialModels;
    }

    public long getVerticesSize() {
        return verticesSize;
    }

    public long getNormalsSize() {
        return normalsSize;
    }

    public long getTexcoordsSize() {
        return texcoordsSize;
    }

    public long getColorsSize() {
        return colorsSize;
    }

    public String getWarn() {
        return warn;
    }

    public String getError() {
        return error;
    }

    @Override
    public String toString() {
        return "ResultModel{" +
                "shapeModels=" + Arrays.toString(shapeModels) +
                ", materialModels=" + Arrays.toString(materialModels) +
                ", verticesSize=" + verticesSize +
                ", normalsSize=" + normalsSize +
                ", texcoordsSize=" + texcoordsSize +
                ", colorsSize=" + colorsSize +
                ", warn='" + warn + '\'' +
                ", error='" + error + '\'' +
                '}';
    }
}