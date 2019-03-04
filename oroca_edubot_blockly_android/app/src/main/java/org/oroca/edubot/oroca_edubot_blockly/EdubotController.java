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
import android.graphics.Color;
import android.util.Log;
import android.widget.Toast;

import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.List;
import java.util.UUID;

import static android.graphics.Color.parseColor;

public class EdubotController {
    private Context mContext;
    private BluetoothManager mBluetoothManager;
    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothDevice mBluetoothDevice;
    private BluetoothGatt  mBluetoothGatt;
    private HashMap<BluetoothGattService, List<BluetoothGattCharacteristic>> mCharacteristics = new HashMap<BluetoothGattService, List<BluetoothGattCharacteristic>>();
    private HashMap<String, BluetoothGattService> mServices = new HashMap<String, BluetoothGattService>();


    private static final int    MOTOR_SERVICE_UUID =                      0xe005;
    private static final String MOTOR_CHARACTERISTIC_SET_STEP_UUID =      "34443c33-3356-11e9-b210-d663bd873d93";
    private static final String MOTOR_CHARACTERISTIC_SET_SPEED_UUID =     "34443c34-3356-11e9-b210-d663bd873d93";
    private static final String MOTOR_CHARACTERISTIC_SET_DISTANCE_UUID =  "34443c35-3356-11e9-b210-d663bd873d93";
    private static final String MOTOR_CHARACTERISTIC_SET_ACCEL_UUID =     "34443c36-3356-11e9-b210-d663bd873d93";

    private static final int    MISC_SERVICE_UUID =                       0xe006;
    private static final String MISC_CHARACTERISTIC_COLOR_LED_UUID =      "34443c37-3356-11e9-b210-d663bd873d93";
    private static final String MISC_CHARACTERISTIC_PLAY_SOUND_UUID =     "34443c38-3356-11e9-b210-d663bd873d93";
    private static final String MISC_CHARACTERISTIC_SET_TEXT_OLED_UUID =  "34443c39-3356-11e9-b210-d663bd873d93";
    private static final String MISC_CHARACTERISTIC_SET_IMAGE_OLED_UUID = "34443c3a-3356-11e9-b210-d663bd873d93";
    private static final String MISC_CHARACTERISTIC_STATUS_INFO_UUID =    "34443c3b-3356-11e9-b210-d663bd873d93";

    private static final int    SENSOR_SERVICE_UUID =                        0xe007;
    private static final String SENSOR_CHARACTERISTIC_FLOOR_SENSORS_UUID =   "34443c3c-3356-11e9-b210-d663bd873d93";
    private static final String SENSOR_CHARACTERISTIC_DISTANCE_SENSOR_UUID = "34443c3d-3356-11e9-b210-d663bd873d93";
    private static final String SENSOR_CHARACTERISTIC_IMU_SENSOR_UUID =      "34443c3e-3356-11e9-b210-d663bd873d93";
    private static final String SENSOR_CHARACTERISTIC_ALL_DATA_UUID =        "34443c3f-3356-11e9-b210-d663bd873d93";

    private BluetoothGattService mMotorService;
    private BluetoothGattCharacteristic mMotorSetStepChar;
    private BluetoothGattCharacteristic mMotorSetSpeedChar;
    private BluetoothGattCharacteristic mMotorSetDistanceChar;
    private BluetoothGattCharacteristic mMotorSetAccelChar;

    private BluetoothGattService mMiscService;
    private BluetoothGattCharacteristic mMiscColorLEDChar;
    private BluetoothGattCharacteristic mMiscPlaySoundChar;
    private BluetoothGattCharacteristic mMiscSetTextChar;
    private BluetoothGattCharacteristic mMiscSetImageChar;
    private BluetoothGattCharacteristic mMiscStatusInfoChar;

    private BluetoothGattService mSensorService;
    private BluetoothGattCharacteristic mSensorFloorSensorChar;
    private BluetoothGattCharacteristic mSensorDistanceSensorChar;
    private BluetoothGattCharacteristic mSensorImuSensorChar;
    private BluetoothGattCharacteristic mSensorAllDataChar;

    private boolean mIsConnected = false;
    private int mMaxVelocity = 300;

