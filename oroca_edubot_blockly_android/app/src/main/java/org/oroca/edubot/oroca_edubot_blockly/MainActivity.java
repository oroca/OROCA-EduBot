package org.oroca.edubot.oroca_edubot_blockly;

import android.media.Image;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;

public class MainActivity extends AppCompatActivity {

    BlocklyWebViewFragment mBlocklyWebViewFragment;
    MainMenuFragment mMainMenuFragment;
    ImageButton mButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

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


}
