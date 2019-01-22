package org.oroca.edubot.oroca_edubot_blockly;


import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import io.github.kbiakov.codeview.CodeView;
import io.github.kbiakov.codeview.adapters.Format;
import io.github.kbiakov.codeview.adapters.Options;
import io.github.kbiakov.codeview.classifier.CodeProcessor;
import io.github.kbiakov.codeview.highlight.ColorTheme;
import io.github.kbiakov.codeview.highlight.Font;


/**
 * A simple {@link Fragment} subclass.
 */
public class ViewCodeFragment extends Fragment {

    CodeView mCodeView;
    Context mContext;

    public ViewCodeFragment(Context context) {
        mContext = context;
        //CodeProcessor.init(context);
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View v = inflater.inflate(R.layout.fragment_view_code, container, false);
        mCodeView = (CodeView) v.findViewById(R.id.code_view);

        return v;
    }

    public void setResultCode(String code) {
        mCodeView.setOptions(Options.Default.get(mContext)
                .withFont(Font.DroidSansMonoSlashed)
                .withLanguage("javascript")
                .withFormat(new Format(1, 24, 3, 18))
                .withCode(code)
                .withTheme(ColorTheme.DEFAULT));
    }

}
