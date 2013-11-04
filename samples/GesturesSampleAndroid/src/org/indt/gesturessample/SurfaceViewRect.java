package org.indt.gesturessample;

import java.util.Random;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PointF;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

public class SurfaceViewRect extends SurfaceView implements Callback {
    private Paint paint;

    private double scale = 0.25f;
    private int width = 100;
    private int height = 100;
    private PointF center;

    private SurfaceHolder holder;
    private TestThread renderThread;

    public SurfaceViewRect(Context context, AttributeSet attrs) {
        super(context, attrs);

        holder = getHolder();
        holder.addCallback(this);

        paint = new Paint();
        paint.setColor(Color.rgb(30, 70, 0));
        renderThread = new TestThread();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width,
            int height) {
        renderThread.start();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        center = new PointF(getWidth() / 2f, getHeight() / 2f);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }

    public void startPinch() {
    }

    public void updatePinch(double scale, int centerX, int centerY) {
        this.scale = scale;
        center.x = centerX;
        center.y = centerY;
    }

    public void stopPinch() {
        width = (int) (width * scale);
        height = (int) (height * scale);
    }

    public void tap(int x, int y, int verticalRadius, int horizontalRadius) {
        Random generator = new Random();
        center.x = x;
        center.y = y;
        paint.setColor(Color.rgb(30, generator.nextInt(255), 0));
    }
    
    public void pan(int x, int y, int deltaX, int deltaY) {
        center.x = x;
        center.y = y;
    }

    class TestThread extends Thread {
        @Override
        public void run() {
            while (!isInterrupted()) {
                Canvas canvas = null;
                try {
                    canvas = holder.lockCanvas();
                    synchronized (holder) {
                        canvas.drawRGB(255, 255, 55);
                        float left = (float) (center.x - width * scale * 0.5);
                        float top = (float) (center.y - height * scale * 0.5);
                        float right = (float) (center.x + width * scale * 0.5);
                        float bottom = (float) (center.y + height * scale * 0.5);
                        canvas.drawRect(left, top, right, bottom, paint);
                    }
                } catch (NullPointerException e) {
                    interrupt();
                } finally {
                    if (canvas != null)
                        holder.unlockCanvasAndPost(canvas);
                }

                try {
                    sleep(5);
                } catch (InterruptedException e) {
                    interrupt();
                }
            }
        }
    }
}
