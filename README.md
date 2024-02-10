# Proxy FTP en local
Il s'agit ici d'un universitaire réalisé dans le cadre de mon BUT  
Le but est de creer un proxy ftp local renvoyant les requetes vers un serveur ftp accésible sur Internet. Pour cela j'utilise des sockets afin de retransmettres les commandes au serveur.  

<<<<<<< HEAD
***
## Comment lancer le projet ?

### Compiler le projet
Pour compiler ce projet j'utilise gcc mais cela reste possible avec d'autres compilateurs.  

`gcc proxy.c -L -lsimpleSocketAPI -o proxy`
Pour compiler le projet vous pouvez utiliser cette commande. Il faut tout de même vérifier que le compilateur `gcc` est installer sur votre machine Linux.  
Pour cela on peut utiliser la commande `gcc --version` si cela ne revoie rien il faut installer le package **build-essential** avec la commande `sudo apt install build-essential`
