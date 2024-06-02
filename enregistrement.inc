
<?php

// Sous-fonction pour vérifier une photo de profile et le mettre dans le dossier 'photo_de_profile'
// si toutes les conditions sont respectées...
function traiter_photo_de_profile($image,$taillemaximale,$surnom){
	// On vérifie d'abord si l'image existe...
	if (isset($image)){

		// Affichage des informations sur le fichier envoyé (debug)...
		echo("<p style='color:blue;border:1px blue dotted;padding:1px;'> Photo de profile détectée </p>");
		foreach($image as $cle => $valeur){
			echo("<p style='color:blue;border:1px blue dotted;padding:1px;'> /// $cle : $valeur </p>");
		}

		// On vérifie si l'extension du fichier donnée est valide...
		$extensions_autorises = array("image/png","image/jpeg","image/jpg","image/gif");
		if ( in_array($image["type"],$extensions_autorises) <> true ){
			die("<p style='color:red;border:1px red dotted;padding:1px;'> photo de profile : L'extension du fichier est invalide </p>");
		}

		// On vérifie si la taille du fichier ne dépasse pas la limite imposée...
		$taille_maximale = $taillemaximale;
		if ( $image["size"] > $taille_maximale ){
			die("<p style='color:red;border:1px red dotted;padding:1px;'> photo de profile : La taille du fichier est trop grande </p>");
		}

		// On modifie le nom du fichier pour qu'il soit identique au surnom du nouveau utilisateur préfixé par "pfp_"
		$prefixe = "pfp_";
		$nouveau_titre;
		switch($_FILES["photodeprofile"]["type"]){
			case "image/jpg":
				$nouveau_titre = $prefixe . $surnom . ".jpg";
				break;
			case "image/jpeg":
				$nouveau_titre = $prefixe . $surnom . ".jpeg";
				break;
			case "image/png":
				$nouveau_titre = $prefixe . $surnom . ".png";
				break;
			case "image/gif":
				$nouveau_titre = $prefixe . $surnom . ".gif";
				break;
		}
		echo("<p style='color:blue;border:1px blue dotted;padding:1px;'> /// Nouveau nom du fichier : $nouveau_titre </p>"); 

		// Si tout est bon, on copie le fichier dans le dossier 'photo_de_profile'...
		if(copy($_FILES["photodeprofile"]["tmp_name"],"photo_de_profile/".$nouveau_titre) <> true){
			die("<p style='color:red;border:1px red dotted;padding:1px;'> photo de profile : Le fichier a effectué toutes les vérifications obligatoires MAIS n'arrive pas à être copié (DEV) </p>");
		} else {
			echo("<p style='color:green;border:1px green dotted;padding:1px;'> photo de profile : Enregistrement de la photo de profile réussie ! </p>");
			return "photo_de_profile/".$nouveau_titre;
		}
		
	}
}



// Sous-fonction pour se connecter à une base de donnée
// prend en paramètre le nom de la base de donnée ciblée
// pour la connection...
function connection_bdd($nom_bdd){
	$connection = mysqli_connect("localhost","root","",$nom_bdd);
	if ( !$connection ){
		die("<p style='color:red;border:1px red dotted;padding:1px;'>Connection à la BDD ''$nom_bdd'' échouée : ".mysqli_connect_error()."</p>");
	} else {
		//echo("<p style='color:green;border:1px green dotted;padding:1px;'>Connection à la BDD ''$nom_bdd'' réussie </p>");
		return $connection;
	}
}

// Sous-fonction pour rechercher un utilisateur
function recherche_membre($connection_bdd,$surnom=""){
	$lecture_requete = "SELECT * FROM membres WHERE surnom = ?";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		die("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_utilisateur : préparation de requête échouée </p>");
	}else{
		echo("<p style='color:green;border:1px green dotted;padding:1px;'> recherhce_utilisateur : préparation de requête réussie </p>");
		mysqli_stmt_bind_param($preparation,"s",$surnom);
		$id; $sur; $adr; $rank; $info; $connex; $pfp;
		mysqli_stmt_bind_result($preparation,$id,$sur,$adr,$rank,$info,$connex,$pfp);
		$resultat = mysqli_stmt_execute($preparation);
		$nombre = 0;
		while(mysqli_stmt_fetch($preparation)){
			//echo("<p>ID : $id // SURNOM : $sur // MOTDEPASSE : $mdp // DESCRIPTION : $desc // PHOTODEPROFILE : $pfp</p><br/>");
			$nombre++;
		}
		echo("<p>$nombre membres(s) trouvé(s)</p>");
		mysqli_stmt_close($preparation);
	}
}

// Sous-fonction pour indiquer le nombre d'utilisateur avec le surnom indiqué en paramètre...
function recherche_nombre_membre($connection_bdd,$surnom=""){
	$lecture_requete = "SELECT * FROM membres WHERE surnom = ?";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		die("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_nombre_utilisateur : préparation de requête échouée </p>");
	}else{
		echo("<p style='color:green;border:1px green dotted;padding:1px;'> recherche_nombre_utilisateur : préparation de requête réussie </p>");
		mysqli_stmt_bind_param($preparation,"s",$surnom);
		$id; $sur; $addr; $rang; $info; $connex; $pfp;
		mysqli_stmt_bind_result($preparation,$id,$sur,$addr,$rang,$info,$connex,$pfp);
		$resultat = mysqli_stmt_execute($preparation);
		$nombre = 0;
		while(mysqli_stmt_fetch($preparation)){
			$nombre++;
		}
		mysqli_stmt_close($preparation);
		return $nombre;
	}
}

