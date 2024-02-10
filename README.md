# Proxy FTP en local
Il s'agit ici d'un universitaire réalisé dans le cadre de mon BUT  
Le but est de creer un proxy ftp local renvoyant les requetes vers un serveur ftp accésible sur Internet. Pour cela j'utilise des sockets afin de retransmettres les commandes au serveur.  

<<<<<<< HEAD
***
## Comment lancer le projet ?

### Compiler le projet
Pour compiler ce projet j'utilise gcc mais cela reste possible avec d'autres compilateurs.  

`gcc proxy.c -L -lsimpleSocketAPI -o proxy`

