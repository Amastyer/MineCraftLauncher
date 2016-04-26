.pragma library

//.import DreamCraft 0.1 as DreamCraft

var token;
var API_ADDRESS = "http://192.168.1.103:3000/api/";

function login( email, password, cb ) {
    var url = API_ADDRESS + "/api/users/login";
    var arg = { email: email, password: password }
    var data = g_DreamCraft.httpPost( url, JSON.stringify(arg) );
    if( data ) {
        data = JSON.parse(data);
        console.log(data);
    }
}
