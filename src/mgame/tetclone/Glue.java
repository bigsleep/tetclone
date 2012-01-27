package mgame.tetclone;

public class Glue
{
    static {
         System.loadLibrary("tetclone");
       }
    static native public void onNdkSurfaceCreated();
    static native public void onNdkSurfaceChanged(int width, int height);
    static native public void onNdkDrawFrame();
    static native public void step();
    static native public void LoadFont(byte[] fontdata);
    static native public void onRotate();
    static native public void onLeft();
    static native public void onRight();
    static native public void onDown();
    static native public void Restart();
}

