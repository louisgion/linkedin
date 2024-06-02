<?php
	session_start();

	include("toolbox/enregistrement.inc");
	include("toolbox/utilisateurs.inc");
	include("toolbox/connexion.inc");

	$CONNEXION = connection_bdd("ecein");

	if (isset($_POST['deconnexion'])){
		deconnexion();
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
	
	<div id="Zone_utilisateur">
		<h1 class="titre_ZU">Espace membre</h1>
		<?php afficher_profile($_SESSION["connected"],"monreseau.php"); ?>
	</div>

	<div id="Zone_contenu">
		<h1 class="titre_ZC">Vos connnexions</h1>
		<?php

			if ( ($_SESSION["connected"] == false) or (!isset($_SESSION["connected"])) ){
				echo("<p style='color:red;border:1px red dotted;padding:1px;'> Vous devez vous authentifier en tant que membre pour avoir accès aux fonctionnalités de cette page. </p>");
			} else {
				lire_connexions($CONNEXION,$_SESSION["surnom"]);
			}

		?>
	</div>

	<div id="Zone_divers">
		<h1 class="titre_ZD">Connectez-vous !</h1>

		<?php if( $_SESSION["connected"] == true ): ?>

		<form method="post" action="monreseau.php" class="formulaire1">
			<?php 

			if (isset($_POST["connexion"])){
				ajouter_connexion($CONNEXION,$_SESSION["surnom"],$_POST["surnom_connexion"]);
			}

			?>
			<p>Nom de la nouvelle connexion :</p>
			<input type="text" name="surnom_connexion" placeholder="nouvelle connexion..."/>
			<input type="submit" name="connexion" value="Connecter"/>
		</form>

	<?php else: ?>

		<p style='color:red;border:1px red dotted;padding:1px;'> Vous devez vous authentifier en tant que membre pour avoir accès aux fonctionnalités de cette page. </p>

	<?php endif; ?>

	</div>

</div>

</body>
<?php mysqli_close($CONNEXION); ?>

</html>