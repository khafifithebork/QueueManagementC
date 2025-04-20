# QueueManagementC
🔁 Fonctionnement :
•	Si un client normal arrive, il va dans la file FIFO.
•	Si un client prioritaire arrive (VIP, urgence), il est inséré dans le tas binaire.
•	Lors du service :
o	On vérifie d'abord la file prioritaire. S’il y a un client, on le sert.
o	Sinon, on sert le client le plus ancien de la file FIFO.
