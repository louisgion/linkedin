<?php

// Sous-fonction pour indiquer le nombre d'événements dans la table 'evenements'...
function recherche_nombre_evenement($connection_bdd){
	$lecture_requete = "SELECT * FROM evenements";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		die("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_nombre_evenement : préparation de requête échouée </p>");
		return false;
	}else{
		mysqli_stmt_bind_result($preparation,$id,$date,$titre,$categorie,$texte,$envoyeur,$likes,$dislikes,$cache,$images);
		$resultat = mysqli_stmt_execute($preparation);
		$nombre = 0;
		while(mysqli_stmt_fetch($preparation)){
			$nombre++;
		}
		mysqli_stmt_close($preparation);
		return $nombre;
	}
}

function informations_image($image){
	foreach($image as $cle => $valeur){
		echo("<p>$cle => $valeur</p><br/>");
	}
}

function verifier_images($connection_bdd,$images,$limite){
	// On compte le nombre d'événements déjà posté sur le site...
	$nombre_event = recherche_nombre_evenement($connection_bdd);
	// On compte le nombre d'images publiées...
	$nombre_images = count($images["diaporama"]["type"]);
	$titre; // Le titre de l'image
	$type; // Le format
	$longueur; // La longueur en bits
	$temporaire; // L'emplacement temporaire.
	$extensions_autorises = array("image/png","image/jpeg","image/jpg","image/gif");
	$titre_element = "img_";
	$titre_tableau = array();
	for ($i = 0; $i < $nombre_images; $i++){
		$titre_element = "img_";
		$titre = $images["diaporama"]["name"][$i];
		$type = $images["diaporama"]["type"][$i];
		$longueur = $images["diaporama"]["size"][$i];
		$temporaire = $images["diaporama"]["tmp_name"][$i];
		// On vérifie si l'extension du fichier donné est valide...
		if ( in_array($type,$extensions_autorises) <> true ){
			echo("<p style='color:red;border:1px red dotted;padding:1px;'> verification image : L'extension du fichier $i est invalide </p>");
			return false;
		}
		// On vérifie si la taille maximale n'est pas dépassée...
		if ( $longueur > $limite ){
			echo("<p style='color:red;border:1px red dotted;padding:1px;'> verification image : Le fichier $i est trop gros </p>");
			return false;
		}
		$titre_element = $titre_element.$nombre_event."_".$i;
		switch($type){
			case "image/jpg":
				$titre_element .= ".jpg";
				break;
			case "image/jpeg":
				$titre_element .= ".jpeg";
				break;
			case "image/png":
				$titre_element .= ".png";
				break;
			case "image/gif":
				$titre_element .= ".gif";
				break;
		}
		//echo("<p>Titre de l'image $i : $titre_element</p><br/>");
		$titre_tableau[] = $titre_element;
		copy($temporaire,"events/$titre_element");
	}
	return $titre_tableau;
}



