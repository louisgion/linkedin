<?php 
session_start();
if (!isset($_GET["origine"])){
	header("location:index.php");
}

include("toolbox/enregistrement.inc");

$CONNECTION = connection_bdd("ecein");

if(isset($_POST["inscription"])){
	//function inserer_nouveau_membre($connection_bdd,$surnom,$adresse,$rang,$info,$connexions,$photodeprofile="")
	$test = inserer_nouveau_membre($CONNECTION,$_POST["surnom"],$_POST["adresse"],$_POST["rang"],$_POST["info"],"",$_FILES["photodeprofile"]);
	if ($test){
		// Enregistrement réussi, l'utilisateur est automatiquement connecté ...
		$_SESSION["surnom"] = $_POST["surnom"];
		$_SESSION["connected"] = true;
		$url_destination=$_POST["destination"];
		$url_travaille = substr($url_destination,0,strlen($url_destination)-1);
		header("location:$url_travaille");
		exit();
	}
}

?>
<!DOCTYPE html>
<html>

<head>
	<title>REGISTER</title>
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
<form class="formulaire1" method="post" action="register.php" enctype="multipart/form-data">
	
	<h1 class="text_centre">Inscription</h1>

	<label for="surnom"><p>Surnom :</p></label>
	<input type="text" id="surnom" name="surnom" placeholder="Ton surnom ici"/>

	<label for="motdepasse"><p>Adresse ECE :</p></label>
	<input type="password" id="adresse" name="adresse" placeholder="Adresse ECE du nouveau membre ici"/>

	<label for="motdepasse_confirmation"><p>Confirmation adresse ECE :</p></label>
	<input type="password" id="adresse_confirmation" name="adresse_confirmation" placeholder="Confirmer l'adresse ECE ici"/>

	<label for="photodeprofile"><p>Image de profile (facultative) :</p></label>
	<input type="file" id="photodeprofile" name="photodeprofile"/>

	<label for="description" id="info"><p>Description (facultative) :</p></label>
	<textarea id="info" name="info" rows="10"></textarea>

	<p>Rang du nouveau membre :</p>
	<label for="membre"><input id="membre" type="radio" name="rang" value="membre" checked /><p>Membre</p></label>
	<label for="admin"><input id="admin"* type="radio" name="rang" value="administrateur"/><p>Administrateur</p></label>

	<input type="hidden" name="destination" value=<?php echo($_GET['origine']); ?>/>

	<input type="submit" id="inscription" name="inscription" value="Inscription"/>

</form>
</div>


</body>

</html>