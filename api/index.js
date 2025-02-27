const express = require("express");
const bodyParser = require("body-parser");
const {MongoClient} = require("mongodb");
const PgMem = require("pg-mem");

const db = PgMem.newDb();

const fs = require('fs');

// Measurements database setup and access

let database = null;
const collectionName = "measurements";

async function startDatabase() {
    const uri = "mongodb://localhost:27017/?maxPoolSize=20&w=majority";	
    const connection = await MongoClient.connect(uri, {useNewUrlParser: true});
    database = connection.db();
}

async function getDatabase() {
    if (!database) await startDatabase();
    return database;
}

async function insertMeasurement(message) {
    const {insertedId} = await database.collection(collectionName).insertOne(message);
    return insertedId;
}

async function getMeasurements() {
    return await database.collection(collectionName).find({}).toArray();	
}

/* API Server
- const app = express() creates a new Express application
- app.use(bodyParser.urlencoded({extended:false})) configures the Express app to use the 
body-parser middleware to parse URL-encoded request bodies.
- app.use(express.static('spa/static')) configures the Express app to serve static files 
from the spa/static directory.
*/

const app = express();

app.use(bodyParser.urlencoded({extended:false}));

app.use(express.static('spa/static'));

const PORT = 8080;

// Defines a route that handles HTTP POST requests to /measurement and inserts a new measurement into the MongoDB database.
app.post('/measurement', function (req, res) {
    // get the current date and time
    const now = new Date(); 
    const options = { timeZone: 'America/Argentina/Buenos_Aires' };
    const timestamp = now.toLocaleString('en-US', options); 
    console.log(timestamp + "       Device ID: " + req.body.id + "     Temperature: " + req.body.t + " °C      Humidity: " + req.body.h + " %       Pressure: " + req.body.p + " Pa");	
    
    // Check if device key is registered in the database
    const deviceKey = req.body.k;
    var devkey = db.public.many("SELECT * FROM devices WHERE key = '"+ deviceKey +"'");
    //console.log(devkey);

    if (devkey == "") {
        console.error('Device key not registered:', deviceKey);
        res.send('Not registered\n');
        return;
    }

    // Device key is registered, insert measurement into database
    const {insertedId} = insertMeasurement({d:timestamp, id:req.body.id, k:req.body.k, t:req.body.t, h:req.body.h, p:req.body.p});
	res.send("Measurement received and inserted into the DB\n");
    //});
});

// Defines a route that handles HTTP POST requests to /device and inserts a new device into the in-memory database.
app.post('/device', function (req, res) {
	console.log("Registring new device with ID: " + req.body.id + "    Name: " + req.body.n + "     Key: " + req.body.k );
    db.public.none("INSERT INTO devices VALUES ('"+req.body.id+ "', '"+req.body.n+"', '"+req.body.k+"')");
	res.send("Device registered\n");
});

// Defines a route that handles HTTP GET requests to /web/device and displays a list of devices in an HTML table.
app.get('/web/device', function (req, res) {
	var devices = db.public.many("SELECT * FROM devices").map( function(device) {
		console.log(device);
		return '<tr><td><a href=/web/device/'+ device.device_id +'>' + device.device_id + "</a>" +
			       "</td><td>"+ device.name+"</td><td>"+ device.key+"</td></tr>";
	   }
	);
	res.send("<html>"+
		     "<head><title>Sensores</title></head>" +
		     "<body>" +
		        "<table border=\"1\">" +
		           "<tr><th>id</th><th>name</th><th>key</th></tr>" +
		           devices +
		        "</table>" +
		     "</body>" +
		"</html>");
});

/*
 * Canibalized from
 *    https://www.npmjs.com/package/sprightly
 *    https://github.com/obadakhalili/Sprightly/blob/main/index.js
 */
function render(template, vars) {
   const regexp = /<<(.*?)>>|\{\{(.*?)\}\}/;
   return template.split('\n').map( function(line) {
       for (let match = line.match(regexp), result; match;) {
	   if (match[0][0] === '<') {
		   console.log("match <");
	   } else {
	      result = vars[match[2].trim()];

	   }
           line = line.replace(match[0], result ? result : '');
	   match = line.match(regexp);
       }	       
       return line;
   }).join('\n');	
}

// Define a route that handles HTTP GET requests to /web/device/:id and displays a device's information in an HTML template.
app.get('/web/device/:id', function (req,res) {
    var template = "<html>"+
                     "<head><title>Sensor {{name}}</title></head>" +
                     "<body>" +
		        "<h1>{{ name }}</h1>"+
		        "id  : {{ id }}<br/>" +
		        "Key : {{ key }}" +
                     "</body>" +
                "</html>";


    var device = db.public.many("SELECT * FROM devices WHERE device_id = '"+req.params.id+"'");
    console.log(device);
    res.send(render(template,{id:device[0].device_id, key: device[0].key, name:device[0].name}));
});	


app.get('/term/device/:id', function (req, res) {
    var red = "\33[31m";
    var green = "\33[32m";
    var blue = "\33[33m";
    var reset = "\33[0m";
    var template = "Device name " + red   + "   {{name}}" + reset + "\n" +
		   "       id   " + green + "       {{ id }} " + reset +"\n" +
	           "       key  " + blue  + "  {{ key }}" + reset +"\n";
    var device = db.public.many("SELECT * FROM devices WHERE device_id = '"+req.params.id+"'");
    console.log(device);
    res.send(render(template,{id:device[0].device_id, key: device[0].key, name:device[0].name}));
});

app.get('/measurement', async (req,res) => {
    res.send(await getMeasurements());
});

app.get('/device', function(req,res) {
    res.send( db.public.many("SELECT * FROM devices") );
});

startDatabase().then(async() => {
    await insertMeasurement({id:'00', t:'18', h:'78'});
    await insertMeasurement({id:'00', t:'19', h:'77'});
    await insertMeasurement({id:'00', t:'17', h:'77'});
    await insertMeasurement({id:'01', t:'17', h:'77'});
    console.log("mongo measurement database Up");

    db.public.none("CREATE TABLE devices (device_id VARCHAR, name VARCHAR, key VARCHAR)");
    //db.public.none("INSERT INTO devices VALUES ('00', 'Fake Device 00', '123456')");
    //db.public.none("INSERT INTO devices VALUES ('01', 'Fake Device 01', '234567')");
    db.public.none("CREATE TABLE users (user_id VARCHAR, name VARCHAR, key VARCHAR)");
    //db.public.none("INSERT INTO users VALUES ('1','Ana','admin123')");
    //db.public.none("INSERT INTO users VALUES ('2','Beto','user123')");

    console.log("sql device database up");

    app.listen(PORT, () => {
        console.log(`Listening at ${PORT}`);
    });
});