// Sous-fonction pour insére un nouveau événement dans la table
// 'evenements' de la base de donnée 'the_big_one'...
// Liste des catégories :
// 0 --> Evenement en provenance d'un membre
// 1 --> Publication de CV
// 2 --> Annonce de stages, d'emplois, d'événements officiaux.
function inserer_nouvel_evenement($connection_bdd,$titre,$categorie,$texte,$envoyeur,$cache,$images){

	$requete = "INSERT INTO evenements (date,titre,categorie,texte,envoyeur,likes,dislikes,cache,images) VALUES (?,?,?,?,?,?,?,?,?)";
	$preparation = mysqli_prepare($connection_bdd,$requete);

	if (!$preparation){
		// MESSAGE D'ECHEC
		echo("<p style='color:red;border:1px red dotted;padding:1px;'> inserer_nouvel_evenement : Préparation de requête échouée </p>");
		return false;
	}

	if ($categorie == 3){

			mysqli_stmt_bind_param($preparation,"ssissiiis",$DATE,$TITRE,$CATEGORIE,$TEXTE,$ENVOYEUR,$LIKES,$DISLIKES,$CACHE,$IMAGES);
			$DATE = date("Y-m-d H:i:s");
			$TITRE = $titre;
			$CATEGORIE = $categorie;
			$TEXTE = $texte;
			$ENVOYEUR = $envoyeur;
			$LIKES = 0;
			$DISLIKES = 0;
			$CACHE = (int)$cache;
			$IMAGES = "";
			
			mysqli_stmt_execute($preparation);

			mysqli_stmt_close($preparation);
			return true;



	}  

	if ( ($categorie == 0) or ($categorie == 1) ) {

			if ( empty($images["name"]) ) {

			// ------------------------------------------------------------------------------------------------

			// PROCESSUS D'INSCRIPTION EN COURS
			mysqli_stmt_bind_param($preparation,"ssissiiis",$DATE,$TITRE,$CATEGORIE,$TEXTE,$ENVOYEUR,$LIKES,$DISLIKES,$CACHE,$IMAGES);
			$DATE = date("Y-m-d H:i:s");
			$TITRE = $titre;
			$CATEGORIE = $categorie;
			$TEXTE = $texte;
			$ENVOYEUR = $envoyeur;
			$LIKES = 0;
			$DISLIKES = 0;
			$CACHE = (int)$cache;
			$IMAGES = "";
			
			mysqli_stmt_execute($preparation);
			// FIN PROCESSUS D'INSCRIPTION

			// ------------------------------------------------------------------------------------------------

			} else {

			// ------------------------------------------------------------------------------------------------

			// ENREGISTREMENT DE LA NOUVELLE PHOTO DE PROFILE...
			$noms_images = verifier_images($connection_bdd,$images,1000000000);
			$resultat_final = "";
			for ($i = 0;$i < count($noms_images) ;$i++){
				$resultat_final .= $noms_images[$i];
				$resultat_final .= "+";
			}


			// PROCESSUS D'INSCRIPTION EN COURS
			mysqli_stmt_bind_param($preparation,"ssissiiis",$DATE,$TITRE,$CATEGORIE,$TEXTE,$ENVOYEUR,$LIKES,$DISLIKES,$CACHE,$IMAGES);
			$DATE = date("Y-m-d H:i:s");
			$TITRE = $titre;
			$CATEGORIE = $categorie;
			$TEXTE = $texte;
			$ENVOYEUR = $envoyeur;
			$LIKES = 0;
			$DISLIKES = 0;
			$CACHE = $cache;
			$IMAGES = $resultat_final;
			mysqli_stmt_execute($preparation);
			// FIN PROCESSUS D'INSCRIPTION

			// ------------------------------------------------------------------------------------------------

			}

		
		mysqli_stmt_close($preparation);
		return true;

	}
		
}





