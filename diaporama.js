

let diapos = document.getElementsByClassName("diaporama");
let compteur = document.getElementsByClassName("image_compteur");
let indexes = [];
let limites = [];

function changer_index(indice){
	//alert(`EEEK indice ${indice}`);
	indexes[indice]++;

	if (indexes[indice] >= limites[indice]){
		indexes[indice] = 0;
	}

	let comptarus = compteur[indice].innerText.split("/");
	comptarus[0]++;
	if ( comptarus[0] > comptarus[1]){
		comptarus[0] = 1;
	}

	//console.log(indexes[indice]);
	// On met à jour les états des photos...
	
	for ( let i = 0 ; i < limites[indice] ; i++ ){
		if ( i == indexes[indice] ){
			diapos[indice].getElementsByTagName("img")[i].style.display = "inline";
		} else {
			diapos[indice].getElementsByTagName("img")[i].style.display = "none";
		}
	}

	compteur[indice].innerText = comptarus[0]+"/"+comptarus[1];
	
}

window.addEventListener("load",function(){
	// Initialisation

	for(let i = 0 ; i < diapos.length ; i++ ){
		diapos[i].addEventListener("click", function(e){changer_index(i);} );
		let longueur_image = diapos[i].getElementsByTagName("img").length;
		limites.push(longueur_image);
		indexes.push(0);
		
		for ( let j = 0 ; j < longueur_image ; j++ ){
			if (j == 0){
				diapos[i].getElementsByTagName("img")[j].style.display = "inline";
			} else {
				diapos[i].getElementsByTagName("img")[j].style.display = "none";
			}
		}
		
	}
	
});