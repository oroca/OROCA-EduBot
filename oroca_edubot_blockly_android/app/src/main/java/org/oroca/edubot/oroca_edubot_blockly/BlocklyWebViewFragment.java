package org.oroca.edubot.oroca_edubot_blockly;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;


/**
 * A simple {@link Fragment} subclass.
 */
public class BlocklyWebViewFragment extends Fragment {
    View mRootView = null;
    WebView mWebView;

    public BlocklyWebViewFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        if(mRootView == null) {
            mRootView = inflater.inflate(R.layout.fragment_blockly_webview, container, false);
            mWebView = mRootView.findViewById(R.id.webView);

            mWebView.setWebContentsDebuggingEnabled(true);
            mWebView.setWebChromeClient(new WebChromeClient() {

            });

            WebSettings webSettings = mWebView.getSettings();
            webSettings.setJavaScriptEnabled(true);

            mWebView.loadUrl("file:///android_asset/blockly/webview.html");
        }
        else {
        }
        return mRootView;
    }

    @Override
    public void onDestroyView() {
        if (mRootView.getParent() != null) {
            ((ViewGroup)mRootView.getParent()).removeView(mRootView);
        }
        super.onDestroyView();
    }
}
