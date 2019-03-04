package org.oroca.edubot.oroca_edubot_blockly;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.JavascriptInterface;
import android.widget.ImageButton;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.List;


public class MainActivity extends AppCompatActivity implements EdubotController.OnEdubotControllerListener {
    private static final int MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE = 1011;
    private static final int MY_PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE = 1012;
    private static final int MY_PERMISSIONS_REQUEST_COARSE_LOCATION = 1013;
    private static final int MY_REQUEST_ENABLE_BLUETOOTH = 1014;


    BlocklyWebViewFragment mBlocklyWebViewFragment;
    DeviceSelectFragment mDeviceSelectFragment;
    MainMenuFragment mMainMenuFragment;
    ViewCodeFragment mViewCodeFragment;
    ImageButton mButton;

    private Uri targetFileName = null;

    BluetoothManager mBluetoothManager;
    BluetoothAdapter mBluetoothAdapter;
    BluetoothLeScanner mBluetoothLeScanner;
    ScanSettings mScanSettings;
    boolean mIsScanning = false;
    boolean mIsConnectedBle = false;
    Handler mHandler = new Handler();
    static final long SCANNING_TIMEOUT = 10000; // 10sec

    private EdubotController mEdubotController;
    private boolean mIsAllowContinueExecuting = false;
    private boolean mModeInExecuting = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        appCheckPermission();

        mBlocklyWebViewFragment = new BlocklyWebViewFragment();
        mMainMenuFragment = new MainMenuFragment();
        mDeviceSelectFragment = new DeviceSelectFragment();
        mViewCodeFragment = new ViewCodeFragment(this);
        mEdubotController = new EdubotController(this);

        mEdubotController.setOnEdubotControllerListener(this);

        mButton = (ImageButton) findViewById(R.id.buttonOpenMenu);
        mButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!mModeInExecuting) {
                    getSupportFragmentManager().beginTransaction()
                            .replace(R.id.frameLayout, mMainMenuFragment, "mainMenuFragment")
                            .setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN)
                            .addToBackStack(null)
                            .commit();

                    mButton.setVisibility(View.GONE);
                }
                else {
                    setAllowContinueExecuting(false);
                    mModeInExecuting = false;
                    mButton.setImageResource(R.drawable.ic_icon_setup);
                }
            }
        });

        getSupportFragmentManager().beginTransaction()
                .replace(R.id.frameLayout, mBlocklyWebViewFragment)
                .commit();
    }

    public void appCheckPermission() {
        // Read & Write in External Storage
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
                    MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE);
        }
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    MY_PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE);
        }

        // Bluetooth
        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this, "This device can't support BLE.", Toast.LENGTH_LONG).show();
            finish();
        }
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.ACCESS_FINE_LOCATION},
                    MY_PERMISSIONS_REQUEST_COARSE_LOCATION);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        switch (requestCode) {
            case MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE: {
                if (grantResults.length > 0 && grantResults[0] != PackageManager.PERMISSION_GRANTED) {
                    Toast.makeText(this, "You can't load the projects in external storage.", Toast.LENGTH_LONG).show();
                }
            }
            case MY_PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE: {
                if (grantResults.length > 0 && grantResults[0] != PackageManager.PERMISSION_GRANTED) {
                    Toast.makeText(this, "You can't save your projects to external storage.", Toast.LENGTH_LONG).show();
                }
            }
            case MY_PERMISSIONS_REQUEST_COARSE_LOCATION: {
                if (grantResults.length > 0 && grantResults[0] != PackageManager.PERMISSION_GRANTED) {
                    Toast.makeText(this, "You need to allow permission for using BLE.", Toast.LENGTH_LONG).show();
                }
            }
        }
    }

    //    @Override