// Sous-fonction pour insére un nouveau utilisateur dans la table
// 'utilisateurs' de la base de donnée 'the_big_one'...
function inserer_nouveau_membre($connection_bdd,$surnom,$adresse,$rang,$info,$connexions,$photodeprofile=""){
	$pdp_defaut = "photo_de_profile/defaut.jpg";
	$requete = "INSERT INTO membres (surnom,adresse,rang,info,connexions,photodeprofile) VALUES (?,?,?,?,?,?)";
	$preparation = mysqli_prepare($connection_bdd,$requete);
	if (!$preparation){

		// MESSAGE D'ECHEC
		die("<p style='color:red;border:1px red dotted;padding:1px;'> inserer_nouveau_membre : Préparation de requête échouée </p>");

	} else {

		// MESSAGE DE REUSSITE
		echo("<p style='color:green;border:1px green dotted;padding:1px;'> inserer_nouveau_membre : Préparation de requête réussie </p>");

		// VERIFICATIONS NECESSAIRES...
		// Si un utilisateur utilise déjà le surnom donné en paramètre, on ne commence pas l'inscription...
		if ( recherche_nombre_membre($connection_bdd,$surnom) <> 0 ){
			die("<p style='color:red;border:1px red dotted;padding:1px;'> inserer_nouveau_membre : Un membre utilise déjà le surnom $surnom </p>");
		}

		if (empty($photodeprofile["name"])){

		// ------------------------------------------------------------------------------------------------

		// PROCESSUS D'INSCRIPTION EN COURS
		mysqli_stmt_bind_param($preparation,"ssssss",$SUR,$ADR,$RANK,$INFO,$CONNEX,$PFP);
		$SUR = $surnom;
		$ADR = $adresse;
		$RANK = $rang;
		$INFO = $info;
		$CONNEX = $connexions;
		$PFP = $pdp_defaut;
		mysqli_stmt_execute($preparation);
		// FIN PROCESSUS D'INSCRIPTION

		// ------------------------------------------------------------------------------------------------

		} else {

		// ------------------------------------------------------------------------------------------------

		// ENREGISTREMENT DE LA NOUVELLE PHOTO DE PROFILE...
		$nom_fichier = traiter_photo_de_profile($photodeprofile,10000000,$surnom);


		// PROCESSUS D'INSCRIPTION EN COURS
		mysqli_stmt_bind_param($preparation,"ssssss",$SUR,$ADR,$RANK,$INFO,$CONNEX,$PFP);
		$SUR = $surnom;
		$ADR = $adresse;
		$RANK = $rang;
		$INFO = $info;
		$CONNEX = $connexions;
		$PFP = $nom_fichier;
		mysqli_stmt_execute($preparation);
		// FIN PROCESSUS D'INSCRIPTION

		// ------------------------------------------------------------------------------------------------

		}

	}
	mysqli_stmt_close($preparation);
	return true;
}

// Sous-fonction pour se connecter à un compte
function connection($connection_bdd,$surnom_saisi,$adresse_saisi){
	$requete = "SELECT adresse FROM membres WHERE surnom='$surnom_saisi';";
	$resultat = mysqli_query($connection_bdd,$requete);
	if (!$resultat){
		die("<p style='color:red;border:1px red dotted;padding:1px;'> connection : Exécution de requête échouée </p>");
	}
	//echo(mysqli_num_rows($resultat));
	if (mysqli_num_rows($resultat) == 0){
		// S'active quand aucun utilisateur a le surnom donné en paramètre.
		die("<p style='color:red;border:1px red dotted;padding:1px;'> connection : Le membre $surnom_saisi n'existe pas ! </p>");
	} else if (mysqli_num_rows($resultat) > 1) {
		// S'active quand le surnom donné en paramètre fait référence à plus d'un utilisateur. ( ERREUR COTE DEV ).
		die("<p style='color:red;border:1px red dotted;padding:1px;'> connection : Le surnom fait référence à plus d'un membre (ERREUR DEV) ! </p>");
	} else {
		// S'active quand le surnom n'appartient qu'à un seul utilisateur (BON SCENARIO).
		echo("<p style='color:blue;border:1px blue dotted;padding:1px;'> connection : Connection en cours... </p>");
		// On compare le mot de passe extrait de la table à celui donné en paramètre...
		$lecture = mysqli_fetch_row($resultat);
		if ($adresse_saisi === $lecture[0]){
			echo("<p style='color:green;border:1px green dotted;padding:1px;'> connection : Connexion réussie ! </p>");
			return true;
		} else {
			die("<p style='color:red;border:1px red dotted;padding:1px;'> connection : surnom / mot de passe incorrect(s) ! </p>");
		}
	}
	return false;
}

// Sous-fonction pour supprimer un membre
function suppression_membre($connection_bdd,$surnom_saisi){
	$requete = "DELETE FROM membres WHERE surnom='$surnom_saisi';";
	$resultat = mysqli_query($connection_bdd,$requete);
	if (!$resultat){
		echo("<p style='color:red;border:1px red dotted;padding:1px;'> connection : Exécution de requête échouée </p>");
		return false;
	}
	return true;
}


?>