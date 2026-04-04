const char index_html[] PROGMEM = R"rawliteral(
<script>
	function updateTime() {
		var now = new Date();
		var xhr = new XMLHttpRequest();
		xhr.open("GET", "/changeTime?seconds=" + now.getSeconds() + "&minutes=" + now.getMinutes() + "&hours=" + now.getHours(), true);
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
		xhr.open("GET", "/deleteAlarm?index=" + index, true);
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