//    public void onWindowFocusChanged(boolean hasFocus) {
//        super.onWindowFocusChanged(hasFocus);
//        if (hasFocus) {
//            getWindow().getDecorView().setSystemUiVisibility(
//                    View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
//                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
//                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
//        }
//    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if(requestCode == MainMenuFragment.ON_REQUEST_OPEN_FILE && resultCode == RESULT_OK) {
            Uri selectFile = data.getData();
            try {
                InputStream is = getContentResolver().openInputStream(selectFile);
                InputStreamReader ir = new InputStreamReader(is);
                BufferedReader br = new BufferedReader(ir);
                mBlocklyWebViewFragment.reqRestoreXmlTextToWorkspace(br.readLine());
            }
            catch (FileNotFoundException e) {}
            catch (IOException e) {}
        }
        else if(requestCode == MainMenuFragment.ON_REQUEST_SAVE_FILE && resultCode == RESULT_OK) {
            targetFileName = data.getData();
            //Log.e("EEE", targetFileName.toString());
            mBlocklyWebViewFragment.reqGetXMLTextFromWorkspace();
        }
        else if(requestCode == MY_REQUEST_ENABLE_BLUETOOTH) {
            //Log.i("EEE", "Enable Bluetooth: " + Integer.toString(resultCode));
        }
    }

    @JavascriptInterface
    void onResponseData(String req_name, String data) {
        if(req_name.equals("getXmlTextFromWorkspace")) {
            try {
                OutputStream os = getContentResolver().openOutputStream(targetFileName, "w");
                PrintStream ps = new PrintStream(os);

                ps.print(data);
                ps.flush();
                ps.close();
                targetFileName = null;
            }
            catch (FileNotFoundException e) {}
            Toast.makeText(this, getResources().getString(R.string.save_project), Toast.LENGTH_SHORT).show();

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    getSupportFragmentManager().popBackStackImmediate();
                    mButton.setVisibility(View.VISIBLE);
                }
            });
        }
        else if(req_name.equals("restoreXmlTextToWorkspace")) {
            Toast.makeText(this, getResources().getString(R.string.load_project), Toast.LENGTH_SHORT).show();
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    getSupportFragmentManager().popBackStackImmediate();
                    mButton.setVisibility(View.VISIBLE);
                }
            });
        }
        else if(req_name.equals("getGeneratedCodeFromWorkspace")) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mViewCodeFragment.setResultCode(data);
                }
            });
        }
    }

    public boolean isConnectedBleDevice() { return mIsConnectedBle; }

    public void changeFragmentDeviceSelection() {
        getSupportFragmentManager().beginTransaction()
                .replace(R.id.frameLayout, mDeviceSelectFragment, "deviceSelection")
                .setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN)
                .addToBackStack(null)
                .commit();
    }

    public void startBleDeviceScanning() {
        if(mIsScanning) {
            this.stopBleDeviceScanning();
        }

        mBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        try {
            mBluetoothAdapter = mBluetoothManager.getAdapter();
        } catch(NullPointerException e) { Toast.makeText(this,"Error to get bluetooth adapter.", Toast.LENGTH_LONG).show(); }

        if(!mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, MY_REQUEST_ENABLE_BLUETOOTH);
        }

        mBluetoothLeScanner = mBluetoothAdapter.getBluetoothLeScanner();
        mScanSettings = new ScanSettings.Builder().setScanMode(ScanSettings.SCAN_MODE_LOW_POWER).build();

        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                mIsScanning = false;
                mBluetoothLeScanner.stopScan(scanCallback);
                mDeviceSelectFragment.onCompletedScanning();
            }
        }, SCANNING_TIMEOUT);

        mIsScanning = true;
        mBluetoothLeScanner.startScan(scanCallback);
    }

    private final ScanCallback scanCallback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            super.onScanResult(callbackType, result);

            if(result.getDevice().getName() != null) {
                mDeviceSelectFragment.onScanResult(result);
            }
        }

        @Override
        public void onBatchScanResults(List<ScanResult> results) {
            super.onBatchScanResults(results);
        }

        @Override
        public void onScanFailed(int errorCode) {
            super.onScanFailed(errorCode);
        }
    };

    @Override
    public void onBackPressed() {
        super.onBackPressed();

        if(getSupportFragmentManager().getBackStackEntryCount() == 0) {
            mButton.setVisibility(View.VISIBLE);
        }
    }

    public void reqGetGeneratedCodeFromBlockly() {
        getSupportFragmentManager().beginTransaction()
                .replace(R.id.frameLayout, mViewCodeFragment, "viewCode")
                .setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN)
                .addToBackStack(null)
                .commit();

        mBlocklyWebViewFragment.reqGetGeneratedCodeFromBlockly();
    }

    public void stopBleDeviceScanning() {
        mIsScanning = false;
        mBluetoothLeScanner.stopScan(scanCallback);
    }

    public void connectBleDevice(String name, String address) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                getSupportFragmentManager().popBackStackImmediate();
            }
        });

        stopBleDeviceScanning();
        mEdubotController.connectToDevice(address);
    }

    public void disconnectBleDevice() {
        if(mIsConnectedBle) {
            mEdubotController.disconnectFromDevice();
        }
    }

    @Override
    public void onConnected() {
        mIsConnectedBle = true;
        mMainMenuFragment.onConnected();
    }

    @Override
    public void onDisconnected() {
        mIsConnectedBle = false;
        mMainMenuFragment.onDisconnected();
    }

    public void runCode() {
        mBlocklyWebViewFragment.reqRunCode();
        mModeInExecuting = true;

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mButton.setImageResource(R.drawable.ic_icon_stop);
            }
        });
    }

    // Functions for interfacing with Javascript
    public void setAllowContinueExecuting(boolean allow) {
        mIsAllowContinueExecuting = allow;
    }

    @JavascriptInterface
    public boolean allowContinueExecuting() {
        return mIsAllowContinueExecuting;
    }

    @JavascriptInterface
    public void doneExecuting() {
        mEdubotController.motorSetVelocity(0, 0);
        mEdubotController.miscSetColorLed("#000000", "#000000");
        mEdubotController.miscSetText("");
    }

    @JavascriptInterface
    public boolean isMoving() {
        return mEdubotController.getDataIsMoving();
    }

    @JavascriptInterface
    public void motorSetStep(final int l_step, final int r_step) {
        mEdubotController.motorSetStep(l_step, r_step);
    }

    @JavascriptInterface
    public void motorSetVelocity(final int l_vel, final int r_vel) {
        mEdubotController.motorSetVelocity(l_vel, r_vel);
    }

    @JavascriptInterface
    public void motorSetDistance(final int l_dist, final int r_dist) {
        mEdubotController.motorSetDistance(l_dist, r_dist);
    }

    @JavascriptInterface
    public void motorSetAcceleration(final int l_acc, final int r_acc) {
        mEdubotController.motorSetAccel(l_acc, r_acc);
    }

    @JavascriptInterface
    public void motorSetMaxVelocity(final int max_vel) {
        mEdubotController.setMotorMaxVelocity(max_vel);
    }

    @JavascriptInterface
    public int getMotorMaxVelocity() {
        return mEdubotController.getMotorMaxVelocity();
    }

    @JavascriptInterface
    public void motorMoveForward() {
        mEdubotController.motorSetVelocity(mEdubotController.getMotorMaxVelocity(),mEdubotController.getMotorMaxVelocity());
    }

    @JavascriptInterface
    public void motorMoveBackWard() {
        mEdubotController.motorSetVelocity(-1 * mEdubotController.getMotorMaxVelocity(), -1 * mEdubotController.getMotorMaxVelocity());
    }

    @JavascriptInterface
    public void motorTurnLeft() {
        mEdubotController.motorSetVelocity(-1 * mEdubotController.getMotorMaxVelocity(), mEdubotController.getMotorMaxVelocity());
    }

    @JavascriptInterface
    public void motorTurnRight() {
        mEdubotController.motorSetVelocity(mEdubotController.getMotorMaxVelocity(), -1 * mEdubotController.getMotorMaxVelocity());
    }

    @JavascriptInterface
    public void motorStop() {
        mEdubotController.motorSetVelocity(0, 0);
    }

    @JavascriptInterface
    public void setColorLED(final String l_rgb, final String r_rgb) {
        Log.e("EEE", "setColorLED");
        mEdubotController.miscSetColorLed(l_rgb, r_rgb);
    }

    @JavascriptInterface
    public void turnOffLEDs() {
        mEdubotController.miscSetColorLed("#000000", "#000000");
    }

    @JavascriptInterface
    public void setText(final String text) {
        mEdubotController.miscSetText(text);
    }

    @JavascriptInterface
    public void setImage(final int index) {
        mEdubotController.miscSetImage(index);
    }

    @JavascriptInterface
    public void clearDisplay() {
        mEdubotController.miscSetText("");
    }

    @JavascriptInterface
    public void playSound(final int index) {
        mEdubotController.miscPlaySound(index);
    }

    @JavascriptInterface
    public double getBattVoltage() {
        return mEdubotController.getDataBattVoltage();
    }

    @JavascriptInterface
    public boolean isNeedCharging() {
        return mEdubotController.getDataIsNeedCharging();
    }

    @JavascriptInterface
    public int getButtonState() {
        return mEdubotController.getDataButtonState();
    }

    @JavascriptInterface
    public boolean isButtonPressed() {
        return mEdubotController.getDataIsButtonPressed();
    }

    @JavascriptInterface
    public int getFloorSensor(int index) {
        Log.e("EEE", Integer.toString(mEdubotController.getDataFloorSensor(index)));
        return mEdubotController.getDataFloorSensor(index);
    }

    @JavascriptInterface
    public int getDistanceSensor(int index) {
        return mEdubotController.getDataDistanceSensor(index);
    }

    @JavascriptInterface
    public double getImuSensor(int index) {
        return mEdubotController.getDataImuSensor(index);
    }

    @JavascriptInterface
    public double getAccSensor(int index) {
        return mEdubotController.getDataAccSensor(index);
    }

    @JavascriptInterface
    public double getGyroSensor(int index) {
        return mEdubotController.getDataGyroSensor(index);
    }
}
