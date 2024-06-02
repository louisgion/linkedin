<?php
	session_start();

	include("toolbox/enregistrement.inc");
	include("toolbox/utilisateurs.inc");
	include("toolbox/emplois.inc");

	if (isset($_POST['deconnexion'])){
		deconnexion();
	}

	$CONNEXION = connection_bdd("ecein");

	if (isset($_POST['Postuler'])){
		//echo("ON POSTULE AHHHHHH§!!!!");
		informer_postulation($CONNEXION,$_SESSION["surnom"],$_POST["emploi_id"]);
	}

?>

<!DOCTYPE html>
<html>

<head>
	<title>LES EMPLOIS</title>
	<meta charset="utf-8">
	<link rel="stylesheet" href="./css/style1.css"/>
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

<div id="Navigation_horizontale">
	
	<div id="Zone_utilisateur">
		<h1 class="titre_ZU">Espace membre</h1>
		<?php afficher_profile($_SESSION["connected"],"emplois.php"); ?>
	</div>

	<div id="Zone_contenu">
		<h1 class="titre_ZC">Tous les emplois en quelques clics !</h1>
		<?php 
		lire_emplois($CONNEXION,$_SESSION["connected"]); 
		?>
	</div>

</div>



</body>

</html>