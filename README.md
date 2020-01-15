# e4： the 4th edition of linear track

Import improvements : 

1. only 1 usb is enough for  all the control and recording of each setup.
2. Pseudo random condition (led or context) orders are encoded in Arduino scripts so that the behavioral train will not stop even the log recording is stop.
3. long-time events, such as opto genetics, motor rotation, led flash and so on, are available through extending Arduino nano as slave connected to master Arduino by IIC communication 

Some details: A4, A5 in each Arduino nano are necessary for IIC communication.

## Stage1
	Aim: only for training of licking and alternative licking
	Time_structure:
		nosepoke
		context enter
		context exit
		choice
		context reverse enter
		context reverse exit
## Stage2
	Aim: for training and recording with context
	Time_structure:
		nosepoke
		context enter
		context exit
		choice
		context reverse enter
		context reverse exit
## Stage3
	Aim: for training, testing and recording with context dependent forced choice
	Time_structure:
		nosepoke
		context enter
		context exit
		choice
		context reverse enter
		context reverse exit
## Stage4
	Aim: for training with led pattern dependent forced choice
	Time_structure:
		nosepoke
		choice
	
## Stage5
	Aim: for training, testing and recording with (paired led and context) dependent forced choice
	Time_structure:
		nosepoke
		context enter
		context exit
		choice
		context reverse enter
		context reverse exit
## Stage6
	Aim: for training, testing and recording with (led later than context) dependent forced choice 
	Time_structure:
		nosepoke
		context enter
		context exit
		choice
		context reverse enter
		context reverse exit