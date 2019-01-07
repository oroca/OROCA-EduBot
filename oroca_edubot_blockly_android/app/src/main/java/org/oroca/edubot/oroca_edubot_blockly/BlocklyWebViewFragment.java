package org.oroca.edubot.oroca_edubot_blockly;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;

public class BlocklyWebViewFragment extends Fragment {
    WebView mWebView;
    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.blockly_webview, null);
        mWebView = view.findViewById(R.id.webviewBlockly);
        mWebView.setWebContentsDebuggingEnabled(true);
        mWebView.setWebChromeClient(new WebChromeClient() {

        });

        WebSettings webSettings = mWebView.getSettings();
        webSettings.setJavaScriptEnabled(true);

        mWebView.loadUrl("file:///android_asset/blockly/webview.html");
        return view;
    }

    public void TestFunction() {
        Log.i("EEEE", "asdlfkjasldkfjalsdkjfalskjflks");
    }
}
