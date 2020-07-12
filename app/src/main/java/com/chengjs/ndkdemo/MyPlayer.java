package com.chengjs.ndkdemo;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MyPlayer implements SurfaceHolder.Callback {
    static {
        System.loadLibrary("native-lib");
    }
    private SurfaceHolder surfaceHolder;

    public void setSurface(SurfaceView surface) {
        if (surfaceHolder!=null)
            surfaceHolder.removeCallback(this);
        surfaceHolder=surface.getHolder();
        surfaceHolder.addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        this.surfaceHolder = surfaceHolder;
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }

    public void start(String url) {
        native_start(url,surfaceHolder.getSurface());
    }

    public native void native_start(String url, Surface surface) ;
}
