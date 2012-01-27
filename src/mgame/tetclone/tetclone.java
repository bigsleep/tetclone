package mgame.tetclone;

import android.app.Activity;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.content.Context;
import android.content.res.Resources;
import android.util.Log;
import java.io.InputStream;

public class tetclone extends Activity
{
    private GLView mGLView;
    public TouchState mTouch = new TouchState();
    
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        mGLView = new GLView(this, mTouch);
        setContentView(mGLView);
        
        Resources r = getResources();
        try{
            InputStream inp = r.openRawResource(R.raw.inconsolata);
            int size = inp.available();
            byte[] data = new byte[size];
            inp.read(data);
            Glue.LoadFont(data);
        }catch (Exception e) {
			throw new RuntimeException(e);
		}finally {
		}
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        mTouch.pstate = mTouch.state;
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                mTouch.state = TouchState.Status.DOWN;
                mTouch.px = event.getX();
                mTouch.py = event.getY();
                mTouch.downx = mTouch.px;
                mTouch.downy = mTouch.py;
                break;
            case MotionEvent.ACTION_UP:
                if(mTouch.state == TouchState.Status.DOWN){
                    float dx = event.getX() - mTouch.downx;
                    float dy = event.getY() - mTouch.downy;
                    float l = dx * dx + dy * dy;
                    if(l < 80.0) mTouch.tap = true;
                }
                mTouch.state = TouchState.Status.UP;
                break;
            case MotionEvent.ACTION_MOVE:
                mTouch.state = TouchState.Status.MOVE;
                break;
            case MotionEvent.ACTION_CANCEL:
                break;
            default:
                break;
        }
        mTouch.x = event.getX();
        mTouch.y = event.getY();
        return super.onTouchEvent(event);
    }
}

