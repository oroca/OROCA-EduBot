package adapters;

public class BleDevice {
    private String mDeviceName;
    private String mDeviceAddress;

    public BleDevice(String name, String address) {
        mDeviceName = name;
        mDeviceAddress = address;
    }

    public String getDeviceName() { return mDeviceName; }
    public String getDeviceAddress() { return mDeviceAddress; }
}
