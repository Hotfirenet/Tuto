exports.action = function(data, callback, config, manager){
	
	config = config.modules.stationMeteoOpenpicus;
	
	//On declare les variables
	var request = require('request'),
	xml2js = require('xml2js'),
	util = require('util'),
	parser = new xml2js.Parser(),
	url = 'http://'+ config.ipStationMeteoOpenpicus +':'+ config.port +'/status.xml',
	sensor = '',
	exprimeToi = '',
	msg ='',
	status = false,
	debug = false;
	
	request({ 'uri' : url }, function (err, response, body){

		if (err || response.statusCode != 200) {
		  callback({'tts': "L'action a échoué"});
		  return;
		}

		//On parse les donnees 
		parser.parseString(body, function (err, result) {
			if (err) {console.dir(err); console.log('mauvaises donnees dans xml'); callback(false);}
			else {		
				if(debug) {
					console.log(result.stationMeteo.capteur[0].title);
					console.log('2 - ' + data.feature);
					console.log('3 - ' + data.type);
				}
				
				//Temperature analogique
				if((data.feature == 'T') && (data.type == 'A')) {
					sensor = 'an1';
					exprimeToi = 'La température analogique est de %s degrès celsius';
				} 
				// Temperature digitale
				else if ((data.feature == 'T') && (data.type == 'D')) {
					sensor = 'd2';
					exprimeToi = 'La température digitale est de %s degrès celsius';					
				} 				
				// Temperature I2C
				else if ((data.feature == 'T') && (data.type == 'I2C')) {
					sensor = 'I2C2';
					exprimeToi = 'La température I2C est de %s degrès celsius';					
				} 				
				// luminosite
				else if ((data.feature == 'L')) {
					sensor = 'an2';
					exprimeToi = 'La luminosité est de %s %';					
				} 				
				// Gaz
				else if ((data.feature == 'G')) {
					sensor = 'an3';
					exprimeToi = 'La teneur en gaz est de %s ppm';					
				}  				
				// humidite
				else if ((data.feature == 'H')) {
					sensor = 'd1';
					exprimeToi = 'Le taux d\'humidité est de %s %';					
				}  				
				// pression
				else if ((data.feature == 'P')) {
					sensor = 'I2C1';
					exprimeToi = 'La pression est de %s méga pascal';					
				}  else {
					callback({'tts': 'Je ne trouve pas de capteur'});
					console.log('Je ne trouve pas de capteur');
				}					
				
				//on parcour le noeud xml
				if(sensor) {
					result.stationMeteo.capteur.forEach(function(info) { 
						if(debug) {
							console.log(info.title);
							console.log(info.valeur);				
						}
						//Si on trouve le capteur alors on renvoie l info a SARAH 
						if(sensor == info.$.id) {
							msg = util.format(exprimeToi, info.valeur);
							callback({'tts': msg});
							console.log(msg);
							//process.exit(0);
						}	
					});			
				}			
			}		
		}); //fin de parsage		
	});
}