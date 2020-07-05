package com.chengjs.ndkdemo;

import javax.xml.transform.ErrorListener;

public class NEPlayer<dataSource> {
    static {
        System.loadLibrary("native-lib");
    }

    private String dataSource;

    public void setDataSource(String dataSource) {
        this.dataSource = dataSource;
    }

    public void prepare() {
        prepareNative(dataSource);
    }

    private native void prepareNative(String dataSource);

    private native void startNative();

    OnPreparedListener onPreparedListener;

    public void setOnPreparedListener(OnPreparedListener onPreparedListener) {
        this.onPreparedListener = onPreparedListener;
    }


    public void onPrepared() {
        if (onPreparedListener != null) {
            onPreparedListener.onPrepared();
        }
    }

    public void start() {

        startNative();
    }

    interface OnPreparedListener {
        void onPrepared();
    }

}
