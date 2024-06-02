<?php 

	// Sous-fonction pour rechercher un utilisateur
	function afficher_info_profil($connection_bdd,$surnom="",$url_origine=""){
		$lecture_requete = "SELECT * FROM membres WHERE surnom = ?";
		$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
		if (!$preparation){
			die("<p style='color:red;border:1px red dotted;padding:1px;'> recherche_utilisateur : préparation de requête échouée </p>");
		}else{
			//echo("<p style='color:green;border:1px green dotted;padding:1px;'> recherhce_utilisateur : préparation de requête réussie </p>");
			mysqli_stmt_bind_param($preparation,"s",$surnom);
			$id; $sur; $adr; $rang; $info; $connex; $pfp;
			mysqli_stmt_bind_result($preparation,$id,$sur,$adr,$rang,$info,$connex,$pfp);
			$resultat = mysqli_stmt_execute($preparation);
			$nombre = 0;
			while(mysqli_stmt_fetch($preparation)){
				echo("<form action=$url_origine method='post' id='zone_membre'>
				<img rel='PFP de $sur' src='$pfp' />
				<p>Bonjour $sur !</p>
				<p>$info</p>
				<input type='submit' name='deconnexion' value='Se deconnecter'/>");
				
				if ($rang == "administrateur"){
					echo("<a href='register.php?origine=$url_origine' id='ajouter_membre'><i>Ajouter un membre</i></a>");
					echo("<a href='deletion.php?origine=$url_origine' id='supprimer_membre'><i>Supprimer un membre</i></a>");
					echo("<a href='poster_officiel.php?origine=$url_origine' id='poster_officiel'><i>Poster un événement officiel</i></a>");
					echo("<a href='nouveau_emploi.php?origine=$url_origine' id='poster_officiel'><i>Créez un nouveau emploi</i></a>");
				}

				echo("</form>");

			}
			mysqli_stmt_close($preparation);
		}
	}

	function afficher_profile($connection,$url_origine){
		if ($connection <> true){
			echo("<div id='zone_etranger'>
				<img rel='PFP de inconnu' src='photo_de_profile/defaut.jpg'/>
				<p style='color:red'>Vous n'êtes pas connectée à TBO !</p>
				<a href='login.php?origine=$url_origine'><p>Se connecter</p></a>
				</div>");
		} else {
			// On cherche toutes les informations sur le profil connecté.
			$connecting = connection_bdd("ecein");
			afficher_info_profil($connecting,$_SESSION["surnom"],$url_origine);
			mysqli_close($connecting);
		}
	}

	function deconnexion(){
		$_SESSION["surnom"] = "";
		$_SESSION["connected"] = false;
	}

?>