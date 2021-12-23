<?php
  session_start();

  if (isset($_SESSION['user_id']))
  {
    header('Location: main.php');
  }

  require 'database.php';

  if (!empty($_POST['email']) && !empty($_POST['password'])) {
    $records = $conn->prepare('SELECT id, email, password FROM users WHERE email = :email');
    $records->bindParam(':email', $_POST['email']);
    $records->execute();
    $results = $records->fetch(PDO::FETCH_ASSOC);

    $message = ' ';

    if (is_countable($results) > 0 && password_verify($_POST['password'], $results['password'])) {
      $_SESSION['user_id'] = $results['id'];
      header("Location: main.php");
    } else {
      $message = 'Credenciales invalidas';
    }
  }

?>
<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-8">
    <title>Login</title>
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="assets/css/style.css">
</head>

<body>
    <div class="login-box">
        <img class="avatar" src="assets/img/user.png" alt="Logo de joss">

        <?php if(!empty($message)): ?>
            <p> <?= $message ?></p>
        <?php endif; ?>

        <h1>Iniciar Sesión</h1>

        <form action="index.php" method="POST">
            <!-- User name -->
            <label for="username">Correo Electronico</label>
            <input type="text" name="email" placeholder="Correo Electronico">

            <!-- password -->
            <label for="password">Contraseña</label>
            <input type="password" name="password" placeholder="Ingresa tu contraseña">

            <!-- <input type="submit" value="Log In "> -->
            <input type="submit" value="Ingresar">
            <a href="forgotten.php">Olvidaste tu contraseña</a><br>
            <a href="signup.php">¿No tienes una cuenta?</a>
        </form>
    </div>
</body>

</html>