<?php
	session_start();
	if (!isset($_SESSION["connected"])){
		$_SESSION["connected"] = false;
		$_SESSION["surnom"] = "";
	}

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
	<title>ACCUEIL</title>
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
		<h1 class="titre_ZU">Espace utilisateur</h1>
		<?php afficher_profile($_SESSION["connected"],"index.php"); ?>
		<img src='./AD1.jpg' style='margin:0 auto;width:75%;'/>
	</div>

	<div id="Zone_contenu">
		<h1 class="titre_ZC">Bienvenue à ECE'in !</h1>
		<p>Le réseau social parfait pour s'entretenir entre professionels et étudiants.</p>

		<?php
		if ($_SESSION["connected"] <> true){
			echo('<i id="publication_interdite">Vous devez être connecté en tant que membre pour publier un événement.</i>');
		} else {
			echo('<a href="publication.php?mode=1" id="publication_evenement"><i>Publiez un évènement !</i></a>');
			//echo('<a href="publication.php?mode=2" id="publication_cv"><i>Publiez votre Curriculum Vitae !</i></a>');
			echo('<a href="index.php?mode=1" class="choix_mode_index"><i>Les événements officiels</i></a>');
			echo('<a href="index.php?mode=2" class="choix_mode_index"><i>Vos propres événements</i></a>');
		} ?>

		<?php if( (!isset($_GET["mode"])) or ($_GET["mode"]==1) ): ?>

		<h1 class="titre_ZC">Les événements officiels de la semaine</h1>

		<?php recherche_evenement($CONNEXION,$_SESSION['surnom'],"1"); ?>

		<?php else: ?>

		<h1 class="titre_ZC">Vos propres événements</h1>

		<?php recherche_evenement($CONNEXION,$_SESSION['surnom'],"CONNEXIONS"); ?>

		<?php endif; ?>

		

	</div>

	<div id="Zone_divers">
		<h1 class="titre_ZD">Comment nous contacter ?</h1>
		<p>Nous sommes accessibles de plusieurs façons...</p>
		<p>Par mail : <a href="mailto:antoine.poirier@edu.ece.fr">antoine.poirier@edu.ece.fr</a></p>
		<p>Par téléphone : <i>01 02 33 45 69</i></p>
		<p>Dans nos bureaux localisés en plein coeur de la ville d'Issou</p>
		<iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d2618.3055768571403!2d1.7923265893071667!3d48.98574196202338!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x47e695006398634b%3A0xdf1042ef6b75fa83!2sMurielle%20Coiffure!5e0!3m2!1sfr!2sfr!4v1716668410431!5m2!1sfr!2sfr" style="border:0;width:100%;height:400px;" loading="lazy" referrerpolicy="no-referrer-when-downgrade"></iframe>
	</div>

</div>



</body>

</html>