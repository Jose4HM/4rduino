<?php
    require 'database.php';
?>
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Menu Principal</title>
    <link rel="stylesheet" href="assets/css/stylecircle.css">
</head>
<body>
    <!-- ---------------------------------------------------------------------- -->
    <?php if(!empty($user)): ?>
    <h1><center>Hola. <?= $user['email']?> Registro Exitoso
    </center></h1>
    <?php endif; ?>

    <div class="login-box-left">
        <img src="assets/img/dash.png" class="imagen">
        <a href="http://127.0.0.1:1880/ui">
        <h2>Menu Principal</h2>
        </a>
    </div>

    <div class="login-box-right">
        <img src="assets/img/dash.png" class="imagen">
        <a href="logout.php">
        <h2>Logout</h2>
        </a>
    </div>

    

    
    
</body>
</html>