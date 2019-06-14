package com.tymphany.abrikotyan;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import io.github.controlwear.virtual.joystick.android.JoystickView;

import static java.lang.Math.PI;

public class MainActivity extends AppCompatActivity {

    final String serverIP = "10.10.10.4";
    final int serverPort = 3490;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final SocketClient socketClient = new SocketClient(MainActivity.this);
        socketClient.connect(serverIP,serverPort);

        JoystickView joystickViewTurn = findViewById(R.id.joyStickLeft);

        joystickViewTurn.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                MotorState newMotorState = calcMotorState(angle, strength);
                socketClient.send(newMotorState.toBytes());
            }
        });

    }

    private MotorState calcMotorState(int angle, int strength) {
        int leftPWM = 0;
        int rightPWM = 0;

        if (angle >= 0 && angle <= 90) {
            leftPWM = strength;
            rightPWM = (int) (strength * Math.sin(2 * PI / 360 * angle));
        } else if (angle > 90 && angle <= 180) {
            leftPWM = (int) (strength * Math.sin(2 * PI / 360 * angle));
            rightPWM = strength;
        } else if (angle > 180 && angle <= 270) {
            leftPWM = (int) (strength * Math.sin(2 * PI / 360 * angle));
            rightPWM -= strength;
        } else if (angle > 270 && angle <= 360) {
            leftPWM -= strength;
            rightPWM = (int) (strength * Math.sin(2 * PI / 360 * angle));
        }

        return new MotorState(leftPWM, rightPWM);
    }
}
