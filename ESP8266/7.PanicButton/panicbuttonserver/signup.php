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
    <link rel="stylesheet" href="assets/css/styleindex.css">
</head>

<body>
    <div class="login-box">
        <img class="avatar" src="assets/img/user.png" alt="Logo de joss">

        <?php if(!empty($message)): ?>
            <p> <?= $message ?></p>
        <?php endif; ?>

        <h1>Sign up</h1>

        <span> or <a href="index.php"> Log in </a></span>

        <form method="POST" action="signup.php">
            <!-- User name -->
            <label for="username">Mail</label>
            <input type="text" name="email" placeholder="Introduce your mail">
            <!-- password -->
            <label for="password">Password</label>
            <input type="password" name="password" placeholder="Introduce a password">

            <label for="password">Confirm your password</label>
            <input type="password" name="confirm_password" placeholder="Password must match">

            <input type="Submit" value="Registrate user">
        </form>
    </div>
</body>

</html>