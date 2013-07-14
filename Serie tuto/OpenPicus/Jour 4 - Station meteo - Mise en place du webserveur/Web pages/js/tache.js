$(document).ready(function() {
 function stationmeteo()
 {
   $.ajax( {
            type: "GET",
            url: "status.xml",
            dataType: "xml",
            success: function(xml) 
                     {
                       $(xml).find('capteur').each(   
                         function()
                         {
                            var id = $(this).attr('id');
                            var type = $(this).find('type').text();
                            var title = $(this).find('title').text();
							var valeur = $(this).find('valeur').text();
                            $("#valeur-"+id).html(valeur);
                          });
                      },
			error: function (request, status, error) 
					{
						console.log('Erreur de chargement du xml');
				}
        });
  }
  setInterval(stationmeteo,1000);
});
   