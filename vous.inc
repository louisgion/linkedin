<?php 

// Sous-fonction pour rechercher tous les évènements d'une catégorie
// dans la table 'evenements' et les afficher une par une...
function recherche_evenement_session($connection_bdd,$surnom_session){
	$lecture_requete = "SELECT * FROM evenements WHERE envoyeur=?";
		$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
		if (!$preparation){
			echo("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_evenement : préparation de requête échouée </p>");
			return false;
		}else{

			$SESSION_SUR;
			mysqli_stmt_bind_param($preparation,"s",$SESSION_SUR);
			$SESSION_SUR = $surnom_session;

			$id; $date; $titre; $categorie; $texte; $envoyeur; $likes; $dislikes; $cache; $images;
			mysqli_stmt_bind_result($preparation,$id, $date, $titre, $categorie, $texte, $envoyeur, $likes, $dislikes, $cache, $images);
			$resultat = mysqli_stmt_execute($preparation);
			$nombre = 0;
			while(mysqli_stmt_fetch($preparation)){
				afficher_evenement_session($id,$date,$titre,$categorie,$texte,$envoyeur,$likes,$dislikes,$cache,$images);
				$nombre++;
			}
			echo("<p>$nombre événement(s) trouvé(s)</p>");
			mysqli_stmt_close($preparation);
	}

}

	// Fonction pour afficher un évènement.
	// L'affichage peut prendre des formes en fonction de
	// la catégorie de l'événement...
	function afficher_evenement_session($id,$date,$titre,$categorie,$texte,$envoyeur,$likes,$dislikes,$cache,$images){
		if ($categorie == "0"){
			// On sépare le nom des images...
			$tableau_img = explode("+",$images);
			echo("<form method='post' action='vous.php' class='E0'><p>$titre</p><p>$date</p>");
			echo("<p>Envoyé par $envoyeur</p>");
			echo("<div class='diaporama'>");
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

			echo("
				<div class='filtrage'>
				<input type='radio' name='filtrer' value='0' checked/><p>Public</p>
				<input type='radio' name='filtrer' value='1'/><p>Restreint au réseau</p>
				</div>
				");

			echo("<div class='filtrage'><input type='submit' name='enlever' value='Supprimer'/><input type='submit' name='filtrage' value='Filtrer'/></div>");
			

			echo("</form>");
		}
		if ($categorie == "3"){
			echo("<form method='post' action='vous.php' class='E3'><p>$titre</p><p>$date</p>");
			echo("<p>Envoyé par $envoyeur</p>");
			echo("<p>$texte</p>");
			echo("<p style='color:green;'>$likes</p><p style='color:red;'>$dislikes</p>");
			echo("<input type='hidden' name='id_cible' value=$id/>");
			echo("<input type='submit' name='enlever' value='Supprimer'/><br/>");
			echo("<form method='post' action='vous.php' >
				<input type='radio' name='filtrer' value='0'/><p>Public</p>
				<input type='radio' name='filtrer' value='1'/><p>Restreint au réseau</p>
				<input type='hidden' name='id_cible' value=$id/>
				<input type='submit' name='filtrage' value='filtrer'/>
				</form>");
			echo("</form>");
		}
		if ($categorie == "1"){
			// On sépare le nom des images...
			$tableau_img = explode("+",$images);
			echo("<form method='post' action='vous.php' class='E1'><p>$titre</p><p>$date</p>");
			echo("<p>Envoyé par $envoyeur</p>");
			echo("<div class='diaporama'>");
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

			echo("
				<div class='filtrage'>
				<input type='radio' name='filtrer' value='0' checked/><p>Public</p>
				<input type='radio' name='filtrer' value='1'/><p>Restreint au réseau</p>
				</div>
				");

			echo("<div class='filtrage'><input type='submit' name='enlever' value='Supprimer'/><input type='submit' name='filtrage' value='Filtrer'/></div>");

			echo("</form>");
		}

	}


// Sous-fonction pour supprimer l'événement avec l'ID donné en paramètre

function enlever_evenement_id($connection_bdd,$id){
	$lecture_requete = "DELETE FROM evenements WHERE id = ?";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		echo("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_utilisateur : préparation de requête échouée </p>");
		return false;
	}else{
		//echo("<p style='color:green;border:1px green dotted;padding:1px;'> recherhce_utilisateur : préparation de requête réussie </p>");
		mysqli_stmt_bind_param($preparation,"i",$id);
		$resultat = mysqli_stmt_execute($preparation);
		mysqli_stmt_close($preparation);
		return true;
	}
}

// Sous-fonction pour afficher les informations du membre en détail
function afficher_membre_detail($surnom,$rang,$info,$photodeprofile){
	echo("<article id='ton_profil'>
		<h1>$surnom</h1>
		<h3>$rang</h3>
		<img src='$photodeprofile'/>
		<div class='information'>
		$info
		<div>
		</article>");
}

// Sous-fonction pour lire toutes les informations d'un membre...
function lire_membre($connection_bdd,$surnom){
	$lecture_requete = "SELECT * FROM membres WHERE surnom = ?";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		echo("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_utilisateur : préparation de requête échouée </p>");
		return false;
	}else{
		//echo("<p style='color:green;border:1px green dotted;padding:1px;'> recherhce_utilisateur : préparation de requête réussie </p>");
		mysqli_stmt_bind_param($preparation,"s",$surnom);
		$id; $surnom; $adresse; $rang; $info; $connexions; $photodeprofile;
		mysqli_stmt_bind_result($preparation,$id, $surnom, $adresse, $rang, $info, $connexions, $photodeprofile);
		$resultat = mysqli_stmt_execute($preparation);
		while(mysqli_stmt_fetch($preparation)){
				afficher_membre_detail($surnom,$rang,$info,$photodeprofile);
			}
		mysqli_stmt_close($preparation);
		return true;
	}
}

// Sous-fonction pour rendre un événement privé...
function filtrage_evenement($connection_bdd,$id,$indication){
	$lecture_privatisation = "UPDATE evenements SET cache = ? WHERE id = ?; ";
	$preparation = mysqli_prepare($connection_bdd,$lecture_privatisation);
	if (!$preparation){
		echo("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_utilisateur : préparation de requête échouée </p>");
		return false;
	}else{
		//echo("<p style='color:green;border:1px green dotted;padding:1px;'> recherhce_utilisateur : préparation de requête réussie </p>");
		mysqli_stmt_bind_param($preparation,"ii",$indication,$id);
		$resultat = mysqli_stmt_execute($preparation);
		mysqli_stmt_close($preparation);
		return true;
	}
}


?>