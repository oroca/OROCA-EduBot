package org.oroca.edubot.oroca_edubot_blockly;

import android.content.Intent;
import android.net.Uri;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
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

    BlocklyWebViewFragment mBlocklyWebViewFragment;
    MainMenuFragment mMainMenuFragment;
    ImageButton mButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

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
            Uri selectFile = data.getData();
            mBlocklyWebViewFragment.reqGetXMLTextFromWorkspace(selectFile.toString());
        }
    }

    @JavascriptInterface
    void onResponseData(String req_name, String data1, String data2) {
        if(req_name.equals("getXmlTextFromWorkspace")) {
            Uri selectFile = Uri.parse(data1);

            try {
                OutputStream os = getContentResolver().openOutputStream(selectFile);
                PrintStream ps = new PrintStream(os);

                ps.print(data2);
                ps.flush();
                ps.close();
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
