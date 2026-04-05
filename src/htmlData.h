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

	updateTime()
</script>

<body>
	hour:<input type="number" id="hours" name="hours" min="0" max="24">minute:<input type="number" id="minutes" name="minutes" min="0" max="60"><button type='button' onclick='addAlarm()'> Add Alarm</button>
</body>
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

		//Testing entry for css, REMOVE BEFORE DEPLOYING
		<p class='time'>7:31</p><button type='button' onclick='deleteAlarm(0)'>Delete</button>
		//End test entry

	</div>
</body>
<style>
	.centered {
		text-align:center
	}
</style>
)rawliteral";