<div role="main" id="yui_3_17_2_1_1602171073065_39"><span id="maincontent"></span><h2>Actividad 3: Cartoon</h2>
<div class="no-overflow" id="yui_3_17_2_1_1602171073065_41"><p>La idea es implementar el efecto de transformar a una imagen en un cartoon presentado en <a href="https://stacks.stanford.edu/file/druid:yt916dh6570/Dade_Toonify.pdf" target="_blank">este trabajo</a>. La implementación va a ser una simplificación,&nbsp;simplemente
    porque aún no vimos todos los temas.</p>
<p></p>
<ol>
    <li>Aplicar filtro de la mediana.<br>Utilizar un filtro de la mediana pequeño (siempre en relación a la imagen).<br><br></li>
    <li>Detectar bordes a la imagen en escala de grises.<br>El único filtro que vimos hasta ahora que sirve para detectar bordes es el Laplaciano (ya vamos a ver mejores).<br><i>El objetivo es obtener bordes negros, ¿qué operaciones habría que hacer para obtenerlos?<br><br></i></li>
    <li>Reducir la cantidad de colores en (1).<br>Si bien encontrar una paleta óptima para reducir la cantidad de colores de una imagen <a href="https://arxiv.org/abs/1101.0395" target="_blank" style="background-color: rgb(255, 255, 255); font-size: 0.9375rem;">no es una tarea trivial</a>
        <span style="font-size: 0.9375rem;">, en el trabajo original presentan una forma simple de hacerlo. También pueden usar alguna otra que se les ocurra.<br><br></span>
    </li>
    <li>Agregar a (3) los bordes obtenidos en (2)<br><i>¿Qué operación hay que realizar?</i></li>
</ol>
<p></p>
<p style="text-align: center;" id="yui_3_17_2_1_1602171073065_40"><b>Original</b></p>
<p style="text-align: center;"><img src="https://i.imgur.com/XKRWlas.png" alt="" width="502" height="408" role="presentation" class="img-responsive atto_image_button_text-bottom"><br></p>
<p style="text-align: center;"><b>Paso 1 - Paso 2</b></p>
<p style="text-align: center;"><img src="https://i.imgur.com/cqnKZTd.png" alt="" width="502" height="408" role="presentation" class="img-responsive atto_image_button_middle"><img src="https://i.imgur.com/AOY0u2Z.png" alt="" width="502" height="408" role="presentation" style="font-size: 0.9375rem;" class="img-responsive atto_image_button_middle"><br></p>
<p style="text-align: center;"><b>Paso 3 - Paso 4</b></p>
<p style="text-align: center;"><img src="https://i.imgur.com/EyRVd2H.jpg" alt="" width="502" height="408" role="presentation" class="img-responsive atto_image_button_middle"> <img src="https://i.imgur.com/urGjGUE.png" alt="" width="502" height="408" role="presentation" class="img-responsive atto_image_button_middle"></p></div>
