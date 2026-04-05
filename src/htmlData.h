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

	updateTime()
</script>

<body class="centered">
	<h2>Add alarm:</h2>
	hour:<input type="number" id="hours" name="hours" min="0" max="24">minute:<input type="number" id="minutes"
		name="minutes" min="0" max="60"><button type='button' onclick='addAlarm()'> Add Alarm</button>
	<br />
	<h2>Change number color:</h2><br/>
	<input type="color" id="numcolor" value="#ff0000"><button type='button' onclick='updateNumColor()'> Submit</button><br/>
	<h2>Change separator color:</h2><br/>
	<input type="color" id="sepcolor" value="#ff0000"><button type='button' onclick='updateSepColor()'> Submit</button>
	<br/>
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
<body>
	<div class="centered">
		<h1>Current Alarms</h1>
		%ALARMS%
	</div>
</body>
<style>
	.centered {
		text-align:center
	}
</style>
)rawliteral";