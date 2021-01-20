package com.example.user.webveiw;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.webkit.WebView;
import android.widget.Toast;

public class A2 extends AppCompatActivity {
    private static final String DEFAULT ="" ;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_a2);




        SharedPreferences e = this.getSharedPreferences("save_file", Context.MODE_PRIVATE);
        String name =e.getString("saveunder", DEFAULT);

        if (name.equals(DEFAULT)){
            Toast.makeText(getBaseContext(), "no value present ",Toast.LENGTH_LONG ).show();
        }
        else {
            String url="http://"+name;

            WebView veiw=(WebView) this.findViewById(R.id.webveiw);

            veiw.getSettings().setJavaScriptCanOpenWindowsAutomatically(true);
            veiw.loadUrl(url);


        }




    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.restor) {
            Intent i=new Intent (this,A2.class);

            SharedPreferences e = this.getSharedPreferences("save_file", Context.MODE_PRIVATE);
            String name =e.getString("saveunder", DEFAULT);

            if (name.equals(DEFAULT)){
                Toast.makeText(getBaseContext(), "no value present ",Toast.LENGTH_LONG ).show();
            }
            else {
                String url="http://"+name;

                WebView veiw=(WebView) this.findViewById(R.id.webveiw);

                veiw.getSettings().setJavaScriptCanOpenWindowsAutomatically(true);
                veiw.loadUrl(url);


            }
            startActivity(i);




        }
        if (id == R.id.back) {
            Intent i=new Intent(this,MainActivity.class);
            startActivity(i);

        }

        return super.onOptionsItemSelected(item);

    }
    @Override
    protected  void onResume(){
        super.onResume();




    }

}
