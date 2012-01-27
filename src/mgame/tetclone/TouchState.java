package mgame.tetclone;

public class TouchState
{
    public enum Status
    {
        NON, DOWN, UP, MOVE
    }
    
    public TouchState()
    {
        pstate = Status.NON;
        state = Status.NON;
        x = 0.0f;
        y = 0.0f;
        px = 0.0f;
        py = 0.0f;
        downx = 0.0f;
        downy = 0.0f;
        tap = false;
    }
    public Status pstate;
    public Status state;
    public float x;
    public float y;
    public float px;
    public float py;
    public float downx;
    public float downy;
    public boolean tap;
}

