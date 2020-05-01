package com.tymphany.abrikotyan;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.regex.Pattern;

import io.github.controlwear.virtual.joystick.android.JoystickView;

import static java.lang.Math.PI;

public class MainActivity extends AppCompatActivity {

    final String defaultServerIP = "10.10.10.4";
    final int defaultServerPort = 3490;
    final SocketClient socketClient = new SocketClient(MainActivity.this);


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        JoystickView joystickViewTurn = findViewById(R.id.joyStickLeft);

        joystickViewTurn.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                MotorState newMotorState = calcMotorState(angle, strength);
                socketClient.send(newMotorState.toBytes());
            }
        });

        Button connectButton = findViewById(R.id.connectButton);
        connectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                final EditText et = new EditText(MainActivity.this);
                et.setText(defaultServerIP + ":" + defaultServerPort);
                new AlertDialog.Builder(MainActivity.this).setTitle("Input IP address:port")
                        .setIcon(R.drawable.ic_launcher_foreground)
                        .setView(et)
                        .setPositiveButton("Connect", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                Pattern pen = Pattern.compile(":");
                                String[] subString = pen.split(et.getText());
                                socketClient.connect(subString[0], Integer.parseInt(subString[1]));
                            }
                        }).setNegativeButton("Cancel", null).show();
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
