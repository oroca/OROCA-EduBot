package org.oroca.edubot.oroca_edubot_blockly;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
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

public class MainActivity extends AppCompatActivity {
    private static final int MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE = 1011;
    private static final int MY_PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE = 1012;
    private static final int MY_PERMISSIONS_REQUEST_COARSE_LOCATION = 1013;

    BlocklyWebViewFragment mBlocklyWebViewFragment;
    MainMenuFragment mMainMenuFragment;
    ImageButton mButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

        appCheckPermission();

        mBlocklyWebViewFragment = new BlocklyWebViewFragment();
        mMainMenuFragment = new MainMenuFragment();

        mButton = (ImageButton) findViewById(R.id.buttonOpenMenu);
        mButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getSupportFragmentManager().beginTransaction()
                        .replace(R.id.frameLayout, mMainMenuFragment, "mainmenuFragment")
                        .setTransition(FragmentTransaction.TRANSIT_FRAGMENT_OPEN)
                        .addToBackStack(null)
                        .commit();
            }
        });

        getSupportFragmentManager().beginTransaction()
                .add(R.id.frameLayout, mBlocklyWebViewFragment, "blocklyFragment")
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
            Toast.makeText(this, "This device can't support BLE.", Toast.LENGTH_SHORT).show();
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
            Log.e("EEE", targetFileName.toString());
            mBlocklyWebViewFragment.reqGetXMLTextFromWorkspace();
        }
    }

    private Uri targetFileName = null;

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

            getSupportFragmentManager().beginTransaction()
                    .replace(R.id.frameLayout, mBlocklyWebViewFragment)
                    .commit();
        }
        else if(req_name.equals("restoreXmlTextToWorkspace")) {
            Toast.makeText(this, getResources().getString(R.string.load_project), Toast.LENGTH_SHORT).show();
            getSupportFragmentManager().beginTransaction()
                    .replace(R.id.frameLayout, mBlocklyWebViewFragment)
                    .commit();
        }
    }
}
