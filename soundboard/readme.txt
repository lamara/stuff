An unfinished soundboard I'm making for a friend. App is mostly complete, currently uses placeholder sounds
and the image is temporary. How the app works is simple: Click on the center button and a sound plays. After
a certain number of button clicks a new tier is unlocked and a different selection of sounds gets played. Upon reaching
the last tier, the sounds reset. 

TO FIX:
	- The button disabling doesn't work as planned, if you press the button while it is disabled it
	will just queue the button press until the sound stops playing.
	- Sound clips are loaded by hardcoding their IDs (which are simple int), this is undesirable because a
	~horrible catastrophe~ will happen if the IDs ever change.