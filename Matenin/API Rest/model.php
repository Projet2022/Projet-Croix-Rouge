<?php

function ConnectDB() {
	$ini=parse_ini_file('');
	$id = new PDO("mysql:host=".$ini['host'].";dbname=".$ini['database'].";charset=utf8", $ini['user'], $ini['password']);
	$id->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	return $id;
}

?>