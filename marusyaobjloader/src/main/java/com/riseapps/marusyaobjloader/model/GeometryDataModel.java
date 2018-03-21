package com.riseapps.marusyaobjloader.model;

public class GeometryDataModel {
    private float[] vertices;
    private float[] normals;
    private float[] textureCoordinates;
    private float[] colors;
    private int[] verticesIndices;
    private int[] normalsIndices;
    private int[] textureCoordinatesIndices;

    public GeometryDataModel() {
    }

    public float[] getVertices() {
        return vertices;
    }

    public float[] getNormals() {
        return normals;
    }

    public float[] getTextureCoordinates() {
        return textureCoordinates;
    }

    public float[] getColors() {
        return colors;
    }

    public int[] getVerticesIndices() {
        return verticesIndices;
    }

    public int[] getNormalsIndices() {
        return normalsIndices;
    }

    public int[] getTextureCoordinatesIndices() {
        return textureCoordinatesIndices;
    }

    @Override
    public String toString() {
        return String.format("GeometryDataModel -> {vertices.length -> %1s, normals.length -> %2s, textureCoordinates.length -> %3s, colors.length -> %4s, verticesIndices.length -> %5s, normalsIndices.length -> %6s, textureCoordinatesIndices.length -> %7s}",
                vertices != null ? vertices.length : 0,
                normals != null ? normals.length : 0,
                textureCoordinates != null ? textureCoordinates.length : 0,
                colors != null ? colors.length : 0,
                verticesIndices != null ? verticesIndices.length : 0,
                normalsIndices != null ? normalsIndices.length : 0,
                textureCoordinatesIndices != null ? textureCoordinatesIndices.length : 0);
    }
}