package com.riseapps.objloaderjni;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.riseapps.marusyaobjloader.MarusyaObjLoader;
import com.riseapps.marusyaobjloader.MarusyaObjLoaderImpl;
import com.riseapps.marusyaobjloader.model.ResultModel;

import java.io.File;
import java.io.FileNotFoundException;

public class ResultActivity extends AppCompatActivity implements ProgressListener {
    private static final File FILES_PATH = new File(Environment.getExternalStorageDirectory(), "jni_test");
    private static final File OBJ = new File(FILES_PATH, "luxury_house_interior.obj");
    private static final File MTL = new File(FILES_PATH, "luxury_house_interior.mtl");

    private TextView result;
    private ProgressBar progress;

    public static Intent getStartIntent(@NonNull Context context) {
        return new Intent(context, ResultActivity.class);
    }

    public static void start(@NonNull Context context) {
        context.startActivity(getStartIntent(context));
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_result);
        result = findViewById(R.id.ar_result);
        progress = findViewById(R.id.ar_progress);
        load();
    }

    @Override
    public void preLoad() {
        progress.setVisibility(View.VISIBLE);
    }

    @Override
    public void onFinish(ResultModel resultModel, long time) {
        progress.setVisibility(View.GONE);
        result.setText(String.format(
                "Result:\n\nOBJ -> %1s\n (%2s MB)\n\nMTL -> %3s (%4s MB)\n\nTIME -> %5s\n\nSHAPES SIZE -> %6s\n\nMATERIALS SIZE -> %7s",
                OBJ.getAbsolutePath(),
                OBJ.length() / 1024.0f / 1024.0f,
                MTL.getAbsolutePath(),
                MTL.length() / 1024.0f / 1024.0f,
                time,
                resultModel.getShapeModels().length,
                resultModel.getMaterialModels().length));
    }

    @Override
    public void onError(String error) {
        result.setText(error);
    }

    private void load() {
        final Load3DModelAsyncTask load3DModelAsyncTask = new Load3DModelAsyncTask();
        load3DModelAsyncTask.setProgressListener(this);
        load3DModelAsyncTask.execute();
    }

    private static class Load3DModelAsyncTask extends AsyncTask<Void, Void, ResultModel> {
        private long time;
        private ProgressListener progressListener;

        public void setProgressListener(ProgressListener progressListener) {
            this.progressListener = progressListener;
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            if (progressListener != null) {
                progressListener.preLoad();
            }
        }

        @Override
        protected ResultModel doInBackground(Void... params) {
            final long startTime = System.currentTimeMillis();
            final MarusyaObjLoader marusyaObjLoader = new MarusyaObjLoaderImpl();
            ResultModel resultModel = null;
            try {
                resultModel = marusyaObjLoader.load(
                        OBJ,
                        MTL,
                        1.0f,
                        true);
                if (resultModel.getError() != null &&
                        !resultModel.getError().isEmpty()
                        && progressListener != null) {
                    progressListener.onError(resultModel.getError());
                }
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                if (progressListener != null) {
                    progressListener.onError(e.getMessage());
                }
            }
            time = System.currentTimeMillis() - startTime;
            return resultModel;
        }

        @Override
        protected void onPostExecute(ResultModel result) {
            super.onPostExecute(result);
            if (progressListener != null) {
                progressListener.onFinish(result, time);
            }
        }
    }
}
