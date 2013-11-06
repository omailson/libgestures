package org.indt.gesturessample;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends Activity {

    private SurfaceViewRect surfaceView;
    public static native void nativeOnTouch(MotionEvent event);
    public static native void nativeOnStart(MainActivity activity);
    public static native void nativeOnStop();
    public static native void nativeUpdateTimestamp(long timestamp);

    static {
        System.loadLibrary("gesturessample");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        setGestureType("No gesture");
        
        surfaceView = (SurfaceViewRect)findViewById(R.id.surfaceView);
        surfaceView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                nativeOnTouch(event);
                return true;
            }
        });

        // Initialize timer
        Timer timer = new Timer();
        // Execute a method every 60ms
        // This will be performed in another thread...
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                // ...however only the UI thread can change the views (and that's what we're doing after all)
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        // Call a native method to update the gesture manager timestamp
                        MainActivity.nativeUpdateTimestamp(System.currentTimeMillis());
                    }
                });
            }
        }, 0, 60);
    }

    public void setGestureType(String gestureType) {
        TextView textView = (TextView)findViewById(R.id.text);
        textView.setText(gestureType);
    }
    
    public void updatePinch(double scale, int centerX, int centerY) {
        surfaceView.updatePinch(scale, centerX, centerY);
    }
    
    public void startPinch() {
        surfaceView.startPinch();
    }
    
    public void stopPinch() {
        surfaceView.stopPinch();
    }
    
    public void tap(int x, int y, int verticalRadius, int horizontalRadius) {
        surfaceView.tap(x, y, verticalRadius, horizontalRadius);
    }
    
    public void pan(int x, int y, int deltaX, int deltaY) {
        surfaceView.pan(x, y, deltaX, deltaY);
    }

    @Override
    protected void onStart() {
        super.onStart();
        nativeOnStart(this);
    }

    @Override
    protected void onStop() {
        super.onStop();
        nativeOnStop();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
}
