const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <title>Ferrari F430</title>
    <meta content='width=device-width; initial-scale=1.0; maximum-scale=1.0; minimum-scale=1.0; user-scalable=no;' name='viewport'/>
    <style> 
       
    input[type=range]::-moz-range-thumb {
    width: 40px;
    height: 40px;
    background: black;
    } 
    
    input[type=range] {
    -webkit-appearance: none;
    height: 5px;
    background: lightgrey;
    outline: none;
    position:absolute;
    }
    
    input[type=range]::-webkit-slider-thumb {
    -webkit-appearance: none;
    width: 40px;
    height: 40px;
    border-radius: 5px;
    background: black;
    }
    </style>
  </head>
  <body>   
    <input type='range' ontouchend='resetX()' onmouseup='resetX()' min='61' max='101' value='81' id='sliderX' style='width:50%; left:5%; top:50%;'>
    <input type='range' ontouchend='resetY()' onmouseup='resetY()' min='-1323' max='1323' value='0' id='sliderY' style='width:40%; -webkit-transform:rotate(270deg); left:60%; top:50%;'>
      <script>
      
      function myFunction(){ 
        var x = document.getElementById("sliderX").value;
        var y = document.getElementById("sliderY").value;
        var xmlhttp=new XMLHttpRequest();
        xmlhttp.open("PUT","Val=X" + x + "Y" + y + "Z",true);
        xmlhttp.send();
      }
      setInterval(myFunction, 150);
      
      function resetX(){
        document.getElementById("sliderX").value = 81;  
      }
      
      function resetY(){
        document.getElementById("sliderY").value = 0;  
      }
      </script>
  </body>
</html>
)=====";