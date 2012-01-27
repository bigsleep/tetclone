package mgame.tetclone;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.os.Bundle;
import android.util.Log;
import android.opengl.GLSurfaceView;
import android.view.SurfaceHolder;

import java.lang.String;
import java.lang.Math;

public class GLView extends GLSurfaceView {
    int width;
    int height;
    TouchState mTouch;
    private RefreshHandler mRedrawHandler = new RefreshHandler();
    class RefreshHandler extends Handler {
        public RefreshHandler()
        {
            sendMessageDelayed(obtainMessage(0), 100);
        }
        
        @Override
        public void handleMessage(Message msg) {
            GLView.this.update();
            GLView.this.invalidate();
        }

        public void sleep(long delayMillis) {
            this.removeMessages(0);
            sendMessageDelayed(obtainMessage(0), delayMillis);
        }
    }
    
    public GLView(Context context, TouchState t) {
        super(context);
        this.setRenderer(new NdkGLRenderer());
        mTouch = t;
    }

    public GLView(Context context, AttributeSet attrs, TouchState t) {
        super(context, attrs);
        this.setRenderer(new NdkGLRenderer());
        mTouch = t;
    }
    
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h)
    {
        width = w;
        height = h;
        super.surfaceChanged(holder, format, w, h);
    } 
    
    private int count = 0;
    
    public void update() {
        int fps = 60;
        int sleep_duration = 1000 / fps;
        float grid = 72.0f;
        switch(mTouch.state){
            case MOVE:
                float vx = mTouch.x - mTouch.px;
                float vy = mTouch.y - mTouch.py;
                float a = (float)height / grid;
                int x = (int)(vx / a);
                int y = (int)(vy / a);
                int b = 8;
                if(x < 0){
                    if(- x >= 1 && - x <= b)
                        Glue.onLeft();
                    for(int i = 0; i < - x - b; ++i)
                        Glue.onLeft();
                }else{
                    if(x >= 1 && x <= b)
                        Glue.onRight();
                    for(int i = 0; i < x - b; ++i)
                        Glue.onRight();
                }
                if(y > 0.0){
                    for(int i = 0; i < y; ++i)
                        Glue.onDown();
                }else{
                    if(y <= - 10) Glue.Restart();
                }
                mTouch.px = mTouch.x;
                mTouch.py = mTouch.y;
                break;
            case UP:
                if(mTouch.tap) Glue.onRotate();
                mTouch.pstate = TouchState.Status.NON;
                mTouch.state = TouchState.Status.NON;
                mTouch.x = 0.0f;
                mTouch.y = 0.0f;
                mTouch.px = 0.0f;
                mTouch.py = 0.0f;
                mTouch.downx = 0.0f;
                mTouch.downy = 0.0f;
                mTouch.tap = false;
                break;
            default:
                break;
        }
        if(count == 6){
            Glue.step();
            count = 0;
        }
        ++count;
        mRedrawHandler.sleep(sleep_duration);
    }

}

