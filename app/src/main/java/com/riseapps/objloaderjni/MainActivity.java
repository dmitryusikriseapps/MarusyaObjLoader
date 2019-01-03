package com.riseapps.objloaderjni;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;

import com.riseapps.marusyaobjloader.MarusyaObjLoader;
import com.riseapps.marusyaobjloader.MarusyaObjLoaderImpl;
import com.riseapps.marusyaobjloader.model.ResultModel;

import java.io.File;
import java.io.FileNotFoundException;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final MarusyaObjLoader marusyaObjLoader = new MarusyaObjLoaderImpl();
        try {
//            marusyaObjLoader.disableLog();
            final ResultModel centerTable3dMeshModel = marusyaObjLoader.load(new File(Environment.getExternalStorageDirectory(), "jni_test/center_table.obj"), true, true);
//            System.out.println(centerTable3dMeshModel.toString());
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}