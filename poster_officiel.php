<?php
	session_start();

	include("toolbox/enregistrement.inc");
	include("toolbox/utilisateurs.inc");
	include("toolbox/gestion_publications_evenements.inc");

	$CONNECTION = connection_bdd("ecein");

	if (isset($_POST["publication"])){
		$cat = 1;
		$test = inserer_nouvel_evenement($CONNECTION,$_POST["titre"],$cat,$_POST["texte"],$_SESSION["surnom"],$_POST["prive"],$_FILES);
		if ($test){
			header("location:index.php");
		}
	}

?>

<!DOCTYPE html>
<html>

<head>
	<title>MON RESEAU</title>
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

<form class="formulaire1" method="post" action="poster_officiel.php" enctype="multipart/form-data">
	
	<h1 class="text_centre">Publiez un événement officiel !</h1>

	<?php

	if (isset($_POST["publication"])){
		$tableau = verifier_images($CONNECTION,$_FILES,1000000);
		foreach($tableau as $cle => $valeur){
			echo("<p>$cle => $valeur</p><br/>");
		}
	}

	?>

	<!-- Le titre de l'événement -->
	<label for="titre"><p>Titre de l'événement :</p></label>
	<input type="text" id="titre" name="titre" placeholder="Titre de l'événement..."/>

	<label for="diaporama"><p>Images (facultatives) :</p></label>
	<input type="file" id="diaporama" name="diaporama[]" multiple/>

	<label for="texte"><p>Texte (facultatif) :</p></label>
	<textarea name="texte" rows="20"></textarea>

	<ul>
	<li><input type="radio" name="prive" value='0' checked/><p>Accesible à tout le monde</p></li>
	<li><input type="radio" name="prive" value='1'/><p>Seulement pour moi et mes connexions</p></li>
	</ul>

	<input type="submit" id="publication" name="publication" value="Publier"/>

</form>

</div>

</body>

<?php mysqli_close($CONNECTION); ?>

</html>