const char index_html[] PROGMEM = R"rawliteral(
<script>
	function updateTime() {
		var now = new Date();
		var xhr = new XMLHttpRequest();
		xhr.open("GET", "/changeTime?seconds=" + now.getSeconds() + "&minutes=" + now.getMinutes() + "&hours=" + now.getHours(), true);
		xhr.send();
	}

	function addAlarm() {
		var xhr = new XMLHttpRequest();
		xhr.open("GET", "/addAlarm?hours=" + document.getElementById('hours').value + "&minutes=" + document.getElementById('minutes').value, true);
		xhr.send();
	}

	function hexToRgb(hex) {
		var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
		return result ? {
			r: parseInt(result[1], 16),
			g: parseInt(result[2], 16),
			b: parseInt(result[3], 16)
		} : null;
	}

	function updateNumColor() {
		var xhr = new XMLHttpRequest();
		var color = hexToRgb(document.getElementById('numcolor').value);
		xhr.open("GET", "/updateNumberColor?r=" + color.r + "&g=" + color.g + "&b=" + color.b, true);
		xhr.send();
	}

	function updateSepColor() {
		var xhr = new XMLHttpRequest();
		var color = hexToRgb(document.getElementById('sepcolor').value);
		xhr.open("GET", "/updateDotColor?r=" + color.r + "&g=" + color.g + "&b=" + color.b, true);
		xhr.send();
	}

	function updateDisplayBrightness() {
		var xhr = new XMLHttpRequest();
		xhr.open("GET", "/updateScreenBrightness?value=" + document.getElementById('displayBrightness').value, true);
		xhr.send();
	}

	function updateRingBrightness() {
		var xhr = new XMLHttpRequest();
		xhr.open("GET", "/updateRingBrightness?value=" + document.getElementById('ringBrightness').value, true);
		xhr.send();
	}

	function updateRingColor() {
		var xhr = new XMLHttpRequest();
		var color = hexToRgb(document.getElementById('ringcolor').value);
		xhr.open("GET", "/updateRingColor?r=" + color.r + "&g=" + color.g + "&b=" + color.b, true);
		xhr.send();
	}

	updateTime()
</script>

<body class="centered" style ="background-color: beige;">
	<h2 style="font-family: 'Times New Roman', Times, serif;">Add alarm:</h2>
	Hour:<input type="number" id="hours" name="hours" min="0" max="24">Minute:<input type="number" id="minutes"
		name="minutes" min="0" max="60"><button type='button' onclick='addAlarm()'> Add Alarm</button>
	<br />
	<h2 style="font-family: 'Times New Roman', Times, serif;">Change number color:</h2><br/>
	<input type="color" id="numcolor" value="#ff0000"><button type='button' onclick='updateNumColor()'> Submit</button><br/>
	<h2 style="font-family: 'Times New Roman', Times, serif;">Change separator color:</h2><br/>
	<input type="color" id="sepcolor" value="#ff0000"><button type='button' onclick='updateSepColor()'> Submit</button><br/>
	<h2 style="font-family: 'Times New Roman', Times, serif;">Change display brightness:</h2>
	<input type="range" min="1" max="255" value="50" class="slider" id="displayBrightness"><button type='button' onclick='updateDisplayBrightness()'> Submit</button><br/>
	<h2 style="font-family: 'Times New Roman', Times, serif;">Change ring color:</h2><br/>
	<input type="color" id="ringcolor" value="#ff0000"><button type='button' onclick='updateRingColor()'> Submit</button><br/>
	<h2 style="font-family: 'Times New Roman', Times, serif;">Change ring brightness:</h2>
	<input type="range" min="1" max="255" value="50" class="slider" id="ringBrightness"><button type='button' onclick='updateRingBrightness()'> Submit</button><br/>
</body>
<style>
	.centered {
		text-align: center
	}
</style>
)rawliteral";

const char alarms_html[] PROGMEM = R"rawliteral(
<script>
	function deleteAlarm(index) {
		var now = new Date();
		var xhr = new XMLHttpRequest();
		xhr.open("GET", "/deleteAlarm?index=" + index, false);
		xhr.send();
		location.reload();
	}
</script>
<body style = "background-color: beige;">
	<div class="centered">
		<h1 style="font-family: 'Times New Roman', Times, serif;">Current Alarms</h1>
		%ALARMS%

		//Testing entry for css, REMOVE BEFORE DEPLOYING
		<p style="font-family: 'Times New Roman', Times, serif;" class='time'>7:31</p><button type='button' onclick='deleteAlarm(0)'>Delete</button>
		//End test entry

	</div>
</body>
<style>
	.centered {
		text-align:center
	}
</style>
)rawliteral";