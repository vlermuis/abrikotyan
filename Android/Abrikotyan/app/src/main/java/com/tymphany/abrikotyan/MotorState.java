package com.tymphany.abrikotyan;

public class MotorState {
    private int leftPWM;
    private int rightPWM;

    public MotorState(int leftPWM, int rightPWM) {
        setLeftPWM(leftPWM);
        setRightPWM(rightPWM);
    }

    public int getLeftPWM() {
        return leftPWM;
    }

    public void setLeftPWM(int leftPWM) {
        this.leftPWM = leftPWM;
    }

    public int getRightPWM() {
        return rightPWM;
    }

    public void setRightPWM(int rightPWM) {
        this.rightPWM = rightPWM;
    }

    public byte[] toBytes() {
        return new byte[]{Integer.valueOf("AA", 16).byteValue(),
                Integer.valueOf("5", 16).byteValue(),
                Integer.valueOf("39", 16).byteValue(),
                (byte) leftPWM,
                (byte) rightPWM};
        // If you received a PWM more than 100, you should transform it into negative
    }

}
