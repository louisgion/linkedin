const WebSocketServer = require('ws');
const mysql = require('mysql');

var con = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "",
    database: "ecein"
});

function lecture_messagerie(id,wss){
    con.query(`SELECT discussion FROM messageries WHERE id=${id};`,function(err,result,fields){
        if (err){
            throw err;
        }
        //console.log("LECTURE MYSQL : ");
        console.log(result);
        let message2 = JSON.stringify(result);
        let json2 = JSON.parse(message2);
        discussion2 = json2[0].discussion;
        wss.send(`${id}:` + discussion2);
    });
}

const wss = new WebSocketServer.Server({port:4567});

wss.on("connection", ws =>{

    let id_cible;

    ws.on("message",function(event){
        //console.log(`Message received : ${event}`);
        let message = `${event}`;
        if (message[0] == '!'){
            id_cible = message.split(":")[1];
            lecture_messagerie(id_cible,ws);
        } else {
            insertion_message(id_cible,`${event}`);
        }
    });

    ws.on("close",function(){
        console.log("The client has disconnected...");
    });

    ws.on("error",function(){
        console.log("Some error occured.");
    });

});

//function insertion_message(personne1,personne2,message,wss)

function insertion_message(id,message){
    let discussion1 = "";
    con.query(`SELECT discussion FROM messageries WHERE id = ${id};`,function(err,result,fields){
        if (err){
            throw err;
        }
        let message1 = JSON.stringify(result);
        let json1 = JSON.parse(message1);
        discussion1 = json1[0].discussion;
        discussion1 += message;
        discussion1 += "$";

        con.query(`UPDATE messageries SET discussion = '${discussion1}' WHERE id = ${id} ;`,function(err,res,fields){
            if ( err ){
                throw err;
            }

            wss.clients.forEach(function(client){
                con.query(`SELECT discussion FROM messageries WHERE id = ${id} ;`,function(err,result,fields){
                    if (err){
                        throw err;
                    }
                    //console.log("LECTURE MYSQL : ");
                    console.log(result);
                    let message2 = JSON.stringify(result);
                    let json2 = JSON.parse(message2);
                    discussion2 = json2[0].discussion;
                    let discussion_finale = `${id}:`+ discussion2;
                    console.log(discussion_finale);
                    client.send(discussion_finale);
                });
            });
        });
    });
}


console.log("Websocket server is running on port 4567 !");