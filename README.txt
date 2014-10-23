Quake 2 modifications Tactics Branch

-turned Quake 2 into a turn-based strategy game
	-requires coop to be on
	-can only be 1v1
		-all other players will be spectators
	-each player starts with 5 units
	-units can only move/attack during the player's turn
	-players have a certain amount of movement and attack points that they can use during their turns
	-movement and attack points are refreshed at the start of each turn
	-players will use a ghost camera when it is not their turn to explored the map
	-turns last a maximum of 1 minute
	-players can switch between units
		-"switchunitf" command to move forwards through the unit list (bound to "[" in my config file)
		-"switchunitb" command to move backwards through the unit list (bound to "]" in my config file)
	-players can end the turn automatically with the "endturn" command (bound to "ENTER" in my config file)
	-player scores will reflect how many units they have alive
		-when a player's score hits 0, the player loses
	-can only play on the map, The Edge, because I forgot that there are other maps
