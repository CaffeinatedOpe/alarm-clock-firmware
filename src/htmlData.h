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

	function updateVolume() {
		var xhr = new XMLHttpRequest();
		xhr.open("GET", "/updateVolume?value=" + document.getElementById('volume').value, true);
		xhr.send();
	}

	updateTime()
</script>
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
<body style = "background-color: EDE8D0;">
	<div class="centered">
		<h1 style="font-family: fantasy;">Current Alarms</h1>
		%ALARMS%

	</div>
</body>
<style>
	.centered {
		text-align:center
	}
</style>
)rawliteral";