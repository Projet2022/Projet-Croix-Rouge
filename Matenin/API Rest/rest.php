<?php

// Affichage des erreurs
error_reporting(E_ALL);
ini_set('display_errors', '1');

// Require
require ('./model.php');

// Connexion Base de données
$mysqli = new mysqli("localhost","projet","projet","Croix Rouge");

$req_type=$_SERVER['REQUEST_METHOD'];

if($req_type ==='GET') {

    // Quantité Minimal
    $quantite = "SELECT NomTypeProduit,Quantitemin FROM TypeProduit";
    $res= $mysqli->query($quantite);
    $Quantite=$res->fetch_assoc();
    echo nl2br(" Réponse : ");
    echo json_encode($Quantite);
    echo "<br />";

    // Date de Péremption
    $peremption = "SELECT nomProduit,datePeremption FROM Produit ORDER BY datePeremption ASC";
    $resp= $mysqli->query($peremption);

    while($repp=$resp->fetch_assoc()) {
        $Peremption = $repp;
        echo nl2br(" \n Réponse : ");
        echo json_encode($Peremption);
    }
    
}

?>