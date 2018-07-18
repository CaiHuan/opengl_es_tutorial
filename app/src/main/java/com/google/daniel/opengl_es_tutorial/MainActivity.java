package com.google.daniel.opengl_es_tutorial;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity implements GLSurfaceView.Renderer {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native_main");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        nativeInit();
        setContentView(R.layout.activity_main);
        mGLSurfaceView = findViewById(R.id.gl_surface_view);
        mGLSurfaceView.setEGLContextClientVersion(3);
        mGLSurfaceView.setRenderer(this);
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        Bitmap bitmap = null;//BitmapFactory.decodeResource(getResources(), R.mipmap);
        nativeOnSurfaceCreated(bitmap);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {
        nativeOnSurfaceChanged(i, i1);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        nativeOnDrawFrame();
    }

    public native boolean nativeInit();
    public native void nativeOnDrawFrame();
    public native void nativeOnSurfaceCreated(Bitmap bitmap);
    public native void nativeOnSurfaceChanged(int width, int height);

    private GLSurfaceView mGLSurfaceView;
}
