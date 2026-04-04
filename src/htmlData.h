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