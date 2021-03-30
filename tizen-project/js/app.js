/* my stuff */


var id_interval = 0;
var host = "";
var delay = 500;
var HR = -1;




$("#btn_ip_screen").on("click", function(){
	
	if(localStorage.getItem("host") !== null){
		$("#HR_URL").val(localStorage.getItem("host"));
	}
	
	$("body").css("background-color", "#71BA51");
	$("#welcome_screen").css("display", "none");
	$("#ip_screen").css("display", "block");

});

$("#btn_ip_delay").on("click", function(){
	
	host = $("#HR_URL").val();
	
	if(localStorage.getItem("delay") !== null){
		$("#HR_DELAY").val(localStorage.getItem("delay"));
	}
	
	$("body").css("background-color", "#E75926");
	$("#ip_screen").css("display", "none");
	$("#delay_screen").css("display", "block");

});

$("#btn_start_send").on("click", function(){
	
	delay = $("#HR_DELAY").val();

	$("#HR_INFO_DELAY").text(delay);
	$("#HR_INFO_HOST").text(host);
	
	try{
		localStorage.setItem("host", host);
		localStorage.setItem("delay", delay);
	}catch(err){
		alert(err);
	}
	
	$("body").css("background-color", "#000000");
	$("#delay_screen").css("display", "none");
	$("#info_screen").css("display", "block");
	
	start();

});

$("#btn_stop_send").on("click", function(){

	end();
	
	$("body").css("background-color", "#3D8EB9");
	$("#info_screen").css("display", "none");
	$("#welcome_screen").css("display", "block");
	

});


function onchangedCB(hrmInfo) {
	HR = hrmInfo.heartRate;
	$("#HR_INFO_BPM").text(HR);
}

function getHR(){

	
    var d = new Date();
    var n = d.toLocaleTimeString();

	$.ajax({
		type: "POST",
		url: host,
		data: { rate: HR },
		success: function(){ $("#HR_DATE").text(n); $("#HR_ERROR").css("color", "grey"); },
		error: function(){ $("#HR_ERROR").text("Error on sending data (verify server)"); },
		dataType: "text"
	});
	
}



function start(){
	
	try{
    	tizen.humanactivitymonitor.start('HRM', onchangedCB);
	}catch(error){
		
		console.log(error);
		$("#HR_ERROR").text(error);
	}
	id_interval = setInterval(getHR, delay);
}

function end(){
	try{
    	tizen.humanactivitymonitor.stop('HRM');
    }catch(error){
		$("#HR_ERROR").text(error);
    }
	clearInterval(id_interval);
}
    