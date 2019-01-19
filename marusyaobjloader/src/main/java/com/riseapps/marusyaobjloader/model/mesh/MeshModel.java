package com.riseapps.marusyaobjloader.model.mesh;

public class MeshModel {
    private float[] vertices;
    private int[] indices;
    private long verticesSize;
    private long normalsSize;
    private long texcoordsSize;

    public MeshModel() {
    }

    public float[] getVertices() {
        return vertices;
    }

    public int[] getIndices() {
        return indices;
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

    @Override
    public String toString() {
        return String.format("MeshModel -> {vertices.length -> %1$s, indices.length -> %2$s, verticesSize -> %3$s, normalsSize -> %4$s, texcoordsSize -> %5$s}}",
                vertices != null ? vertices.length : 0,
                indices != null ? indices.length : 0,
                verticesSize,
                normalsSize,
                texcoordsSize);
    }
}