<?php

// Require
require ('./model.php');

// Connexion Base de données
$mysqli = new mysqli("localhost","root","","croixrouge");

$req_type=$_SERVER['REQUEST_METHOD'];

if($req_type ==='GET') {

    // Quantité Minimal
    $quantite = "SELECT NomTypeProduit,Quantitemin FROM TypeProduit";
    $res= $mysqli->query($quantite);
    $Quantite=$res->fetch_assoc();
    echo nl2br(" Réponse : ");
    echo json_encode($Quantite);
    echo "<br />";

    // Date de péremption
    $peremption = "SELECT nomProduit,datePeremption FROM produit ORDER BY datePeremption";
    $resp= $mysqli->query($peremption);

    while($repp=$resp->fetch_assoc()) {
        $Peremption = $repp;
        echo nl2br(" \n Réponse : ");
        echo json_encode($Peremption);
    }
    
}


?>