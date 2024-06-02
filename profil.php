<?php
	session_start();

	include("toolbox/enregistrement.inc");
	include("toolbox/utilisateurs.inc");
	include("toolbox/connexion.inc");
	include("toolbox/gestion_publications_evenements.inc");

	$CONNEXION = connection_bdd("ecein");

	if (isset($_POST['deconnexion'])){
		deconnexion();
	}

	if( (!isset($_GET['surnom_cible'])) or (empty($_GET['surnom_cible'])) ){
		header("location:index.php");
	}

?>

<!DOCTYPE html>
<html>

<head>
	<title>PROFIL DE <?php echo($_GET["surnom_cible"]); ?></title>
	<meta charset="utf-8">
	<link rel="stylesheet" href="./css/style1.css"/>
	<link rel="shortcut icon" type="image/x-icon" href="logo.jpg" />
</head>

<script type="text/javascript" src="diaporama.js"></script>

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
		<?php afficher_profile($_SESSION["connected"],"monreseau.php"); ?>
	</div>

	<div id="Zone_contenu">
		<h1 class="titre_ZC">Profil de <?php echo($_GET["surnom_cible"]); ?></h1>
		<?php

			if ( ($_SESSION["connected"] == false) or (!isset($_SESSION["connected"])) ){
				echo("<p style='color:red;border:1px red dotted;padding:1px;'> Vous devez vous authentifier en tant que membre pour avoir accès aux fonctionnalités de cette page. </p>");
			} else {
				recherche_membre_special1($CONNEXION,$_GET["surnom_cible"]);
			}

		?>
		<h1 class="titre_ZC">Publications récentes de <?php echo($_GET["surnom_cible"]); ?></h1>
		<?php lire_evenements_surnom($CONNEXION,$_GET["surnom_cible"],$_SESSION["surnom"]); ?>
	</div>

</div>

</body>
<?php mysqli_close($CONNEXION); ?>

</html>