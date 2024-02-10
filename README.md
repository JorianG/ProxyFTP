# Proxy FTP en local
Il s'agit ici d'un universitaire réalisé dans le cadre de mon BUT  
Le but est de creer un proxy ftp local renvoyant les requetes vers un serveur ftp accésible sur Internet. Pour cela j'utilise des sockets afin de retransmettres les commandes au serveur.  


***
## Comment lancer le projet ?

### Compiler le projet
Pour compiler ce projet j'utilise gcc mais cela reste possible avec d'autres compilateurs.  

`gcc proxy.c -L -lsimpleSocketAPI -o proxy`  
  
Pour compiler le projet vous pouvez utiliser cette commande. Il faut tout de même vérifier que le compilateur `gcc` est installé sur votre machine Linux.  
Pour cela on peut utiliser la commande `gcc --version` si cela ne revoie rien il faut installer le package **build-essential** avec la commande `sudo apt install build-essential`


### Lancer le projet 
Si la derniére étape a été réalisée vous devriez avoir un fichier `proxy` dans le dossier de ce projet. Il s'agit de la version compilée du code pour la lancer utilisez  
`./proxy`
   
Cela devrait afficher l'adresse ip '127.0.0.1' ainsi qu'un port d'écoute. Il suffit ensuite de se connecter a cette adresse en ftp avec la commande 
`ftp localhost PORT`
en remplacant le PORT par le port obtenue plus tôt.  

Il faut ensuite entre le 'Name' qui ce compose comme cela `anonymous@SERVEUR_FTP` par exemple `anonymous@ftp.de.debian.org` pour utiliser le server ftp de Debian en Allemagne. On peut ensuite continuer sans mot de passe puisque on se connecter en anonymous.  

Une fois cela fait nous devrions pouvoir entrer des comandes dans la console FTP qui seront relayés au server distant. Par exemple 'ls' nous renvoie la liste des fichiers du serveur.