    private boolean mDataIsMoving = false;
    private double mDataBattVoltage = 0.0;
    private boolean mDataIsNeedCharging = false;
    private int mDataButtonState = 0;
    private boolean mDataIsButtonPressed = false;
    private int[] mDataFloorSensor = {0, 0, 0, 0};
    private int[] mDataDistanceSensor = {0, 0};
    private double[] mDataImuSensor = {0.0, 0.0, 0.0};
    private double[] mDataAccSensor = {0.0, 0.0, 0.0};
    private double[] mDataGyroSensor = {0.0, 0.0, 0.0};

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

    private UUID convertFromInteger(int i) {
        final long MSB = 0x0000000000001000L;
        final long LSB = 0x800000805f9b34fbL;
        long value = i & 0xFFFFFFFF;
        return new UUID(MSB | (value << 32), LSB);
    }

    public boolean connectToDevice(String address) {
        try {
            mBluetoothAdapter = mBluetoothManager.getAdapter();
        } catch(NullPointerException e) { Toast.makeText(mContext,"Error to get bluetooth adapter.", Toast.LENGTH_LONG).show(); }

        if(!mBluetoothAdapter.isEnabled()) {
            return false;
        }
        mBluetoothDevice = mBluetoothAdapter.getRemoteDevice(address);
        mBluetoothGatt = mBluetoothDevice.connectGatt(mContext, false, bleGattCallback);
        return true;
    }

    public void disconnectFromDevice() {
        mBluetoothGatt.disconnect();
    }


    private final BluetoothGattCallback bleGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            super.onConnectionStateChange(gatt, status, newState);

