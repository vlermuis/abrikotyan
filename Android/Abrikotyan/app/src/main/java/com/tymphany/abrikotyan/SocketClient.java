package com.tymphany.abrikotyan;

import android.app.Activity;
import android.content.Context;
import android.widget.Toast;

import com.koushikdutta.async.AsyncServer;
import com.koushikdutta.async.AsyncSocket;
import com.koushikdutta.async.Util;
import com.koushikdutta.async.callback.CompletedCallback;
import com.koushikdutta.async.callback.ConnectCallback;

public class SocketClient implements ConnectCallback {

    private AsyncSocket socket;
    private Context uiContext;

    public SocketClient(Context uiContext) {
        this.uiContext = uiContext;
    }

    public void connect(String ipAddr, int portNum) {
        AsyncServer.getDefault().connectSocket(ipAddr, portNum, this);
        ((Activity) uiContext).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(uiContext, "Socket: Connecting", Toast.LENGTH_SHORT);
            }
        });
    }

    public void send(byte[] data) {
        if (socket == null)
            return;
        if (socket.isOpen()) {
            Util.writeAll(socket, data, new CompletedCallback() {
                @Override
                public void onCompleted(Exception ex) {
                    if (ex != null) {
                        ((Activity) uiContext).runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(uiContext, "Socket: Send error", Toast.LENGTH_SHORT);
                            }
                        });
                    }
                }
            });
        }
    }

    @Override
    public void onConnectCompleted(Exception ex, AsyncSocket socket) {
        if (ex != null) {
            ((Activity) uiContext).runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(uiContext, "Socket: Connect error", Toast.LENGTH_SHORT);
                }
            });
        } else {
            ((Activity) uiContext).runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(uiContext, "Socket: Connected", Toast.LENGTH_SHORT);
                }
            });
            this.socket = socket;
        }
    }
}
