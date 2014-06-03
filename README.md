Ell-i-bare-metal-Hackathon
==========================
Authors:
Andrea Buda, Manik Madhikermi, Jose Granados.

Project goal:
Streaming pulsioximeter data to JavaScript clients via WebSocket server embedded on Ell-i.
A discovery protocol is implemented for requesting available resources.

/embedded
In the embedded folder you'll find the software to be flashed on the Ell-i board.
The code has been developed usign the CooCox CooIDE
Before flashing rember to change the path in >COnfiguration>User (tab)> After Build/Rebuild> Run#1

/web
Here you can find the html and javascript code.
Still TODO > Generalize the sending of the requests after discovery 
NOTE: There is still a bug when requesting getCO2
