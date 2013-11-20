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
    private float [] swipeArea;
    private OnSizeChangedListerner onSizeChangedListener;

    private SurfaceHolder holder;
    private TestThread renderThread;

    public interface OnSizeChangedListerner {
        public void onSizeChanged(int w, int h, int oldw, int oldh);
    }

    public SurfaceViewRect(Context context, AttributeSet attrs) {
        super(context, attrs);

        this.swipeArea = new float[4];

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

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);

        if (onSizeChangedListener != null)
            onSizeChangedListener.onSizeChanged(w, h, oldw, oldh);
    }

    public void setOnSizeChangedListener(OnSizeChangedListerner listener) {
        this.onSizeChangedListener = listener;
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
        scale = 1;
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

    public void setSwipeArea(float left, float top, float right, float bottom) {
        this.swipeArea[0] = left;
        this.swipeArea[1] = top;
        this.swipeArea[2] = right;
        this.swipeArea[3] = bottom;
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

                        int color = paint.getColor();
                        paint.setColor(Color.BLACK);
                        float [] lines = {
                            swipeArea[0], swipeArea[1], swipeArea[2], swipeArea[1],
                            swipeArea[2], swipeArea[1], swipeArea[2], swipeArea[3],
                            swipeArea[2], swipeArea[3], swipeArea[0], swipeArea[3],
                            swipeArea[0], swipeArea[3], swipeArea[0], swipeArea[1]
                        };
                        canvas.drawLines(lines, paint);
                        paint.setColor(color);
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
