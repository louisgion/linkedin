<?php
	session_start();

	include("toolbox/enregistrement.inc");
	include("toolbox/utilisateurs.inc");
	include("toolbox/gestion_publications_evenements.inc");

	if (isset($_POST['deconnexion'])){
		deconnexion();
	}

	$CONNEXION = connection_bdd("ecein");

?>

<!DOCTYPE html>
<html>

<head>
	<title>LES NOTIFICATIONS</title>
	<meta charset="utf-8">
	<link rel="stylesheet" href="./css/style1.css"/>
	<link rel="shortcut icon" type="image/x-icon" href="logo.jpg" />
</head>

<script type="text/javascript" src="toolbox/diaporama.js"></script>

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
		<?php afficher_profile($_SESSION["connected"],"notifications.php"); ?>
		<img src='./AD1.jpg' style='margin:0 auto;width:75%;'/>
	</div>

	<div id="Zone_contenu">
		<h1 class="titre_ZC">Les notifications</h1>
		<?php  

			if ( isset($_POST["recherche"]) ){

				// Si l'utilisateur est connecté en tant que membre
				// il peut choisir de ne voir que les notifications
				// de son réseau. Autrement il est contraint de voir
				// toutes les notifications de l'école.
				recherche_notifications_avancee($_POST["categorie"],$_POST["ordre"],$_POST["restriction"],$_SESSION["connected"],$CONNEXION,$_SESSION["surnom"]);
				
			
			} else {



			}

		?>
	</div>


	<div id="Zone_divers">
		<h1 class="titre_ZD">Critères de recherche</h1>
		<form id="formulaire_notif_recherche" method="post" action="notifications.php">
			<label for="categorie"><p>Type de notification :</p></label>
			<select name="categorie[]" id="categorie" multiple>
				<option value="0" selected><p>Annonce de membres</p></option>
				<option value="1"><p>Annonces officielles</p></option>
				<option value="3"><p>Postulation</p></option>
			</select>
			<p>Ordre d'apparition</p>
			<ul>
			<li><input type="radio" name="ordre" value="croissant"/>Ordre croissant de date</li>
			<li><input type="radio" name="ordre" value="decroissant" checked />Ordre décroissant de date</li>
			</ul>
			<p>Filtrage des notifications</p>
			<ul>
			<li><input type="radio" name="restriction" value="seulement"/>Uniquement celles de vos connexions</li>
			<li><input type="radio" name="restriction" value="passeulement" checked/>Toutes les notifications de l'école</li>
			</ul>
			<input type="submit" name="recherche" value="Rechercher"/>
		</form>
	</div>

</div>


</body>

<?php mysqli_close($CONNEXION); ?>

</html>