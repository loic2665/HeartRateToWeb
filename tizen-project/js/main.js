
window.onload = function () {
    // TODO:: Do your initialization job


	var privileges = ["http://tizen.org/privilege/healthinfo",
	                  "http://tizen.org/privilege/power",
	                  "http://tizen.org/privilege/mediastorage"];

	for(var i = 0; i < privileges.length; i = i + 1){
		tizen.ppm.requestPermission(privileges[i], function(){
			// cool continue
		}, function(){
			alert("You must grant this privilege, otherwise the app won't work.");
			tizen.application.getCurrentApplication().exit();
		});
	}
	

    // add eventListener for tizenhwkey
    document.addEventListener('tizenhwkey', function(e) { 
    	
        if(e.keyName == "back"){
        	try {
                tizen.application.getCurrentApplication().exit();
            } catch (ignore) {}
        }
    });
    
    document.addEventListener("rotarydetent", function(event){
    	 if (event.detail.direction === "CW") { 

    		 window.scrollBy({
    			  top: 100,
    			  behavior: 'smooth'
    			});
    	 
    	 
    	 } else { 


    		 window.scrollBy({
    			  top: -100,
    			  behavior: 'smooth'
    			});
    	 
    	 }
    	 }, false);
    
};
