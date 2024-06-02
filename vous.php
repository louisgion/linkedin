<?php
	session_start();

	include("toolbox/enregistrement.inc");
	include("toolbox/utilisateurs.inc");
	include("toolbox/vous.inc");
	include("toolbox/gestion_publications_evenements.inc");


	$CONNEXION = connection_bdd("ecein");

	if (isset($_POST['deconnexion'])){
		deconnexion();
	}
?>

<!DOCTYPE html>
<html>

<head>
	<title>VOUS</title>
	<meta charset="utf-8">
	<link rel="stylesheet" href="./css/style1.css"/>
	<link rel="shortcut icon" type="image/x-icon" href="logo.jpg" />
</head>

<script type="text/javascript" src="toolbox/diaporama.js"></script>
<script type="text/javascript" src="toolbox/generation_cv.js"></script>

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
		<?php afficher_profile($_SESSION["connected"],"vous.php"); ?>
		<img src='./AD1.jpg' style='margin:0 auto;width:75%;'/>
	</div>

	<div id="Zone_contenu">
		<?php

		if (isset($_POST["enlever"])){
			enlever_evenement_id($CONNEXION,$_POST["id_cible"]);
		}

		if ( isset($_POST["upvote"]) or isset($_POST["downvote"]) ){
			like_ou_dislike($CONNEXION,$_POST["id_cible"]);
		}

		if (isset($_POST['filtrer'])){

			//echo("<p>FILTRER : ".$_POST['filtrer']."</p><br/>");
			//echo("<p>ID_CIBLE : ".$_POST['id_cible']."</p>");
			
			filtrage_evenement($CONNEXION,$_POST["id_cible"],(int)$_POST["filtrer"]);
		}

		if( ($_SESSION["connected"] == false) or (!isset($_SESSION["connected"])) ){

			echo("<p style='color:red;border:1px red dotted;padding:1px;'> Vous devez vous authentifier en tant que membre pour avoir accès aux fonctionnalités de cette page. </p>");

		} else {

			echo('<h1 class="titre_ZC">Votre profil</h1>');

			lire_membre($CONNEXION,$_SESSION["surnom"]);

			echo('<h1 class="titre_ZC">Vos propres événements</h1>');

			recherche_evenement_session($CONNEXION,$_SESSION["surnom"]);

		}



		?>
	</div>

	<div id="Zone_divers">

		<h1 class="titre_ZD">Génération de CV</h1>

		<?php if( ($_SESSION["connected"] == false) or (!isset($_SESSION["connected"])) ): ?>

			<p style='color:red;border:1px red dotted;padding:1px;'> Vous devez vous authentifier en tant que membre pour avoir accès aux fonctionnalités de cette page. </p>

		<?php else: ?>

			<button onclick="generateCV()">Générer mon CV</button>

	    	<div id="cv-output" style="margin-top: 20px;">
	        	<!-- Le CV généré apparaîtra ici -->
	    	</div>

		<?php endif; ?>

	</div>

</div>



</body>

<?php mysqli_close($CONNEXION); ?>
</html>