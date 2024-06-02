<?php

// Sous-fonction pour rechercher un utilisateur
function recherche_membre_special1($connection_bdd,$surnom=""){
	$lecture_requete = "SELECT * FROM membres WHERE surnom = ?";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		die("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_utilisateur : préparation de requête échouée </p>");
	}else{
		//echo("<p style='color:green;border:1px green dotted;padding:1px;'> recherhce_utilisateur : préparation de requête réussie </p>");
		mysqli_stmt_bind_param($preparation,"s",$surnom);
		$id; $sur; $adr; $rank; $info; $connex; $pfp;
		mysqli_stmt_bind_result($preparation,$id,$sur,$adr,$rank,$info,$connex,$pfp);
		$resultat = mysqli_stmt_execute($preparation);
		while(mysqli_stmt_fetch($preparation)){
			afficher_profil_membre($sur,$info,$pfp);
		}
		mysqli_stmt_close($preparation);
	}
} 

// Sous-fonction pour afficher une connexion...
function afficher_profil_membre($surnom,$info,$image){
	echo("<article class='profile'>
		<a href='profil.php?surnom_cible=$surnom'><img src=$image /></a>
		<div>
		<h2 style='color:blue;'>$surnom</h2>
		<p>$info</p>
		</div>
		</article>");
}

// Sous-fonction pour lire les connexions d'un membre...
function lire_connexions($connection_bdd,$surnom_cible){
	$requete1 = "SELECT connexions FROM membres WHERE surnom='$surnom_cible';";
	$resultat1 = mysqli_query($connection_bdd,$requete1);
	$lecture = mysqli_fetch_row($resultat1);
	$liste_connexions = $lecture[0];
	$connexions_elements = explode("+",$liste_connexions);
	foreach($connexions_elements as $cle => $valeur){
		if ($valeur <> ""){
			recherche_membre_special1($connection_bdd,$valeur);
		}
	}
}

// Sous-fonction pour ajouter une nouvelle connexion...
function ajouter_connexion($connection_bdd,$surnom_envoyeur,$surnom_destinataire){
	$requete = "SELECT * FROM membres WHERE surnom='$surnom_destinataire';";
	$resultat = mysqli_query($connection_bdd,$requete);
	if (!$resultat){
		echo("<p style='color:red;border:1px red dotted;padding:1px;'> ajouter_connexion : Exécution de requête échouée </p>");
		return false;
	}
	//echo(mysqli_num_rows($resultat));
	if (mysqli_num_rows($resultat) == 0){
		// S'active quand aucun utilisateur a le surnom donné en paramètre.
		echo("<p style='color:red;border:1px red dotted;padding:1px;'> ajouter_connexion : Le membre $surnom_destinataire n'existe pas ! </p>");
		return false;
	} else if (mysqli_num_rows($resultat) > 1) {
		// S'active quand le surnom donné en paramètre fait référence à plus d'un utilisateur. ( ERREUR COTE DEV ).
		echo("<p style='color:red;border:1px red dotted;padding:1px;'> ajouter_connexion : Le surnom fait référence à plus d'un membre (ERREUR DEV) ! </p>");
		return false;
	} else {

		echo("<p style='color:green;border:1px green dotted;padding:1px;'> ajouter_connexion : Membre trouvé ! </p>");
		$requete1 = "SELECT connexions FROM membres WHERE surnom='$surnom_envoyeur';";
		$resultat1 = mysqli_query($connection_bdd,$requete1);
		$lecture = mysqli_fetch_row($resultat1);
		$liste_connexions = $lecture[0];
		$connexions_elements = explode("+",$liste_connexions);
		//var_dump($connexions_elements);
		
		foreach($connexions_elements as $cle => $valeur){
			if ( $valeur == $surnom_destinataire ){
				echo("<p style='color:red;border:1px red dotted;padding:1px;'> ajouter_connexion : Vous êtes déjà connecté avec $surnom_destinataire ! </p>");
				return false;
			}
		}

		$liste_connexions .= "+";
		$liste_connexions .= $surnom_destinataire;

		$requete2 = "UPDATE membres SET connexions = '$liste_connexions' WHERE surnom='$surnom_envoyeur';";
		$resultat2 = mysqli_query($connection_bdd,$requete2);
		//var_dump($liste_connexions);



		$requete2 = "SELECT connexions FROM membres WHERE surnom='$surnom_destinataire';";
		$resultat2 = mysqli_query($connection_bdd,$requete2);
		$lecture2 = mysqli_fetch_row($resultat2);
		$liste_connexions2 = $lecture2[0];
		$connexions_elements2 = explode("+",$liste_connexions2);
		//var_dump($connexions_elements);
		
		foreach($connexions_elements2 as $cle => $valeur){
			if ( $valeur == $surnom_envoyeur ){
				echo("<p style='color:orange;border:1px orange dotted;padding:1px;'> ajouter_connexion : $surnom_destinataire vous a déjà en tant que connexion ??? (DEV) </p>");
			}
		}

		$liste_connexions2 .= "+";
		$liste_connexions2 .= $surnom_envoyeur;

		$requete3 = "UPDATE membres SET connexions = '$liste_connexions2' WHERE surnom='$surnom_destinataire';";
		$resultat3 = mysqli_query($connection_bdd,$requete3);
		//var_dump($liste_connexions);

		ajouter_messagerie($connection_bdd,$surnom_envoyeur,$surnom_destinataire);

		return true;
	}
	return false;
}



function ajouter_messagerie($connection_bdd,$personne1,$personne2){



	$lecture_requete = "INSERT INTO messageries (personne1,discussion,personne2) VALUES (?,'',?)";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		die("<p style='color:red;border:1px red dotted;padding:1px;'> lire_discussions : préparation de requête échouée </p>");
	}else{
		//echo("<p style='color:green;border:1px green dotted;padding:1px;'> lire_discussions : préparation de requête réussie </p>");
		mysqli_stmt_bind_param($preparation,"ss",$personne1,$personne2);
		$resultat = mysqli_stmt_execute($preparation);
		mysqli_stmt_close($preparation);
	}
}



?>