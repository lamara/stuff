package com.example.calculator;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.TextView;

public class CalculatorActivity extends Activity {
	
	//currentVal is the text that populates the results screen,
	//all computations are done based on its current state
	private String currentVal = "";
	private boolean calculated = false;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_calculator);
        //retrieves global variables from the bundle
        if (savedInstanceState != null) {
        	String retrievedVal = savedInstanceState.getString("Value");
        	if (retrievedVal != null) {
        		currentVal = retrievedVal;
        	}
        	Boolean retrievedBool = savedInstanceState.getBoolean("Calculated");
        	if (retrievedBool != null) {
        		calculated = retrievedBool;
        	}
        }
        //updates number display
    	TextView numberScreen = ((TextView)findViewById (R.id.textView));
    	numberScreen.setText(currentVal);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_calculator, menu);
        return true;
    }
    
    @Override
    public void onSaveInstanceState(Bundle savedInstanceState) 
    {
      // Store UI state to the savedInstanceState.
      // This bundle will be passed to onCreate on next call.
      String savedVal = currentVal;
      Boolean savedBool = calculated;
      savedInstanceState.putString("Value", savedVal);
      savedInstanceState.putBoolean("Calculated", savedBool);
      super.onSaveInstanceState(savedInstanceState);
    }
    
    
    
    //onClickListener for all of the buttons in the App
    public void parseButton(View button) {
    	
    	TextView numberScreen = ((TextView)findViewById (R.id.textView));
    	if (calculated) {
    		currentVal = "";
    		calculated = false;
    	}
    	switch(button.getId()) {
    		case R.id.num0: 
    			currentVal = currentVal + "0"; break;
    		case R.id.num1:
    			currentVal = currentVal + "1"; break;
    		case R.id.num2:
    			currentVal = currentVal + "2"; break;
    		case R.id.num3:
    			currentVal = currentVal + "3"; break;
    		case R.id.num4:
    			currentVal = currentVal + "4"; break;
    		case R.id.num5: 
    			currentVal = currentVal + "5"; break;
    		case R.id.num6:
    			currentVal = currentVal + "6"; break;
    		case R.id.num7:
    			currentVal = currentVal + "7"; break;
    		case R.id.num8:
    			currentVal = currentVal + "8"; break;
    		case R.id.num9:
    			currentVal = currentVal + "9"; break;
    		case R.id.plus:
    			currentVal = currentVal + "+"; break;
    		case R.id.minus:
    			currentVal = currentVal + "-"; break;
    		case R.id.multiply:
    			currentVal = currentVal + "*"; break;
    		case R.id.divide:
    			currentVal = currentVal + "/"; break;
    		case R.id.clear:
    			currentVal = "";			   break;
    		case R.id.del:
    			currentVal = currentVal + '.'; break;
    		case R.id.enter:
    		{
    			if (currentVal.length() == 0) {
    				break;
    			}
    			char beginChar = currentVal.charAt(0);
    			char endChar = currentVal.charAt(currentVal.length() - 1);
    			//returns "Error" if string starts or ends with an operator
    			if (beginChar == '+' || beginChar == '-' || beginChar == '*' || beginChar == '/'
    				|| endChar == '+' || endChar == '-' || endChar == '*' || endChar == '/') {
    				currentVal = "Error";
    				calculated = true;
    				break;
    			}
    			currentVal = Calculator.calculate(currentVal);
    			calculated = true;
    			break;
    		}
    			
    	}
		numberScreen.setText(currentVal);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
