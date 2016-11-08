

Assignment 4: Replication
EGP-405-01
By David Jordan
Due 11/7/2016


To accomplish the replication, I chose to follow the slides by setting up a Replication Manager, equipped with a Linking Context and Object Creation Registry, to help automatically serialize/deserialize object states. The abstract GameObject class has virtual methods for serialize and deserialize, so subclasses (Archer and TownHall) are responsible for serial/deserializing themselves, given a bit stream and linking context. I also implemented RPC, using an RPC Manager. The single RPC I made is SpawnObject ('SPWN'), which takes a game object as a parameter. Clients can send SpawnObject RPCs to the server, where the game objects then get created, through the RPC's function.

The server program is what sets up the initial game state, creating the town hall and archer objects. Whenever a client connects, the server sends them a replication packet containing the entire game state. It also responds to RPCs in replication packets received from its clients.

A replication packet is a list of replication actions (either state-replication or RPC), which is terminated by a invalid action identifier. By having a list, multiple object states and/or RPCs can be sent in a single packet.

The client program will connect to the server, then receive a replication packet containing the entire game state. It will then ask for the name of a town hall to create through an RPC called "Spawn Object". After the RPC is executed on the server, another replication packet is broadcasted to all clients, updating their states after a new object was spawned. Lastly, the client disconnects after 1 second and prints its state, also outputting its state to a file called 'state.txt'.


NOTE: I decided for the names:
  'TownHall' instead of 'TownCentre'
  'Human'    instead of 'Werewolf'
  'Undead'   instead of 'Vampire'
  
  