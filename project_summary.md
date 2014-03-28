# Project Title
The Book Of Voices

## Authors
- [Masahide Yoshida](https://github.com/flatscape)
- [Naoki Sekiguchi](https://github.com/seckie)

## Description
"The Book Of Voices" is an interactive guest book using Google web speech API. When the visitor speeches to a microphone, their voice will be appeared in three dimensional texts on the screen. The texts will be inhaled to an actual notebook on the table then will be stored on the data base with its voice sound. When the book is opened, they are flowing out to the screen and the exhibition space will be surrounded by recorded voice sound.

## Tech Description

-When visitor's voice were input to a microphone, Web Speech API in Google Chrome will receive it and send data to openFrameworks application through Web Socket.
-The application saves visitor’s voices as audio files, and stores texts data to the MySQL database.
-Then the application draws texts data as three dimensional model, and sends location of the audio file to MAX patch via OSC message.
-The MAX patch reconstructs and play the audio data.
-When a visitor open the book, texts and audios stored in database will play.
-The texts will disappear when the book was closed.

## Images & Videos
https://www.youtube.com/watch?v=xaP_xQpfVCg
Sound track: Stilzand by Marco Raaphorst (http://freemusicarchive.org/music/Marco_Raaphorst/~/Stilzand)

![Project Image1](project_images/image1.jpg?raw=true "Project Image1")
![Project Image2](project_images/image2.jpg?raw=true "Project Image2")
