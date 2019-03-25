package org.oroca.edubot.oroca_edubot_blockly;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Base64;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import java.io.IOException;
import java.io.InputStream;
import java.util.Locale;


/**
 * A simple {@link Fragment} subclass.
 */
public class BlocklyWebViewFragment extends Fragment {
    View mRootView = null;
    String mLocale;
    WebView mWebView;

    public BlocklyWebViewFragment() {
        // Required empty public constructor
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        Locale locale = getContext().getResources().getConfiguration().locale;
        mLocale = locale.getLanguage();

        // Inflate the layout for this fragment
        if(mRootView == null) {
            mRootView = inflater.inflate(R.layout.fragment_blockly_webview, container, false);
            mWebView = mRootView.findViewById(R.id.webView);

            mWebView.setWebContentsDebuggingEnabled(true);
            mWebView.setWebChromeClient(new WebChromeClient() {
            });

            WebSettings webSettings = mWebView.getSettings();
            webSettings.setJavaScriptEnabled(true);
            mWebView.addJavascriptInterface(getContext(), "Android");

            switch(mLocale) {
                case "en":
                    mWebView.loadUrl("file:///android_asset/blockly/webview.html");
                    break;
                case "ko":
                    mWebView.loadUrl("file:///android_asset/blockly/webview_ko.html");
                    break;
                default:
                    mWebView.loadUrl("file:///android_asset/blockly/webview.html");
                    break;
            }
        }
        else {
        }
        return mRootView;
    }

    public void reqGetXMLTextFromWorkspace() {
        String script = "javascript:Android.onResponseData(\"getXmlTextFromWorkspace\", ";
        script = script.concat("getXmlTextFromWorkspace());");
        mWebView.loadUrl(script);
    }

    public void reqRestoreXmlTextToWorkspace(String xml_text) {
        String script = "javascript:Android.onResponseData(\"restoreXmlTextToWorkspace\", ";
        String conv_string = xml_text.replace("\"", "\\\"");
        script = script.concat("restoreXmlTextToWorkspace(\"" +
                conv_string + "\"));");
        mWebView.loadUrl(script);
    }

    public void reqGetGeneratedCodeFromBlockly() {
        String script = "javascript:Android.onResponseData(\"getGeneratedCodeFromWorkspace\", ";
        script = script.concat("generateCodeOnly());");
        mWebView.loadUrl(script);
    }

    public void reqRunCode() {
        String script = "javascript:generateCodeAndLoadIntoInterpreter();";
        mWebView.loadUrl(script);

        script = "javascript:runCode();";
        mWebView.loadUrl(script);
    }

    @Override
    public void onDestroyView() {
        if (mRootView.getParent() != null) {
            ((ViewGroup)mRootView.getParent()).removeView(mRootView);
        }
        super.onDestroyView();
    }
}
