<?php 
session_start();

include("toolbox/enregistrement.inc");
include("toolbox/emplois.inc");

$CONNEXION = connection_bdd("ecein");

if(isset($_POST["creation"])){
	//function inserer_emploi($intitule,$nature,$remuneration,$entreprise,$localisation)
	$test = inserer_emploi($CONNEXION,$_POST["intitule"],$_POST["nature"],$_POST["remuneration"],$_POST["entreprise"],$_POST["localisation"]);
	if ($test){
		header("location:emplois.php");
		exit();
	}
}

?>
<!DOCTYPE html>
<html>

<head>
	<title>NOUVEAU EMPLOI</title>
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
<form class="formulaire1" method="post" action="nouveau_emploi.php">
	
	<h1 class="text_centre">Créez un nouveau emploi !</h1>

	<label for="intitule"><p>Intitulé de l'emploi :</p></label>
	<input type="text" id="intitule" name="intitule" placeholder="Intitulé de l'emploi..."/>

	<label for="nature"><p>Nature de l'emploi :</p></label>
	<select id="nature" name="nature">
		<option selected value="Stage"><p>Stage</p></option>
		<option value="Apprentissage"><p>Apprentissage</p></option>
		<option value="Emploi temporaire"><p>Emploi temporaire</p></option>
		<option value="Emploi permanent"><p>Emploi permanent</p></option>
	</select>

	<label for="remuneration"><p>Rémunération mensuelle attendue :</p></label>
	<input type="number" min="0" id="remuneration" name="remuneration"/>

	<label for="entreprise"><p>Entreprise :</p></label>
	<select id="entreprise" name="entreprise">
		<option selected value="Orange"><p>Orange</p></option>
		<option value="Capgmeni"><p>Capgemini</p></option>
		<option value="SFR"><p>SFR</p></option>
		<option value="Société Générale"><p>Société Générale</p></option>
		<option value="Safran"><p>Safran</p></option>
		<option value="TF1"><p>TF1</p></option>
		<option value="Canal+"><p>Canal+</p></option>
		<option value="OMNES Education"><p>OMNES Education</p></option>
	</select>

	<label for="localisation"><p>Localisation :</p></label>
	<select id="localisation" name="localisation">
		<option selected value="Paris"><p>Paris, France</p></option>
		<option value="Bruxelles"><p>Bruxelles, France</p></option>
		<option value="Toulouse"><p>Toulouse, France</p></option>
		<option value="Londres"><p>Londres, Royaume-Uni</p></option>
		<option value="New York"><p>New York, USA</p></option>
		<option value="Chambery"><p>Chambéry, France</p></option>
	</select>

	<input type="submit" id="creation" name="creation" value="Creation"/>

</form>
</div>


</body>

</html>