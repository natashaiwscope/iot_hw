using System;

namespace MyStopWatch
{
public class StopWatch
{

    private DateTime startTime;
    private DateTime stopTime;
    private bool running = false;


    public void Start()
    {
        this.startTime = DateTime.Now;
        this.running = true;
    }


    public void Stop()
    {
        this.stopTime = DateTime.Now;
        this.running = false;
    }


    // elaspsed time in milliseconds
    public double GetElapsedTime()
    {
        TimeSpan interval;

        if (running)
            interval = DateTime.Now - startTime;
        else
            interval = stopTime - startTime;

        return interval.TotalMilliseconds;
    }


    // elaspsed time in seconds
    public double GetElapsedTimeSecs()
    {
        TimeSpan interval;

        if (running)
            interval = DateTime.Now - startTime;
        else
            interval = stopTime - startTime;

        return interval.TotalSeconds;
    }
}
}
