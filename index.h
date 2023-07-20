const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
.card{
    max-width: 400px;
     min-height: 250px;
     background: #02b875;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
</style>
<body>

<div class="card">
  <h2>The coordinates will update every 2 seconds</h4><br>
  <h2>HDOP:<span id="HDOPValue">0</span></h1><br>
  <h2>Lat :<span id="LatValue">0</span></h1><br>
  <h2>Long:<span id="LongValue">0</span></h1><br>
  <h4>Counter:<span id="CounterValue">0</span></h1><br>
</div>
<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getHDOP();
  getLat();
  getLong();
  getCounter();
}, 1000); // 1 Seconds update rate

function getHDOP() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("HDOPValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readHDOP", true);
  xhttp.send();
}

function getLat() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("LatValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readLat", true);
  xhttp.send();
}

function getLong() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("LongValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readLong", true);
  xhttp.send();
}

function getCounter() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("CounterValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readCounter", true);
  xhttp.send();
}

</script>
</body>
</html>
)=====";
