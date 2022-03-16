<?php

// Require
require ('./model.php');

// Connexion Base de données
$mysqli = new mysqli("localhost","root","","croixrouge");


// Requete quantite minimale d'article atteinte 
$quantite = "SELECT NomTypeProduit,Quantitemin FROM TypeProduit";
$res= $mysqli->query($quantite);
$rep=$res->fetch_assoc();

// Affichage
echo nl2br(" | Quantité Minimal | \n");
print_r($rep);

// Requete produit a date de peremption depassee 
$peremption = "SELECT nomProduit,datePeremption FROM produit ORDER BY datePeremption";
$resp= $mysqli->query($peremption);
while($repp=$resp->fetch_assoc()) {
// Affichage
echo nl2br("\n");
echo nl2br(" | Date de péremption | \n");
print_r($repp);
}



?>