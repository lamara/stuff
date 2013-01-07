package com.example.an.example.app;

import java.util.Random;

import android.app.Activity;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {
	//array holding the audio tracks to play, if this number is every updated you
	//have to update info in returnTier() as well
    private int[] tracks = new int[10];
    //counts the number of times play button is pushed, used for keeping
    //track of the tiers used on playback
    private int counter = 0;

	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //ID for first audio file in raw folder == 2130968576, the next file's
        //ID in the raw folder is the previous file's ID + 1, so we can iterate through
        //them using a simple numeric for loop (hardcoding the resource IDs is
        //really bad if we can find a way to not do that then implement it).
        int tracksLen = tracks.length;
        for (int i = 0; i < tracksLen; i++ ) {
        	//adds resource ids to tracks
        	tracks[i] = 2130968576+i;
        }
        
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }
    
    //onClickListener for the sound button
    public void playSound(View view) throws IllegalArgumentException, IllegalAccessException {
    	//retrieves the main sound buttton
    	Button button;
    	button = (Button)findViewById(R.id.button1);
    	//finds a random track to play that is inclusive
    	//in the current tier
    	int lowRange = returnTier()[0];
    	int highRange = returnTier()[1];
    	//selects a random track within the given range
    	Random randomGenerator = new Random();
    	int randomTrack =  randomGenerator.nextInt(highRange - lowRange) + lowRange;
    	final MediaPlayer sound = MediaPlayer.create(this, tracks[randomTrack]);
    	sound.start();
    	//disables button while track is playing
    	boolean disabled = false;
    	while (sound.isPlaying()) {
    		if (!disabled) {
    			button.setEnabled(false);
    			disabled = true;
    		}
    	}
    	sound.release();
    	button.setEnabled(true);
    	counter++;
    }
    	

    public int[] returnTier() {
    	//creates the range of the current tier, low number
    	//is inclusive, high number is exclusive
    	int[] range = new int[2];
    	//first tier
    	if (counter < 2) {
    		range[0] = 0;
    		range[1] = 2;
    	}
    	//second tier and so on
    	else if (counter >= 2 && counter < 5) {
    		range[0] = 2;
    		range[1] = 5;
    	}
    	else if (counter >= 5 && counter < 8) {
    		range[0] = 5;
    		range[1] = 8;
    	}
    	else if (counter >= 8 && counter < 10) {
    		range[0] = 8;
    		range[1] = 10;
    	}
    	//counter reached limit, so it is reset.
    	else {
    		counter = 0;
    		return returnTier();
    	}
     	return range;
    }
    
}
