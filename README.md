## Server simulator

#### Build the Client
    cd [PROJECT_PATH]]/client
    source configure
    buildClient

The client binary can be also build with linking the clientlib as a shared libary
with the command.

    buildClientLib

#### Build the Server
    cd [PROJECT_PATH]]/server
    source configure
    buildServer

#### Run the demo
    cd [PROJECT_PATH]]
    ./demo -n 4 -t 0.1 -d

Demo parameters:
n Number of clients to connect to the server.
t Delay which represents the data aquistion in the server.
d (optional) Connect the clients one after another with aleatory
    delay between 0.5 and 1.5 seconds

The expected output should look like the following picture:

<p align="center">
  <img src="https://github.com/IlievIliya92/serverSimulator/blob/master/demo.png" alt="Demo"/>
</p>


