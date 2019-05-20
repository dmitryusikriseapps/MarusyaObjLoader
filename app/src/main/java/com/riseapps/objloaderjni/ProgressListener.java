package com.riseapps.objloaderjni;

import com.riseapps.marusyaobjloader.model.ResultModel;

public interface ProgressListener {
    void preLoad();

    void onFinish(ResultModel resultModel, long time);

    void onError(String error);
}
