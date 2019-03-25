package adapters;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.TextView;

import org.oroca.edubot.oroca_edubot_blockly.R;

import java.util.List;

public class BleDeviceAdapter extends RecyclerView.Adapter<BleDeviceAdapter.ViewHolder> {

    private OnItemClickListener listener;
    public interface OnItemClickListener {
        void onItemClick(String name, String address);
    }

    public void setOnItemClickListener(OnItemClickListener listener) {
        this.listener = listener;
    }

    public class ViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {

        public TextView mTextDeviceName;
        public TextView mTextDeviceAddress;

        public ViewHolder(View itemView) {
            super(itemView);

            mTextDeviceName = (TextView) itemView.findViewById(R.id.textDeviceName);
            mTextDeviceAddress = (TextView) itemView.findViewById(R.id.textDeviceAddress);

            itemView.setOnClickListener(this::onClick);
        }

        @Override
        public void onClick(View v) {
            if(listener != null) {
                int position = getAdapterPosition();
                if (position != RecyclerView.NO_POSITION) {
                    BleDevice device = mBleDevices.get(position);
                    listener.onItemClick(device.getDeviceName(), device.getDeviceAddress());
                }
            }
        }
    }

    private List<BleDevice> mBleDevices;
    public BleDeviceAdapter(List<BleDevice> bleDevices) {
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
    public void onBindViewHolder(@NonNull BleDeviceAdapter.ViewHolder viewHolder, int i) {
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
