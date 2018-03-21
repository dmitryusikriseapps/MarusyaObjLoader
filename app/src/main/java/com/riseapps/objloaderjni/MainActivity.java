package com.riseapps.objloaderjni;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;

import com.riseapps.marusyaobjloader.MarusyaObjLoader;
import com.riseapps.marusyaobjloader.MarusyaObjLoaderImpl;
import com.riseapps.marusyaobjloader.model.GeometryDataModel;
import com.riseapps.marusyaobjloader.model.MeshModel;

import java.io.File;
import java.io.FileNotFoundException;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final MarusyaObjLoader marusyaObjLoader = new MarusyaObjLoaderImpl();
        try {
            final MeshModel faceLandmarksMeshModel = marusyaObjLoader.loadMesh(new File(Environment.getExternalStorageDirectory(), "scan/faceLandmarks.obj"), true, true);
            final GeometryDataModel faceLandmarksGeometryDataModel = marusyaObjLoader.loadGeometryData(new File(Environment.getExternalStorageDirectory(), "scan/faceLandmarks.obj"), true, true);
            final MeshModel head3dMeshModel = marusyaObjLoader.loadMesh(new File(Environment.getExternalStorageDirectory(), "scan/head3d.obj"), true, true);
            final GeometryDataModel head3dGeometryDataModel = marusyaObjLoader.loadGeometryData(new File(Environment.getExternalStorageDirectory(), "scan/head3d.obj"), true, true);
            System.out.println(faceLandmarksMeshModel);
            System.out.println(faceLandmarksGeometryDataModel.toString());
            System.out.println(head3dMeshModel.toString());
            System.out.println(head3dGeometryDataModel.toString());
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}