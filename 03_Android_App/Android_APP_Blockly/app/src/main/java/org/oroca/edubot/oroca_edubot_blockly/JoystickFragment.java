package org.oroca.edubot.oroca_edubot_blockly;


import android.os.Bundle;
import android.provider.ContactsContract;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.DragEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ImageView;

import java.util.Timer;
import java.util.TimerTask;

import static java.lang.Math.min;
import static java.lang.StrictMath.max;


/**
 * A simple {@link Fragment} subclass.
 */
public class JoystickFragment extends Fragment {
    JoystickView mJoystickView;
    Timer mTimer;
    double currentX = 0;
    double currentY = 0;
    double viewWidth = 0;
    double viewHeight = 0;
    Boolean isTouched = false;

    ImageButton mButtonX;
    ImageButton mButtonY;
    ImageButton mButtonA;
    ImageButton mButtonB;

    public JoystickFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View v =  inflater.inflate(R.layout.fragment_joystick, container, false);

        mJoystickView = (JoystickView) v.findViewById(R.id.viewJoystick);
        mJoystickView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                double width = mJoystickView.getWidth();
                double height = mJoystickView.getHeight();
                viewWidth = width;
                viewHeight = height;
                switch (event.getAction() & MotionEvent.ACTION_MASK) {
                    case MotionEvent.ACTION_DOWN:
                        isTouched = true;

                    case MotionEvent.ACTION_MOVE:
                        currentX = (event.getX() - width / 2.0) / (width / 2.0);
                        currentY = (height / 2.0 - event.getY()) / (height / 2.0);

                        currentX = min(max(-1.0, currentX), 1.0);
                        currentY = min(max(-1.0, currentY), 1.0);
                        break;

                    case MotionEvent.ACTION_UP:
                        isTouched = false;
                }
                return true;
            }
        });

        TimerTask timerTask = new TimerTask() {
            @Override
            public void run() {
                if(isTouched) {
                    double x = currentX * 300.0;
                    double y = currentY * 300.0;

                    int v1 = (int)(y + x);
                    int v2 = (int)(y - x);

                    int vv1 = max(min(v1, 300), -300);
                    int vv2 = max(min(v2, 300), -300);

                    ((MainActivity)getActivity()).motorSetVelocity(vv1, vv2);
                    getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mJoystickView.setCenterPoint((int)(currentX * viewWidth / 2.0), (int)(currentY * viewHeight / 2.0));
                        }
                    });
                }
                else {
                    ((MainActivity)getActivity()).motorSetVelocity(0, 0);
                    getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            mJoystickView.setCenterPoint(0, 0);
                        }
                    });
                }
            }
        };

        mButtonX = v.findViewById(R.id.buttonJoystickX);
        mButtonY = v.findViewById(R.id.buttonJoystickY);
        mButtonA = v.findViewById(R.id.buttonJoystickA);
        mButtonB = v.findViewById(R.id.buttonJoystickB);

        mButtonX.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity)getActivity()).setColorLED("#FF0000", "#FF0000");
            }
        });

        mButtonY.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity)getActivity()).setColorLED("#00FF00", "#00FF00");
            }
        });

        mButtonA.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity)getActivity()).setText("A버튼");
            }
        });

        mButtonB.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity)getActivity()).setImage(0);
            }
        });

        mTimer = new Timer();
        mTimer.schedule(timerTask, 0, 50);
        return v;
    }

    public void stopTimer() {
        if(mTimer != null) {
            mTimer.cancel();
        }
    }
}
