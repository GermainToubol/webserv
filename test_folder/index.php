<!DOCTYPE html>
<html style="background-image: url('background.jpg'); background-size: cover;">
	<head>
		<title>Michel</title>
		<meta charset="utf-8">
		<link rel="stylesheet" href="css.css">
		<link rel="icon" href="monster.ico">
		<!-- include javascript -->
		
	</head>
	<body>
		<script src="script.js"></script>
		<h1>My Page</h1>
		<p><a href="https://youtube.com">youtube</a></p>
		<img src="monster.webp"/>
		<form action="/" method="POST" enctype="multipart/form-data">
			<div>
				<label for="msg" style="color:red;">Message :</label>
				<textarea id="msg" name="user_message"></textarea>
			</div>
			<div>
				<label for="msg" style="color:red;">Message :</label>
				<textarea id="msg" name="userlife"></textarea>
			</div>
			<div>
				<label for="msg" style="color:red;">Message :</label>
				<textarea id="msg" name="username"></textarea>
			</div>
			<input type="file" name="filename" />
			<input type="submit" value="OK!">
		</form>
		<audio src="audio.mp3" controls>
			O no pas d'audio
		</audio>
				
			<video id="vid1" width="320" height="240" controls preload="auto"">
			<source src="/video.mp4" type=video/mp4>
		  </video>
		  <video id="vid1" width="320" height="240" controls preload="auto"">
			<source src="/yeah.mp4" type=video/mp4>
		  </video>
		</body>
</html>
