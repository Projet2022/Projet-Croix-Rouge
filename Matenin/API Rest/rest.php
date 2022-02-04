<?php

// Connexion Base de données
require ('./model.php');
$id = ConnectDB();

// Requete
$req_type = $_SERVER["REQUEST_METHOD"];
$req_path = $_SERVER["PATH_INFO"];
$req_data = explode("/", $req_path);

// Affichage Quantite minimale d'article atteinte
if($req_type === "GET"){
	echo "GET";
	if(isset($req_data[1]) && $req_data[1] === "") 
		{
			$requeteSQL = "SELECT nomCategorie,quantite_min FROM TypeProduit";
        }
    echo $requeteSQL;
}

// Affichage Produit a date de peremption depassee
if($req_type === "GET"){
	echo "GET";
	if(isset($req_data[1]) && $req_data[1] === "") 
		{
			$requeteSQL = "SELECT datePeremption FROM Produit ORDER BY datePeremption";
        }
    echo $requeteSQL;
}

?>