package adapters;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import org.oroca.edubot.oroca_edubot_blockly.R;

import java.util.List;

public class BleDeviceAdaper extends RecyclerView.Adapter<BleDeviceAdaper.ViewHolder> {

    public class ViewHolder extends RecyclerView.ViewHolder {

        public TextView mTextDeviceName;
        public TextView mTextDeviceAddress;

        public ViewHolder(View itemView) {
            super(itemView);

            mTextDeviceName = (TextView) itemView.findViewById(R.id.textDeviceName);
            mTextDeviceAddress = (TextView) itemView.findViewById(R.id.textDeviceAddress);
        }
    }

    private List<BleDevice> mBleDevices;
    public BleDeviceAdaper(List<BleDevice> bleDevices) {
        mBleDevices = bleDevices;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        Context context = viewGroup.getContext();
        LayoutInflater inflater = LayoutInflater.from(context);

        View v = inflater.inflate(R.layout.item_device, viewGroup, false);
        ViewHolder viewHolder = new ViewHolder(v);
        return viewHolder;
    }

    @Override
    public void onBindViewHolder(@NonNull BleDeviceAdaper.ViewHolder viewHolder, int i) {
        BleDevice device = mBleDevices.get(i);

        TextView textName = viewHolder.mTextDeviceName;
        textName.setText(device.getDeviceName());

        TextView textAddress = viewHolder.mTextDeviceAddress;
        textAddress.setText(device.getDeviceAddress());
    }

    @Override
    public int getItemCount() {
        return mBleDevices.size();
    }
}