            if(newState == BluetoothProfile.STATE_CONNECTED) {
                // For change the maximum size of BLE packet size.
                gatt.requestMtu(60);
            }
            else if(newState == BluetoothProfile.STATE_DISCONNECTED) {
                if(mOnEdubotControllerListener != null) {
                    mOnEdubotControllerListener.onDisconnected();
                    mIsConnected = false;
                }
            }
        }

        @Override
        public void onMtuChanged(BluetoothGatt gatt, int mtu, int status) {
            super.onMtuChanged(gatt, mtu, status);
            gatt.discoverServices();
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            super.onServicesDiscovered(gatt, status);

            if(status == BluetoothGatt.GATT_SUCCESS) {
                mServices.clear();
                mCharacteristics.clear();

                if(mOnEdubotControllerListener != null) {
                    mOnEdubotControllerListener.onConnected();
                    mIsConnected = true;
                }

                List<BluetoothGattService> gattServices = gatt.getServices();
                for(BluetoothGattService gattService: gattServices) {
                    List<BluetoothGattCharacteristic> gattCharacteristics = gattService.getCharacteristics();
                    mServices.put(gattService.getUuid().toString(), gattService);
                    mCharacteristics.put(gattService, gattCharacteristics);
                }

                mMotorService = mBluetoothGatt.getService(convertFromInteger(MOTOR_SERVICE_UUID));
                mMotorSetStepChar = mMotorService.getCharacteristic(UUID.fromString(MOTOR_CHARACTERISTIC_SET_STEP_UUID));
                mMotorSetSpeedChar = mMotorService.getCharacteristic(UUID.fromString(MOTOR_CHARACTERISTIC_SET_SPEED_UUID));
                mMotorSetDistanceChar = mMotorService.getCharacteristic(UUID.fromString(MOTOR_CHARACTERISTIC_SET_DISTANCE_UUID));
                mMotorSetAccelChar = mMotorService.getCharacteristic(UUID.fromString(MOTOR_CHARACTERISTIC_SET_ACCEL_UUID));

                mMiscService = mBluetoothGatt.getService(convertFromInteger(MISC_SERVICE_UUID));
                mMiscColorLEDChar = mMiscService.getCharacteristic(UUID.fromString(MISC_CHARACTERISTIC_COLOR_LED_UUID));
                mMiscSetTextChar = mMiscService.getCharacteristic(UUID.fromString(MISC_CHARACTERISTIC_SET_TEXT_OLED_UUID));
                mMiscSetImageChar = mMiscService.getCharacteristic(UUID.fromString(MISC_CHARACTERISTIC_SET_IMAGE_OLED_UUID));
                mMiscPlaySoundChar = mMiscService.getCharacteristic(UUID.fromString(MISC_CHARACTERISTIC_PLAY_SOUND_UUID));
                mMiscStatusInfoChar = mMiscService.getCharacteristic(UUID.fromString(MISC_CHARACTERISTIC_STATUS_INFO_UUID));

                mSensorService = mBluetoothGatt.getService(convertFromInteger(SENSOR_SERVICE_UUID));
                mSensorAllDataChar = mSensorService.getCharacteristic((UUID.fromString(SENSOR_CHARACTERISTIC_ALL_DATA_UUID)));
                mBluetoothGatt.setCharacteristicNotification(mSensorAllDataChar, true);

                BluetoothGattDescriptor descriptor = mSensorAllDataChar.getDescriptor(convertFromInteger(0x2902));
                descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
                mBluetoothGatt.writeDescriptor(descriptor);
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

            byte[] data = characteristic.getValue();

            mDataIsMoving = data[0] == 1 ? true : false;
            mDataBattVoltage = data[1] / 10.0;
            mDataIsNeedCharging = data[2] == 1 ? true : false;
            mDataButtonState = data[3];
            mDataIsButtonPressed = data[3] == 1 ? true : false;

            mDataFloorSensor[0] = data[4] & 0xff;
            mDataFloorSensor[1] = data[5] & 0xff;
            mDataFloorSensor[2] = data[6] & 0xff;
            mDataFloorSensor[3] = data[7] & 0xff;

            mDataDistanceSensor[0] = (((data[9] << 8) & 0xff00) + (data[8] & 0xff));
            mDataDistanceSensor[1] = (((data[11] << 8) & 0xff00) + (data[10] & 0xff));

            int imu_x = (short)(((data[13] << 8) & 0xff00) | (data[12] & 0xff));
            int imu_y = (short)(((data[15] << 8) & 0xff00) | (data[14] & 0xff));
            int imu_z = (short)(((data[17] << 8) & 0xff00) | (data[16] & 0xff));

            mDataImuSensor[0] = imu_x / 100.0;
            mDataImuSensor[1] = imu_y / 100.0;
            mDataImuSensor[2] = imu_z / 100.0;

            int acc_x = (short)(((data[19] << 8) & 0xff00) | (data[18] & 0xff));
            int acc_y = (short)(((data[21] << 8) & 0xff00) | (data[20] & 0xff));
            int acc_z = (short)(((data[23] << 8) & 0xff00) | (data[22] & 0xff));

            mDataAccSensor[0] = acc_x / 100.0;
            mDataAccSensor[1] = acc_y / 100.0;
            mDataAccSensor[2] = acc_z / 100.0;

            int gyro_x = (short)(((data[25] << 8) & 0xff00) | (data[24] & 0xff));
            int gyro_y = (short)(((data[27] << 8) & 0xff00) | (data[26] & 0xff));
            int gyro_z = (short)(((data[29] << 8) & 0xff00) | (data[28] & 0xff));

            mDataGyroSensor[0] = gyro_x / 100.0;
            mDataGyroSensor[1] = gyro_y / 100.0;
            mDataGyroSensor[2] = gyro_z / 100.0;
        }

        @Override
        public void onDescriptorRead(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
            super.onDescriptorRead(gatt, descriptor, status);
        }

        @Override
        public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
            super.onDescriptorWrite(gatt, descriptor, status);

            BluetoothGattCharacteristic characteristic = gatt.getService(convertFromInteger(SENSOR_SERVICE_UUID))
                    .getCharacteristic(UUID.fromString(SENSOR_CHARACTERISTIC_ALL_DATA_UUID));
            characteristic.setValue(new byte[]{1, 1});
            gatt.writeCharacteristic(characteristic);
        }
    };

    public boolean getDataIsMoving() { return mDataIsMoving; }
    public double getDataBattVoltage() { return mDataBattVoltage; }
    public boolean getDataIsNeedCharging() { return mDataIsNeedCharging; }
    public int getDataButtonState() { return mDataButtonState; }
    public boolean getDataIsButtonPressed() { return mDataIsButtonPressed; }
    public int getDataFloorSensor(int index) { return mDataFloorSensor[index]; }
    public int getDataDistanceSensor(int index) { return mDataDistanceSensor[index]; }
    public double getDataImuSensor(int index) { return mDataImuSensor[index]; }
    public double getDataAccSensor(int index) { return mDataAccSensor[index]; }
    public double getDataGyroSensor(int index) { return mDataGyroSensor[index]; }

    public int getMotorMaxVelocity() { return mMaxVelocity; }
    public void setMotorMaxVelocity(int vel) {
        mMaxVelocity = Math.max(Math.min(vel, 300), 0);
    }

    public void motorSetStep(int l_step, int r_step) {
        byte[] data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        data[0] = (byte)((l_step >> 24) & 0xff);
        data[1] = (byte)((l_step >> 16) & 0xff);
        data[2] = (byte)((l_step >> 8) & 0xff);
        data[3] = (byte)( l_step & 0xff);
        data[4] = (byte)((r_step >> 24) & 0xff);
        data[5] = (byte)((r_step >> 16) & 0xff);
        data[6] = (byte)((r_step >> 8) & 0xff);
        data[7] = (byte)( r_step & 0xff);
        data[8] = (byte)((mMaxVelocity >> 8) & 0xff);
        data[9] = (byte)( mMaxVelocity & 0xff);

        if(mMotorSetStepChar.setValue(data)) {
            while(!mBluetoothGatt.writeCharacteristic(mMotorSetStepChar) && mIsConnected);
        }
    }

    public void motorSetVelocity(int l_vel, int r_vel) {
        int left_vel = Math.max(Math.min(l_vel, 300), -300);
        int right_vel = Math.max(Math.min(r_vel, 300), -300);

        byte[] data = {0, 0, 0, 0, 0, 0};

        data[0] = (byte)((left_vel >> 8) & 0xff);
        data[1] = (byte)((left_vel) & 0xff);
        data[2] = (byte)((right_vel >> 8) & 0xff);
        data[3] = (byte)((right_vel) & 0xff);
        data[4] = 0;
        data[5] = 0;

        if(mMotorSetSpeedChar.setValue(data)) {
            while(!mBluetoothGatt.writeCharacteristic(mMotorSetSpeedChar) && mIsConnected);
        }
    }

    public void motorSetDistance(int l_dist, int r_dist) {
        byte[] data = {0, 0, 0, 0, 0, 0};

        data[0] = (byte)((l_dist >> 8) & 0xff);
        data[1] = (byte)( l_dist & 0xff);
        data[2] = (byte)((r_dist >> 8) & 0xff);
        data[3] = (byte)( r_dist & 0xff);
        data[4] = (byte)((mMaxVelocity >> 8) & 0xff);
        data[5] = (byte)( mMaxVelocity & 0xff);

        if(mMotorSetDistanceChar.setValue(data)) {
            while(!mBluetoothGatt.writeCharacteristic(mMotorSetDistanceChar) && mIsConnected);
        }
    }

    public void motorSetAccel(int l_acc, int r_acc) {
        int left_acc = Math.max(Math.min(l_acc, 4), 0);
        int right_acc = Math.max(Math.min(r_acc, 4), 0);

        byte[] data = {0, 0};

        data[0] = (byte)(left_acc & 0xff);
        data[1] = (byte)(right_acc & 0xff);

        if(mMotorSetAccelChar.setValue(data)) {
            while(!mBluetoothGatt.writeCharacteristic(mMotorSetAccelChar) && mIsConnected);
        }
    }

    public void miscSetColorLed(String left_rgb_hex, String right_rgb_hex) {
        int l_rgb = Color.parseColor(left_rgb_hex);
        int r_rgb = Color.parseColor(right_rgb_hex);

        byte[] data = {0, 0, 0, 0, 0, 0};

        data[0] = (byte)((l_rgb >> 16) & 0xff);
        data[1] = (byte)((l_rgb >> 8) & 0xff);
        data[2] = (byte)((l_rgb) & 0xff);

        data[3] = (byte)((r_rgb >> 16) & 0xff);
        data[4] = (byte)((r_rgb >> 8) & 0xff);
        data[5] = (byte)((r_rgb) & 0xff);

        if(mMiscColorLEDChar.setValue(data)) {
            while(!mBluetoothGatt.writeCharacteristic(mMiscColorLEDChar) && mIsConnected);
        }
    }

    public void miscSetText(String str) {
        byte[] data = str.getBytes(StandardCharsets.UTF_8);

        if(mMiscSetTextChar.setValue(data)) {
            while(!mBluetoothGatt.writeCharacteristic(mMiscSetTextChar) && mIsConnected);
        }
    }

    public void miscSetImage(int index) {
        byte[] data = {0};

        data[0] = (byte)(index & 0xff);

        if(mMiscSetImageChar.setValue(data)) {
            while(!mBluetoothGatt.writeCharacteristic(mMiscSetImageChar) && mIsConnected);
        }
    }

    public void miscPlaySound(int index) {
        byte[] data = {0};

        data[0] = (byte)(index & 0xff);

        if(mMiscPlaySoundChar.setValue(data)) {
            while(!mBluetoothGatt.writeCharacteristic(mMiscPlaySoundChar) && mIsConnected);
        }
    }
}
