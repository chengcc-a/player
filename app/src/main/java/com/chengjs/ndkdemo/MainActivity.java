package com.chengjs.ndkdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceView;
import android.widget.Toast;

import java.io.File;

public class MainActivity extends AppCompatActivity {


    private SurfaceView surfaceView;
    private NEPlayer player;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        surfaceView = findViewById(R.id.surfaceView);
        player = new NEPlayer();
        player.setDataSource(new File(Environment.getExternalStorageDirectory() + File.separator + "input.mp4").getAbsolutePath());
        player.setOnPreparedListener(new NEPlayer.OnPreparedListener() {
            @Override
            public void onPrepared() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, "开始播放", Toast.LENGTH_SHORT).show();
                    }
                });
                player.start();
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    @Override
    protected void onResume() {
        super.onResume();
        player.prepare();
    }
}
