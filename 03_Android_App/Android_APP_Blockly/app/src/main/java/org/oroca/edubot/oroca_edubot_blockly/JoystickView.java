package org.oroca.edubot.oroca_edubot_blockly;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import static java.lang.Math.max;
import static java.lang.Math.min;

public class JoystickView extends View {
    private Paint drawPaint;
    int pointX = 0;
    int pointY = 0;

    public JoystickView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setupPaint();
    }

    private void setupPaint() {
        drawPaint = new Paint();
        drawPaint.setAntiAlias(true);
        drawPaint.setStrokeWidth(5);
        drawPaint.setStrokeJoin(Paint.Join.ROUND);
        drawPaint.setStrokeCap(Paint.Cap.ROUND);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        drawPaint.setStrokeWidth(2);
        drawPaint.setColor(Color.rgb(200, 200, 200));
        canvas.drawLine(this.getWidth()/2, 0, this.getWidth()/2, this.getHeight(), drawPaint);
        canvas.drawLine(0, this.getHeight()/2, this.getWidth(), this.getHeight()/2, drawPaint);

        int x = this.getWidth()/2 + pointX;
        int y = this.getHeight()/2 - pointY;

        drawPaint.setStrokeWidth(5);
        drawPaint.setStyle(Paint.Style.STROKE);
        drawPaint.setColor(Color.rgb(50, 200, 50));
        canvas.drawCircle(x, y, 40, drawPaint);

        drawPaint.setStyle(Paint.Style.FILL);
        drawPaint.setColor(Color.rgb(0, 150, 50));
        canvas.drawCircle(x, y, 36, drawPaint);
    }

    public void setCenterPoint(int x, int y) {
        pointX = max(min(this.getWidth()/2 - 42, x), -1 * this.getWidth()/2 + 42);
        pointY = max(min(this.getHeight()/2 - 42, y), -1 * this.getHeight()/2 + 42);
        postInvalidate();
    }
}
