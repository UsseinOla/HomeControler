package com.example.user.webveiw;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;



public class MainActivity extends AppCompatActivity {
    private static final String DEFAULT ="" ;
    String s;
    Button pin1_on;
    Button pin1_off;
    EditText p;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        SharedPreferences e = this.getSharedPreferences("save_file", Context.MODE_PRIVATE);
        final String name =e.getString("saveunder", DEFAULT);
        s=name;
        pin1_on=(Button)findViewById(R.id.button);
        pin1_off=(Button)findViewById(R.id.button2);
        p = (EditText)findViewById(R.id.editText);
      //  p.setText(name);
        pin1_on.setOnClickListener(new View.OnClickListener() {
    @Override
    public void onClick(View view) {
        Intent i=new Intent(getApplicationContext(), A2.class);
        EditText p = (EditText)findViewById(R.id.editText);
        String x = p.getText().toString().concat("/PIN1=ON");
        SharedPreferences e = getApplicationContext().getSharedPreferences("save_file", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor=e.edit();
        editor.putString("saveunder", x);
        editor.commit();

        startActivity(i);
    }
});
        pin1_off.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i=new Intent(getApplicationContext(), A2.class);

                String x = p.getText().toString().concat("/PIN1=OFF");
                SharedPreferences e = getApplicationContext().getSharedPreferences("save_file", Context.MODE_PRIVATE);
                SharedPreferences.Editor editor=e.edit();
                editor.putString("saveunder", x);
                editor.commit();
                startActivity(i);
            }
        });


    }

        public void go(View view){

            Intent i=new Intent(this,A2.class);
            EditText p = (EditText)findViewById(R.id.editText);
            String x = p.getText().toString();
            SharedPreferences e = this.getSharedPreferences("save_file", Context.MODE_PRIVATE);
            SharedPreferences.Editor editor=e.edit();
            editor.putString("saveunder", x);

            startActivity(i);

            editor.commit();


            startActivity(i);
        }







}
