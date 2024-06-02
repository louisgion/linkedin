<?php 

include("gestion_publications_evenements.inc");

// Sous-fonction pour afficher un emploi
// Ne s'emploi que dans la page 'emplois.php'...
function afficher_emploi($id,$date,$intitule,$nature,$remuneration,$entreprise,$localisation,$connection_session){
	if ($connection_session == false){
		echo("<form method='post' action='emplois.php' class='emploi'>
		
		<div class='meta_info'>
		<h5>Date de publication : $date</h5>
		<h5>$nature</h5>
		<h5>Localisation : $localisation</h5>
		</div>

		<span style='color:grey;'><h2>$intitule</h2></span>
		<h3>Poste proposé par <span style='color:blue;'>$entreprise</span></h3>
		<h3>Rémunération attendue : <span style='color:green;'>$remuneration</span></h3>
		
		<p style='color:red;border:1px red dotted;padding:1px;'> Vous devez vous authentifier en tant que membre pour postuler. </p>

		</form>");
	} else {
		echo("<form method='post' action='emplois.php' class='emploi'>
		
		<div class='meta_info'>
		<h5>Date de publication : $date</h5>
		<h5>$nature</h5>
		<h5>Localisation : $localisation</h5>
		</div>

		<span style='color:grey;'><h2>$intitule</h2></span>
		<h3>Poste proposé par <span style='color:blue;'>$entreprise</span></h3>
		<h3>Rémunération attendue : <span style='color:green;'>$remuneration euros / mois </span></h3>
		
		<input type='hidden' value=$id name='emploi_id'/>
		
		<input type='submit' value='Postuler' name='Postuler'/>
		</form>");
	}
}

// Sous-fonction pour lire tous les emplois de la table 'emplois'
// Ne s'emploi que dans la page 'emplois.php'...
function lire_emplois($connection_bdd,$connection_session=false){
	$lecture_requete = "SELECT * FROM emplois;";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		die("<p style='color:red;border:1px red dotted;padding:1px;'> lire_emplois : préparation de requête échouée </p>");
	}else{
		//echo("<p style='color:green;border:1px green dotted;padding:1px;'> lire_emplois : préparation de requête réussie </p>");
		$ID; $DATE; $INTITULE; $NATURE; $REMUNERATION; $ENTREPRISE; $LOCALISATION;
		mysqli_stmt_bind_result($preparation,$ID,$DATE,$INTITULE,$NATURE,$REMUNERATION,$ENTREPRISE,$LOCALISATION);
		$resultat = mysqli_stmt_execute($preparation);
		$nombre = 0;
		while(mysqli_stmt_fetch($preparation)){
			// On affiche l'emploi en question...
			afficher_emploi($ID,$DATE,$INTITULE,$NATURE,$REMUNERATION,$ENTREPRISE,$LOCALISATION,$connection_session);
			$nombre++;
		}
		echo("<p>$nombre emploi(s) trouvé(s)</p>");
		mysqli_stmt_close($preparation);
	}
}

// Sous-fonction pour lire un emploi particulier...
function lire_emploi_precis($connection_bdd,$id){
	$lecture_requete = "SELECT * FROM emplois WHERE id = ?;";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		die("<p style='color:red;border:1px red dotted;padding:1px;'> lire_emplois : préparation de requête échouée </p>");
	}else{
		$code_cible = $id;
		$resultato = array();
		mysqli_stmt_bind_param($preparation,"i",$code_cible);
		$ID; $DATE; $INTITULE; $NATURE; $REMUNERATION; $ENTREPRISE; $LOCALISATION;
		mysqli_stmt_bind_result($preparation,$ID,$DATE,$INTITULE,$NATURE,$REMUNERATION,$ENTREPRISE,$LOCALISATION);
		$resultat = mysqli_stmt_execute($preparation);
		while(mysqli_stmt_fetch($preparation)){
			$resultato[] = $INTITULE;
			$resultato[] = $NATURE;
			$resultato[] = $REMUNERATION;
			$resultato[] = $ENTREPRISE;
			$resultato[] = $LOCALISATION;
		}
		mysqli_stmt_close($preparation);
		return $resultato;
	}
}

// Sous-fonction pour ajouter une postulation dans la liste des événnements.
function informer_postulation($connection_bdd,$surnom_membre,$id){
	//function inserer_nouvel_evenement($connection_bdd,$titre,$categorie,$texte,$envoyeur,$cache,$images)
	$INFOS = lire_emploi_precis($connection_bdd,$id);
	$titre = "$surnom_membre a postulé pour le poste ".$INFOS[0];
	$categorie = 3;
	$texte = " Nature : ".$INFOS[1];
	$texte .= " Rémunération : ".$INFOS[2];
	$texte .= " Entreprise : ".$INFOS[3];
	$texte .= " Localisation : ".$INFOS[4];
	$envoyeur = "ECEin";
	$cache = 0;
	$images = "";
	$test = inserer_nouvel_evenement($connection_bdd,$titre,$categorie,$texte,$envoyeur,$cache,$images);      
	//inserer_nouvel_evenement($connection_bdd,)
}

// Sous-fonction pour insérer un nouveau emploi
// dans la table 'emplois'.
function inserer_emploi($connection_bdd,$intitule,$nature,$remuneration,$entreprise,$localisation){
	$requete1 = "INSERT INTO emplois (date,intitule,nature,remuneration,entreprise,localisation) VALUES (?,?,?,?,?,?);";
	$preparation = mysqli_prepare($connection_bdd,$requete1);
	if (!$preparation){
		echo("<p style='color:red;border:1px red dotted;padding:1px;'> inserer_emploi : préparation de requête échouée </p>");
		return false;
	}else{
		$DATE = date("Y-m-d H:i:s");
		mysqli_stmt_bind_param($preparation,"ssssss",$DATE,$intitule,$nature,$remuneration,$entreprise,$localisation);
		$resultat = mysqli_stmt_execute($preparation);
		mysqli_stmt_close($preparation);
		//function inserer_nouvel_evenement($connection_bdd,$titre,$categorie,$texte,$envoyeur,$cache,$images)
		$texte = " Nature : ".$nature;
		$texte .= " Rémunération : ".$remuneration;
		$texte .= " Entreprise : ".$entreprise;
		$texte .= " Localisation : ".$localisation;
		$envoyeur = "ECEin";
		$cache = 0;
		$images = "";
		inserer_nouvel_evenement($connection_bdd,"Nouveau offre de $nature disponible !",3,$texte,$envoyeur,$cache,$images);
	}
}


?>