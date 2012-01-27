package mgame.tetclone;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.util.Log;
import android.opengl.GLSurfaceView.Renderer;


public class NdkGLRenderer implements Renderer {
    @Override
    public void onDrawFrame(GL10 gl) {
        Glue.onNdkDrawFrame();
    }
    
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Glue.onNdkSurfaceChanged(width, height);
    }
    
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Glue.onNdkSurfaceCreated();
    }
    
}