// Sous-fonction pour rechercher tous les évènements d'une catégorie
// dans la table 'evenements' et les afficher une par une...
function recherche_evenement($connection_bdd,$surnom_session,$categorie=""){
	if (!empty($categorie)){

		if ($categorie == "CONNEXIONS"){

			// On fait la partie de la requete SQL qui restreint les événements à ceux
			// du membre en sessio net de ses connexions.
			$requete_restriction = "WHERE envoyeur IN ('$surnom_session'";
			$requete1 = "SELECT connexions FROM membres WHERE surnom='$surnom_session';";
			$resultat1 = mysqli_query($connection_bdd,$requete1);
			$lecture = mysqli_fetch_row($resultat1);
			$liste_connexions = $lecture[0];
			$connexions_elements = explode("+",$liste_connexions);
			foreach($connexions_elements as $cle => $valeur){
				if ($valeur <> ""){
					$requete_restriction .= ",";
					$requete_restriction .= "'$valeur'";
				}
			}
			$requete_restriction .= ");";

			$lecture_requete = "SELECT * FROM evenements ".$requete_restriction;
			//echo("<p>$lecture_requete</p>");
			$resultat2 = mysqli_query($connection_bdd,$lecture_requete);
			if (!$resultat2){
				echo("<p>WTF</p>");
			}
			while($lecture = mysqli_fetch_row($resultat2)){
				//afficher_evenement($id,$date,$titre,$categorie,$texte,$envoyeur,$likes,$dislikes,$cache,$images);
				afficher_evenement($lecture[0],$lecture[1],$lecture[2],$lecture[3],$lecture[4],$lecture[5],$lecture[6],$lecture[7],$lecture[8],$lecture[9]);
				//var_dump($lecture);
			}
			

		} else {

			$lecture_requete = "SELECT * FROM evenements WHERE categorie = ? ";
			$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
			if (!$preparation){
				echo("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_evenement : préparation de requête échouée </p>");
				return false;
			}else{

				$categorie_ciblee = $categorie;

				mysqli_stmt_bind_param($preparation,"i",$categorie_ciblee);

				//echo("<p style='color:green;border:1px green dotted;padding:1px;'> recherhce_utilisateur : préparation de requête réussie </p>");
				$id; $date; $titre; $categorie; $texte; $envoyeur; $likes; $dislikes; $cache; $images;
				mysqli_stmt_bind_result($preparation,$id, $date, $titre, $categorie, $texte, $envoyeur, $likes, $dislikes, $cache, $images);
				$resultat = mysqli_stmt_execute($preparation);
				$nombre = 0;
				while(mysqli_stmt_fetch($preparation)){
					afficher_evenement($id,$date,$titre,$categorie,$texte,$envoyeur,$likes,$dislikes,$cache,$images);
					$nombre++;
				}
				echo("<p>$nombre événement(s) trouvé(s)</p>");
				mysqli_stmt_close($preparation);
			}

		}

		
	} else {

		$lecture_requete = "SELECT * FROM evenements ";
		$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
		if (!$preparation){
			echo("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_evenement : préparation de requête échouée </p>");
			return false;
		}else{
			//echo("<p style='color:green;border:1px green dotted;padding:1px;'> recherhce_utilisateur : préparation de requête réussie </p>");
			$id; $date; $titre; $categorie; $texte; $envoyeur; $likes; $dislikes; $cache; $images;
			mysqli_stmt_bind_result($preparation,$id, $date, $titre, $categorie, $texte, $envoyeur, $likes, $dislikes, $cache, $images);
			$resultat = mysqli_stmt_execute($preparation);
			$nombre = 0;
			while(mysqli_stmt_fetch($preparation)){
				afficher_evenement($id,$date,$titre,$categorie,$texte,$envoyeur,$likes,$dislikes,$cache,$images);
				$nombre++;
			}
			echo("<p>$nombre événement(s) trouvé(s)</p>");
			mysqli_stmt_close($preparation);
		}

	}
}

// sous-fonction pour lire toutes les publications d'un membre...
function lire_evenements_surnom($connection_bdd,$surnom,$surnom_session){
		$lecture_requete = "SELECT * FROM evenements WHERE envoyeur = '$surnom'";
		$requete = mysqli_query($connection_bdd,$lecture_requete);
		
			$id; $date; $titre; $categorie; $texte; $envoyeur; $likes; $dislikes; $cache; $images;
			
			
			
			while($ligne = mysqli_fetch_row($requete)){
				
				//function lecture_autorisee($connection_bdd,$surnom_session,$id_evenement){
				if (lecture_autorisee($connection_bdd,$surnom_session,$ligne[0]) == true){
					afficher_evenement($ligne[0],$ligne[1],$ligne[2],$ligne[3],$ligne[4],$ligne[5],$ligne[6],$ligne[7],$ligne[8],$ligne[9]);					
				} else {
					echo("<article style='width:100%;border:1px black solid;padding:10px;background-color:#ff7979;'> Tu ne peux pas lire cet evenement car tu n'est pas une connexion de $surnom :( </article>");
				}

			}
		
}


