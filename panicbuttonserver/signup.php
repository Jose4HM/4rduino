<?php 
    require 'database.php';

    $message='';

    if(!empty($_POST['email']) && !empty($_POST['password']))
    {
        $sql= "INSERT INTO users (email, password) VALUES (:email, :password)";
        $stmt = $conn->prepare($sql);
        $stmt->bindParam(':email',$_POST['email']);
        $password=password_hash($_POST['password'], PASSWORD_BCRYPT);
        $stmt->bindParam(':password',$password);
        
        if($stmt->execute()){
            $message='Creación de usuario exitoso';
        }else{
            $message='Hubo un problema, intentalo luego.';
        }

    }
?>
<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-8">
    <title>Registrate</title>
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="assets/css/style.css">
</head>

<body>
    <div class="login-box">
        <img class="avatar" src="assets/img/user.jpg " alt="Logo de joss">

        <?php if(!empty($message)): ?>
            <p> <?= $message ?></p>
        <?php endif; ?>

        <h1>Registrate</h1>

        <span> o <a href="index.php"> Inicia sesión </a></span>

        <form method="POST" action="signup.php">
            <!-- User name -->
            <label for="username">Correo Electronico</label>
            <input type="text" name="email" placeholder="Ingresa tu correo">
            <!-- password -->
            <label for="password">Contraseña</label>
            <input type="password" name="password" placeholder="Ingresa contraseña">

            <label for="password">Confirma contraseña</label>
            <input type="password" name="confirm_password" placeholder="Confirma tu contraseña">

            <input type="Submit" value="Registrarse">
        </form>
    </div>
</body>

</html>