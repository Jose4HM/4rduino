<?php
    echo "***Yo recibo datos*** <br>";
    $device=$_GET['device_label'];
    $temperature=$_GET['temperature'];
    $humedity=$_GET['humedity'];
    
    echo "Datos recibidos<br>";
    echo "{DEVICE:".$device. ",TEMP:".$temperature.", HUMI:".$humedity."}";


?>