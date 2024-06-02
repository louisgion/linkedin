<?php 
session_start();

include("toolbox/utilisateurs.inc");
include("toolbox/enregistrement.inc");

$CONNECTION = connection_bdd("ecein");

if(isset($_POST["suppression"])){
	//function inserer_nouveau_membre($connection_bdd,$surnom,$adresse,$rang,$info,$connexions,$photodeprofile="")
	$test = suppression_membre($CONNECTION,$_POST["surnom"]);
	
	if ($test){
		$url_destination=$_POST["destination"];
		$url_travaille = substr($url_destination,0,strlen($url_destination)-1);
		header("location:$url_travaille");
		exit();
	} else {
		die("Il ne s'est rien passé mdr");
	}
	
}

?>
<!DOCTYPE html>
<html>

<head>
	<title>Suppression d'un membre</title>
	<meta charset="utf-8">
	<link rel="stylesheet" href="css/style1.css"/>
	<link rel="shortcut icon" type="image/x-icon" href="logo.jpg" />
</head>

<body>
	

<nav id="barre_navigation">
	<a href="index.php"><img alt="logo_full du site web" src="logo.png"/></a>
	<a href="monreseau.php"><p>Mon réseau</p></a>
	<a href="vous.php"><p>Vous</p></a>
	<a href="notifications.php"><p>Notifications</p></a>
	<a href="messagerie.php"><p>Messagerie</p></a>
	<a href="emplois.php"><p>Emplois</p></a>
</nav>

<div id="enregistrement_connexion">
<!-- Formulaire d'inscription -->
<form class="formulaire1" method="post" action="deletion.php">
	
	<h1 class="text_centre">Suppression d'un membre :</h1>

	<label for="surnom"><p>Surnom :</p></label>
	<input type="text" id="surnom" name="surnom" placeholder="Le surnom ici"/>

	<label for="motdepasse"><p>Adresse ECE :</p></label>
	<input type="password" id="adresse" name="adresse" placeholder="Adresse ECE du membre à supprimer ici"/>

	<label for="motdepasse_confirmation"><p>Confirmation adresse ECE :</p></label>
	<input type="password" id="adresse_confirmation" name="adresse_confirmation" placeholder="Confirmer l'adresse ECE ici"/>

	<input type="hidden" name="destination" value=<?php echo($_GET['origine']); ?>/>

	<input type="submit" id="suppression" name="suppression" value="Suppression"/>

</form>
</div>


</body>

</html>