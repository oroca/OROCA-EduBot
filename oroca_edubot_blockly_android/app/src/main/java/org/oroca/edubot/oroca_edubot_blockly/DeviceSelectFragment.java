package org.oroca.edubot.oroca_edubot_blockly;


import android.bluetooth.le.ScanResult;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;

import java.util.ArrayList;

import adapters.BleDevice;
import adapters.BleDeviceAdapter;


/**
 * A simple {@link Fragment} subclass.
 */
public class DeviceSelectFragment extends Fragment {

    ImageButton buttonReload;
    ArrayList<BleDevice> mDeviceList;
    RecyclerView mRecyclerView;
    BleDeviceAdapter mBleDeviceAdapter;


    public DeviceSelectFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.fragment_device_select, container, false);

        buttonReload = (ImageButton) v.findViewById(R.id.buttonReloadDevice);
        buttonReload.setEnabled(false);
        buttonReload.setImageResource(R.drawable.ic_icon_reload_disabled);

        buttonReload.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mDeviceList.clear();
                mBleDeviceAdapter.notifyDataSetChanged();

                ((MainActivity)getActivity()).startBleDeviceScanning();
                buttonReload.setEnabled(false);
                buttonReload.setImageResource(R.drawable.ic_icon_reload_disabled);
            }
        });

        mRecyclerView = (RecyclerView) v.findViewById(R.id.recyclerViewDevices);
        mDeviceList = new ArrayList<>();
        mBleDeviceAdapter = new BleDeviceAdapter(mDeviceList);
        mRecyclerView.setAdapter(mBleDeviceAdapter);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(getContext(), LinearLayoutManager.HORIZONTAL, false));

        mBleDeviceAdapter.setOnItemClickListener(new BleDeviceAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(String name, String address) {
                ((MainActivity)getActivity()).connectBleDevice(name, address);
            }
        });

        return v;
    }

    void onScanResult(ScanResult result) {
        boolean is_duplicated = false;
        for(int i = 0; i < mDeviceList.size(); i++) {
            if(result.getDevice().getAddress().equals(mDeviceList.get(i).getDeviceAddress())) {
                is_duplicated = true;
            }
        }

        if(is_duplicated) {
            return;
        }

        if(result.getDevice().getName().toLowerCase().contains("edubot")) {
            BleDevice device = new BleDevice(result.getDevice().getName(), result.getDevice().getAddress());
            mDeviceList.add(device);
            mBleDeviceAdapter.notifyDataSetChanged();
        }
    }

    void onCompletedScanning() {
        buttonReload.setEnabled(true);
        buttonReload.setImageResource(R.drawable.ic_icon_reload);
    }

}
