package com.riseapps.marusyaobjloader.model.mesh;

public class MeshModel {
    private float[] vertices;
    private int[] indices;
    private int verticesSize;
    private int normalsSize;
    private int texCoordsSize;
    private int colorsSize;

    public MeshModel() {
    }

    public float[] getVertices() {
        return vertices;
    }

    public int[] getIndices() {
        return indices;
    }

    public int getVerticesSize() {
        return verticesSize;
    }

    public int getNormalsSize() {
        return normalsSize;
    }

    public int getTexCoordsSize() {
        return texCoordsSize;
    }

    public int getColorsSize() {
        return colorsSize;
    }

    @Override
    public String toString() {
        return String.format("MeshModel -> {vertices.length -> %1$s, indices.length -> %2$s, geometry data params -> {vertices size -> %3$s, normals size -> %4$s, texture coordinates size -> %5$s, colors size -> %6$s}}",
                vertices != null ? vertices.length : 0,
                indices != null ? indices.length : 0,
                verticesSize,
                normalsSize,
                texCoordsSize,
                colorsSize);
    }
}