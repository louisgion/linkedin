<?php
	session_start();

	include("toolbox/enregistrement.inc");
	include("toolbox/utilisateurs.inc");
	include("toolbox/messagerie.inc");

	$CONNEXION = connection_bdd("ecein");

	if (isset($_POST['deconnexion'])){
		deconnexion();
	}
?>

<!DOCTYPE html>
<html>

<head>
	<title>MA MESSAGERIE</title>
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
		<?php afficher_profile($_SESSION["connected"],"messagerie.php"); ?>
		<img src='./AD1.jpg' style='margin:0 auto;width:75%;'/>
	</div>



	<div id="Zone_contenu">

		<?php if( ($_SESSION["connected"] == false) or (!isset($_SESSION["connected"]))  ): ?>

		<p style='color:red;border:1px red dotted;padding:1px;'> Vous devez vous authentifier en tant que membre pour avoir accès aux fonctionnalités de cette page.</p>

		<?php else: ?>

		<p id="envoyeur"><?php echo($_SESSION["surnom"]); ?></p>
		<div id="messagerie_zone">
		<article id="affichage_message"></article>
		<textarea id="texte_messagerie" rows="10"></textarea>	
		<button id="bouton_envoyer_message" onclick="">Envoyer (Enter)</button>
		</div>

		<?php endif; ?>
		


	</div>



	<div id="Zone_divers">

		<?php if( ($_SESSION["connected"] == false) or (!isset($_SESSION["connected"]))  ): ?>

		<p style='color:red;border:1px red dotted;padding:1px;'> Vous devez vous authentifier en tant que membre pour avoir accès aux fonctionnalités de cette page. </p>

		<?php else: ?>

			<p>Parler avec qui ?</p>

			<div id="liste_discussions">
			<article class='disci' id='1' onclick='changer_disc(1)'> GENERAL </article>
			<?php  lire_discussions($CONNEXION,$_SESSION["surnom"]); ?>
			</div>

		<?php endif; ?>

	</div>

</div>

<script>

let discussion_choisie;
const ws = new WebSocket("ws://localhost:4567");
const afficheur = document.getElementById("affichage_message");
const discussions = document.getElementsByClassName("disci");

ws.addEventListener("open", () =>{
	
});

ws.addEventListener('message', function (event) {
	console.log(event.data);
	afficher_message(event.data);
});

const bouton_envoyer = document.getElementById("bouton_envoyer_message");

bouton_envoyer.addEventListener("click",function(event){
	let surnom_envoyeur = document.getElementById("envoyeur").innerText;
	let message = document.getElementById("texte_messagerie").value;
	let message_final = surnom_envoyeur + " > " + message;
	ws.send(message_final);
	document.getElementById("texte_messagerie").value = "";
});

// Sous-fonction pour mettre un message dans le conteneur affichage_message
function afficher_message(contenu){

	let IDD = contenu.split(":");
	console.log(IDD);
	id_discussion_associee = IDD[0];

	if ( id_discussion_associee == discussion_choisie ){

		// On rafraichit le contenu de l'afficheur...
		while (afficheur.firstChild) {
	    	afficheur.firstChild.remove();
		}

		// On sépare le contenu dans un tableau
		let array_message = contenu.split("$");

		// On affiche les messages un par un...
		for ( let i = 0 ; i < array_message.length ; i++ ){
			let paragraphe = document.createElement("p");
			paragraphe.innerText = array_message[i];
			document.getElementById("affichage_message").appendChild(paragraphe);
		}


	}

	
}

function changer_disc(id_disc){
	
	discussion_choisie = id_disc;

	// On rafraichit le contenu de l'afficheur...
	while (afficheur.firstChild) {
	    afficheur.firstChild.remove();
	}

	for ( let i = 0 ; i < discussions.length ; i++ ){
		if ( discussions[i].id == id_disc ){
			discussions[i].style.backgroundColor = "#fffa65";
		} else {
			discussions[i].style.backgroundColor = "white";
		}
	}

	document.getElementById("texte_messagerie").value = "";
	ws.send(`!:${id_disc}`); 
}

</script>



</body>

</html>