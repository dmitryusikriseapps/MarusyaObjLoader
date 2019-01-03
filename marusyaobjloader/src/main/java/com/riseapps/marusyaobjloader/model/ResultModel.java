package com.riseapps.marusyaobjloader.model;

import com.riseapps.marusyaobjloader.model.material.MaterialModel;
import com.riseapps.marusyaobjloader.model.mesh.ShapeModel;

import java.util.Arrays;

public class ResultModel {
    private ShapeModel[] shapeModels;
    private MaterialModel[] materialModels;
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
                ", warn='" + warn + '\'' +
                ", error='" + error + '\'' +
                '}';
    }
}