# Setting up and running the tests

This mynewt test project contains YAKINDU SCT test models and corresponding mynewt unit tests which check that the generated state machine code properly works in the mynewt environment. 
It also serves as an example how YAKINDU state machines are used using the mynewt C code generator. 


### Test project structure

This project consistst of a skeleton of a Mynewt workspace. It is also configured as an Eclipse project so that you can use the YAKINDU Statechart Tools 
to edit the contained statechart models and to generate code from it.


### editing the test statecharts

1. Install YAKINDU Statechart Toools Professional Edition version 3.4 or higher. Or install the plugins into an Eclipse distribution of your choice. The professional edition is required for the Deep-C-integration tests.
2. Create a workspace and import this project.
3. The statechart model are located in 
   * sct_tests/apps/simple/model - for default domain generator tests 
   * sct_tests/apps/cdomain/model - for C domain generator tests
4. Edit the statecharts


### generating the state machine code 

1. Use the .sgen files to generate code from the statechart models.

The generated code will be generated to the location specified in the sgen files. It will also be checked into the repository. So don't forget to commit and push your changes.


### editing tests

The test project makes use of the Mynewt unit test framework. You will find the test code in the 'test/src/testcases' directory of each Mynewt app.
You may edit it within Eclipse or by using the IDE/editor of your choice.


### running the tests

The project as checked out from the repository just consists of a Mynewt workspace skeleton. 
 
1. The setup makes use of the Mynewt docker image. So setup the docker image.
1. You have to install the required dependencies. Open a terminal window and change the directory 
      
	cd mynewt_tests/sct_tests

1. Install the dependencies. A lot of stuff will be downloaded. You'll find it in the newly created 'repos' directory. It is about 400MB and thats why this folder is git ignored.
	
	../newt install

1.  Run the tests.

	../newt test apps/simple
	../newt test apps/cdomain
	