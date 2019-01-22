package org.oroca.edubot.oroca_edubot_blockly;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.util.HashMap;
import java.util.List;

public class EdubotController {
    private Context mContext;
    private BluetoothManager mBluetoothManager;
    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothDevice mBluetoothDevice;
    private HashMap<BluetoothGattService, List<BluetoothGattCharacteristic>> mCharacteristics = new HashMap<BluetoothGattService, List<BluetoothGattCharacteristic>>();
    private HashMap<String, BluetoothGattService> mServices = new HashMap<String, BluetoothGattService>();

    public interface OnEdubotControllerListener {
        void onConnected();
        void onDisconnected();
    }

    private OnEdubotControllerListener mOnEdubotControllerListener;
    public void setOnEdubotControllerListener(OnEdubotControllerListener listener) {
        mOnEdubotControllerListener = listener;
    }


    public EdubotController(Context context) {
        mContext = context;
        mBluetoothManager = (BluetoothManager) context.getSystemService(Context.BLUETOOTH_SERVICE);
    }

    public boolean connectToDevice(String address) {
        try {
            mBluetoothAdapter = mBluetoothManager.getAdapter();
        } catch(NullPointerException e) { Toast.makeText(mContext,"Error to get bluetooth adapter.", Toast.LENGTH_LONG).show(); }

        if(!mBluetoothAdapter.isEnabled()) {
            return false;
        }
        mBluetoothDevice = mBluetoothAdapter.getRemoteDevice(address);
        mBluetoothDevice.connectGatt(mContext, false, bleGattCallback);

        return true;
    }

    private final BluetoothGattCallback bleGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            super.onConnectionStateChange(gatt, status, newState);

            if(newState == BluetoothProfile.STATE_CONNECTED) {
                gatt.discoverServices();
            }
            else if(newState == BluetoothProfile.STATE_DISCONNECTED) {
                if(mOnEdubotControllerListener != null) {
                    mOnEdubotControllerListener.onDisconnected();
                }
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            super.onServicesDiscovered(gatt, status);

            if(status == BluetoothGatt.GATT_SUCCESS) {
                mServices.clear();
                mCharacteristics.clear();

                if(mOnEdubotControllerListener != null) {
                    mOnEdubotControllerListener.onConnected();
                }

                List<BluetoothGattService> gattServices = gatt.getServices();
                for(BluetoothGattService gattService: gattServices) {
                    List<BluetoothGattCharacteristic> gattCharacteristics = gattService.getCharacteristics();
                    mServices.put(gattService.getUuid().toString(), gattService);
                    mCharacteristics.put(gattService, gattCharacteristics);

                    Log.i("FFF", gattService.getUuid().toString());
                    Log.i("FFF", gattCharacteristics.toString());
                }
            }
        }

        @Override
        public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            super.onCharacteristicRead(gatt, characteristic, status);
        }

        @Override
        public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            super.onCharacteristicWrite(gatt, characteristic, status);
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
            super.onCharacteristicChanged(gatt, characteristic);
        }

        @Override
        public void onDescriptorRead(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
            super.onDescriptorRead(gatt, descriptor, status);
        }

        @Override
        public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
            super.onDescriptorWrite(gatt, descriptor, status);
        }
    };
}
