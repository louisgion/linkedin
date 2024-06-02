<?php 

function afficher_discussion($personne1,$personne2,$id){
	echo("<article class='disci' id='$id' onclick='changer_disc($id)'> $personne1 & $personne2 </article>");
}

function lire_discussions($connection_bdd,$surnom_session){
	$lecture_requete = "SELECT * FROM messageries WHERE personne1 = ? OR personne2 = ?";
	$preparation = mysqli_prepare($connection_bdd,$lecture_requete);
	if (!$preparation){
		die("<p style='color:red;border:1px red dotted;padding:1px;'> lire_discussions : préparation de requête échouée </p>");
	}else{
		//echo("<p style='color:green;border:1px green dotted;padding:1px;'> lire_discussions : préparation de requête réussie </p>");
		mysqli_stmt_bind_param($preparation,"ss",$surnom_session,$surnom_session);
		$per1; $disc; $per2; $id;
		mysqli_stmt_bind_result($preparation,$per1,$disc,$per2,$id);
		$resultat = mysqli_stmt_execute($preparation);
		$nombre = 0;
		while(mysqli_stmt_fetch($preparation)){
			//echo("<p style='font-size:8px;'>personne1 : $per1 // discussion : $disc // personne2 : $per2 </p><br/>");
			afficher_discussion($per1,$per2,$id);
			$nombre++;
		}
		echo("<p>$nombre discussion(s) trouvée(s)</p>");
		mysqli_stmt_close($preparation);
	}
}

?>