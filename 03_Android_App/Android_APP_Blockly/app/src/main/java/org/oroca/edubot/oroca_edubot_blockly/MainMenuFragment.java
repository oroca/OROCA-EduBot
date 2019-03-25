package org.oroca.edubot.oroca_edubot_blockly;


import android.app.Activity;
import android.content.Intent;
import android.media.Image;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;


/**
 * A simple {@link Fragment} subclass.
 */
public class MainMenuFragment extends Fragment {

    ImageButton buttonLoadProject;
    ImageButton buttonSaveProject;
    ImageButton buttonConnectDevice;
    ImageButton buttonExecute;
    ImageButton buttonViewCode;

    public static int ON_REQUEST_OPEN_FILE = 11;
    public static int ON_REQUEST_SAVE_FILE = 12;

    public MainMenuFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View v = inflater.inflate(R.layout.fragment_main_menu, container, false);

        buttonLoadProject = (ImageButton) v.findViewById(R.id.buttonLoadProject);
        buttonLoadProject.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent()
                        .setType("*/*")
                        .setAction(Intent.ACTION_OPEN_DOCUMENT);
                ((Activity)getContext()).startActivityForResult(
                        Intent.createChooser(intent, getResources().getString(R.string.select_project)),
                        ON_REQUEST_OPEN_FILE);
            }
        });

        buttonSaveProject = (ImageButton) v.findViewById(R.id.buttonSaveProject);
        buttonSaveProject.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent()
                        .addCategory(Intent.CATEGORY_OPENABLE)
                        .setType("*/*")
                        .setAction(Intent.ACTION_CREATE_DOCUMENT)
                        .putExtra(Intent.EXTRA_TITLE, "untitled.xml");
                ((Activity)getContext()).startActivityForResult(intent, ON_REQUEST_SAVE_FILE);
            }
        });

        buttonConnectDevice = (ImageButton) v.findViewById(R.id.buttonConnectDevice);
        buttonExecute = (ImageButton) v.findViewById(R.id.buttonExecute);
        if(((MainActivity)getActivity()).isConnectedBleDevice()) {
            buttonConnectDevice.setImageResource(R.drawable.ic_icon_connected);
            buttonExecute.setEnabled(true);
            buttonExecute.setImageResource(R.drawable.ic_icon_play);
        }
        else {
            buttonConnectDevice.setImageResource(R.drawable.ic_icon_disconnected);
            buttonExecute.setEnabled(false);
            buttonExecute.setImageResource(R.drawable.ic_icon_play_disabled);
        }

        buttonConnectDevice.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity)getActivity()).disconnectBleDevice();
                ((MainActivity)getActivity()).changeFragmentDeviceSelection();
                ((MainActivity)getActivity()).startBleDeviceScanning();
            }
        });

        buttonExecute.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity)getActivity()).setAllowContinueExecuting(true);
                ((MainActivity)getActivity()).runCode();

                ((MainActivity)getActivity()).onBackPressed();
            }
        });

        buttonViewCode = (ImageButton) v.findViewById(R.id.buttonViewCode);
        buttonViewCode.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity)getActivity()).reqGetGeneratedCodeFromBlockly();
            }
        });

        return v;
    }

    void onConnected() {
        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                buttonConnectDevice.setImageResource(R.drawable.ic_icon_connected);
                buttonExecute.setEnabled(true);
                buttonExecute.setImageResource(R.drawable.ic_icon_play);
            }
        });
    }

    void onDisconnected() {
        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                buttonConnectDevice.setImageResource(R.drawable.ic_icon_disconnected);
                buttonExecute.setEnabled(false);
                buttonExecute.setImageResource(R.drawable.ic_icon_play_disabled);
            }
        });
    }
}
