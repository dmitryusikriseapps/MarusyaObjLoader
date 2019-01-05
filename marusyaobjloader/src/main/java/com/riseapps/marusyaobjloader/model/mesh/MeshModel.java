package com.riseapps.marusyaobjloader.model.mesh;

public class MeshModel {
    private float[] vertices;
    private int[] indices;

    public MeshModel() {
    }

    public float[] getVertices() {
        return vertices;
    }

    public int[] getIndices() {
        return indices;
    }

    @Override
    public String toString() {
        return String.format("MeshModel -> {vertices.length -> %1$s, indices.length -> %2$s}}",
                vertices != null ? vertices.length : 0,
                indices != null ? indices.length : 0);
    }
}