// Fonction pour afficher un évènement.
// L'affichage peut prendre des formes en fonction de
// la catégorie de l'événement...
function afficher_evenement($id,$date,$titre,$categorie,$texte,$envoyeur,$likes,$dislikes,$cache,$images){
	if ($categorie == "0"){
			// On sépare le nom des images...
			$longueur = 0;
			$tableau_img = explode("+",$images);
			foreach($tableau_img as $cle => $valeur){
				if (!empty($valeur)){
					$longueur++;
				}
			}
			echo("<form method='post' action='vous.php' class='E0'><p>$titre</p><p>$date</p>");
			echo("<p>Envoyé par <b>$envoyeur</b></p>");
			echo("<div class='diaporama'>");
			echo("<p class='image_compteur'>1/$longueur</p>");
			foreach($tableau_img as $index => $img){
				if ($img <> ""){
					$chemin = "events/".$img;
					echo("<img src=$chemin />");

				}
			}
			echo("</div>");
			echo("<div class='contenu_evenement'><p>$texte</p></div>");
			echo("<div class='like_et_dislike'><div><p style='color:green;'>$likes</p><input type='submit' value='Upvote' name='upvote'/></div><div><p style='color:red;'>$dislikes</p><input type='submit' value='Downvote' name='downvote'/></div></div>");
			echo("<input type='hidden' name='id_cible' value=$id/>");

			echo("</form>");
		}
		if ($categorie == "3"){
			echo("<form method='post' action='vous.php' class='E3'><p>$titre</p><p>$date</p>");
			echo("<p>Envoyé par $envoyeur</p>");
			echo("<div class='contenu_evenement'><p>$texte</p></div>");
			echo("<div class='like_et_dislike'><div><p style='color:green;'>$likes</p><input type='submit' value='Upvote' name='upvote'/></div><div><p style='color:red;'>$dislikes</p><input type='submit' value='Downvote' name='downvote'/></div></div>");
			echo("<input type='hidden' name='id_cible' value=$id/>");
			echo("</form>");
		}
		if ($categorie == "1"){
			// On sépare le nom des images...
			$longueur = 0;
			$tableau_img = explode("+",$images);
			foreach($tableau_img as $cle => $valeur){
				if (!empty($valeur)){
					$longueur++;
				}
			}
			echo("<form method='post' action='vous.php' class='E1'><p>$titre</p><p>$date</p>");
			echo("<p>Envoyé par $envoyeur</p>");
			echo("<div class='diaporama'>");
			echo("<p class='image_compteur'>1/$longueur</p>");
			foreach($tableau_img as $index => $img){
				if ($img <> ""){
					$chemin = "events/".$img;
					echo("<img src=$chemin />");

				}
			}
			echo("</div>");
			echo("<div class='contenu_evenement'><p>$texte</p></div>");
			echo("<div class='like_et_dislike'><div><p style='color:green;'>$likes</p><input type='submit' value='Upvote' name='upvote'/></div><div><p style='color:red;'>$dislikes</p><input type='submit' value='Downvote' name='downvote'/></div></div>");
			echo("<input type='hidden' name='id_cible' value=$id/>");
			echo("</form>");
		}

}


// Sous-fonction pour effectuer des recherches avec paramètres de
// notifications. Est principalement utilisée par la page notifications.php
function recherche_notifications_avancee($categories,$ordre,$filtrage,$connexion,$connexion_bdd,$session_surnom=""){

	$string_debut = "SELECT * FROM evenements ";
	$string_categories;
	$string_filtrage;
	$string_ordre;
	$string_final;

	// string_final = string_debut . string_categories . string_filtrage . string_ordre.

	if (!empty($categories)){
		$string_categories = "WHERE categorie IN (69 ";
		foreach($categories as $cle => $valeur){
			$string_categories .= (",".$valeur);
		}
		$string_categories .= ") ";
	}
	
	$string_debut .= $string_categories;

	if ( $connexion == true ){
		// Si on demande uniquement celles de notre réseau, on ajoute
		// une nouvelle clotûre IN...

		if($filtrage == "seulement"){

			$requete1 = "SELECT connexions FROM membres WHERE surnom='$session_surnom';";
			$resultat1 = mysqli_query($connexion_bdd,$requete1);
			$lecture = mysqli_fetch_row($resultat1);
			$liste_connexions = $lecture[0];
			$connexions_elements = explode("+",$liste_connexions);
			//var_dump($connexions_elements);

			$string_debut .= "AND envoyeur IN ('@@@'";
			foreach($connexions_elements as $cle => $valeur){
				if ($valeur <> ""){
					$string_debut .= ",'";
					$string_debut .= $valeur;
					$string_debut .= "'";
					
				}
			}
			$string_debut .= ") ";

		}
		
	}

	if ($ordre == "croissant"){
		$string_ordre = "ORDER BY id ASC;";
	}else{
		$string_ordre = "ORDER BY id DESC;";
	}

	$string_debut .= $string_ordre;

	$string_final = $string_debut;

	//echo($string_final);

	$resultat_lecture = mysqli_query($connexion_bdd,$string_final);

	while( $ligne = mysqli_fetch_row($resultat_lecture) ){

		if ( $ligne[8] == 1 ){
			if (lecture_autorisee($connexion_bdd,$session_surnom,$ligne[0]) == true){
				//afficher_evenement($id,$date,$titre,$categorie,$texte,$envoyeur,$likes,$dislikes,$cache,$images);
				afficher_evenement($ligne[0],$ligne[1],$ligne[2],$ligne[3],$ligne[4],$ligne[5],$ligne[6],$ligne[7],$ligne[8],$ligne[9]);
			} else {
				echo("<article style='width:100%;border:1px black solid;padding:10px;background-color:#ff7979;margin-bottom:10px;'> Tu ne peux pas lire cet événement car tu n'es pas une connexion de $ligne[5] </article>");
			}
		} else {
			afficher_evenement($ligne[0],$ligne[1],$ligne[2],$ligne[3],$ligne[4],$ligne[5],$ligne[6],$ligne[7],$ligne[8],$ligne[9]);
		}

		

	}

}


