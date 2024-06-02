<?php

session_start();

if (!isset($_GET["origine"])){
	header("location:index.php");
}

include("toolbox/enregistrement.inc");

$CONNEXION = connection_bdd("ecein");

if(isset($_POST["connexion"])){
	$test = connection($CONNEXION,$_POST["surnom_connex"],$_POST["motdepasse_connex"]);
	if ($test){
		// Connexion réussie...
		$_SESSION["surnom"] = $_POST["surnom_connex"];
		$_SESSION["connected"] = true;
		//$url_destination="stuff2.php";
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
	<title>LOGIN</title>
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

<!-- Formulaire de connexion -->
<div id="enregistrement_connexion">
<form class="formulaire2" method="post" action="login.php">
	
	<h1 class="text_centre">Connexion</h1>

	<label for="surnom"><p>Surnom :</p></label>
	<input type="text" id="surnom" name="surnom_connex" placeholder="Ton surnom ici"/>

	<label for="motdepasse"><p>Mot de passe :</p></label>
	<input type="password" id="motdepasse" name="motdepasse_connex" placeholder="Ton mot de passe ici"/>

	<input type="hidden" name="destination" value=<?php echo($_GET['origine']); ?>/>

	<input type="submit" id="connexion" name="connexion" value="Connexion"/>

</form>
</div>


</body>

</html>