// Sous-fonction pour liker ou disliker un évnènement
function like_ou_dislike($connection_bdd,$id){
	$lecture_requete = "SELECT likes,dislikes FROM evenements WHERE id = ?";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		echo("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_utilisateur : préparation de requête échouée </p>");
		return false;
	}else{
		//echo("<p style='color:green;border:1px green dotted;padding:1px;'> recherhce_utilisateur : préparation de requête réussie </p>");
		mysqli_stmt_bind_param($preparation,"i",$id);
		$likee; $dislikee;
		mysqli_stmt_bind_result($preparation,$likee,$dislikee);
		$resultat = mysqli_stmt_execute($preparation);

		mysqli_stmt_fetch($preparation);

			if ( isset($_POST["upvote"]) ){
				$likee++;
				//echo($likee);
			} elseif ( isset($_POST["downvote"]) ) {
				$dislikee++;
				//echo($dislikee);
			}

			mysqli_stmt_close($preparation);

			//echo("likee : ".$likee." dislikee : ".$dislikee);

			$idee = substr($id,0,strlen($id)-1);
			$requete2 = "UPDATE evenements SET likes=?,dislikes=? WHERE id=?;";
			$prep2 = mysqli_prepare($connection_bdd,$requete2);
			mysqli_stmt_bind_param($prep2,"iii",$likee,$dislikee,$idee);
			$resultat2 = mysqli_stmt_execute($prep2);
			if (!$resultat2){
				echo("<p style='color:red;border:1px red dotted;padding:1px;'> like_ou_dislike : mise à jour des valeurs échouées </p>");
			}
			mysqli_stmt_close($prep2);
			
		return true;
	}
}

function lecture_autorisee($connection_bdd,$surnom_session,$id_evenement){

	$requete_lecture1 = "SELECT envoyeur FROM evenements WHERE id = ? ";
	$preparation1 = mysqli_prepare($connection_bdd,$requete_lecture1);
	mysqli_stmt_bind_param($preparation1,"i",$id_evenement);
	$envoyeur_surnom;
	mysqli_stmt_bind_result($preparation1,$envoyeur_surnom);
	$resultat1 = mysqli_execute($preparation1);
	mysqli_stmt_fetch($preparation1);
	mysqli_stmt_close($preparation1);

	if ($envoyeur_surnom <> "ECEin"){

	// On veut savoir les connexions de l'envoyeur...
	$requete_lecture_connexions = "SELECT connexions FROM membres WHERE surnom = '$envoyeur_surnom' ";
	//echo($requete_lecture_connexions);
		
	$resultat2 = mysqli_query($connection_bdd,$requete_lecture_connexions);

	$ligne = mysqli_fetch_row($resultat2);

	$connexions_elements = explode("+",$ligne[0]);

	foreach($connexions_elements as $cle => $valeur){
		if ($valeur <> ""){
			if ($surnom_session == $valeur){
				return true;
			}
		}
	}
	
	return false;

	} else {

		return true;

	}

	
}



?>