# Clasificación de objetos a través del color.
Este ejemplo muestra cómo utilizar un conjunto de datos de un sensor para crear un modelo de aprendizaje profundo que clasifique diferentes objetos.
Se ha creado diferentes carpetas para cada placa ya que algunas librerías difieren entre placas.
## Características.
El ejemplo usa el sensor APDS9960 para capturar el color de diferentes objetos. Esto como una demostración de cómo podríamos diferenciar objetos de diferentes colores o materiales.
Esta idea se basa del ejemplo “Color Clasify”. Vemos una oportunidad de usar el TinyML para la clasificación de objetos.

Opciones como la programación convencional resulta más compleja para sistemas que se pueden ver afectados por factores externos así pues, factores como la iluminación podrían dar resultados diferentes en el color capturado durante transcurso del día.
Debido a esto se decidió crear un modelo de machine learning que busque reconocer colores de un objeto a través de un entrenamiento con una base de datos de colores que abarque todos esos cambios posibles.
Para desarrollar esta idea tomamos 4 objetos de diferentes colores y los pusimos en un lugar con una iluminación variable y controlada.
Los objetos son rollos de cinta de diferentes colores (Rojo, Azul, Amarillo, Verde). Se ha creado una base de datos para cada placa debido a que la placa Arduino Nano 33 Ble ya incorpora este sensor en su misma PCB y las demás placas usan un módulo separado de este sensor.

## Requisitos.
Para desarrollar este ejemplo debemos tener instalado algunas herramientas como son:

-	 [Librería para la tarjeta Arduino Nano 33 Ble Sense.](#Librería-para-la-tarjeta-Arduino-Nano-33-Ble-Sense)
-	[Librería para la tarjeta Esp32.](#Librería-para-la-tarjeta-Esp32)
-	[Librería para la tarjeta Raspberry Pi Pico.](#Librería-para-la-tarjeta-Raspberry-Pi-Pico)
-	[Coolterm.](#coolterm)
-	[Entorno de Google Colaboratory.](#Entorno-de-Google-Colaboratory.)
## Librería para la tarjeta Arduino Nano 33 Ble Sense.
Para realizar la instalación debemos abrir el IDE de Arduino y posteriormente remitirnos al gestor de tarjetas de este modo:
- Herramientas.
   - Placa.
	 - Gestor de tarjetas.
       
![Gestor1]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/gestor1.png)

Ya en el gestor de placas debemos buscar la librería como “Arduino Mbed OS Nano Boards”

![gestor]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/gestor.PNG)

Para corroborar que la placa se instaló correctamente procedemos a cargar un programa para un parpadeo de un led.
Primero debemos escoger la placa correspondiente de esta manera:

![escoger placa]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/escoger%20placa.png)

Luego de esto debemos escoger el ejemplo así:

![blink]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/blink.png)

Finalmente cargamos el ejemplo. Para esto debemos conectar la placa y presionar dos veces el botón de reset para que la placa entre en modo de programación.

Cuando la placa empieza a iluminar el led de color naranja procedemos a escoger el puerto que se le ha asignado a la placa

**Nota** este puerto cambio según el modo en que este la placa.

![puerto]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/puerto.png)

Por último, cargamos el ejemplo y debemos observar que el LED de la placa empezara a parpadear.


## Librería para la tarjeta Esp32.
Para la instalación de la placa Esp32 debemos ir a IDE de Arduino para luego ir a la parte de Archivo y seguido a Preferencias.

![preferencia](https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/preferencias.png)

En la parte de preferencias nos ubicamos en “Gestor de URLs adicionales de tarjetas”
Ahí debemos pegar el siguiente link https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json luego le damos aceptar y posteriormente debemos ir a la parte del gestor de tarjetas.

![preferencias2]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/preferencias2.PNG)

Ahí debemos buscar la librería como “esp32” y procedemos a instalar. Esto demora unos minutos.

![gestoresp32]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/gestoresp32.png)

Luego de la instalación debemos reiniciar el IDE para que todos los cambios se efectúen.
Para corroborar la instalación de la placa procedemos a cargar el ejemplo de blink. Debemos
Escoger la placa y realizar un cambio al programa ya que el pin que se conecta al led de la placa cambia según la versión de la placa, la marca y la referencia.

![escoger placa]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/escoger_esp32.png)

Para este ejemplo hemos usado la placa Esp32 llamada “Node MCU 32”. Esta trae el Led conectado de pin 2.

![cambio blink]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/cambio%20blink.PNG)

Realizado este cambio procedemos a cargar el programa y luego debemos ver el led parpadeando.

## Librería para la tarjeta Raspberry Pi Pico.
Para la instalación de esta placa debemos ir al gestor de tarjetas y hacer la búsqueda así” Arduino Mbed OS RP2040”

![gestor_pico]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/gestor_pico.PNG)

Luego de instalar la librería debemos escoger la placa de Raspberry pi pico en Arduino así:

![escoger pico]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/escoger_pico.png)

Así con la placa seleccionada procedemos a cargar un ejemplo de prueba, así que nos dirigimos a el ejemplo de blink y realizamos el siguiente cambio:

![cambio_blink_pico]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/cambio%20blink_pico.PNG)

Así con este cambio realizamos la compilación del ejemplo. Como la placa Raspberry Pi Pico no está asignada a ningún puerto COM la primera vez que la usamos, debemos cargar el ejemplo sin escoger el puerto. luego de que compile la placa ya será reconocida por el IDE de Arduino de este modo procedemos a seleccionar el puerto que le asignaron y volvemos a cargar el ejemplo.

![puerto_pico]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/puerto_pico.png)

Finalmente debemos ver que el led parpadea en la placa.

## coolterm.
La aplicación de coolterm es la herramienta que usaremos para realizar la captura de los datos. Esta aplicación es un tipo de terminal para el control del puerto serial, es gratuita y muy fácil de usar.
Para realizar la descarga debemos buscar en el navegador el nombre de “coolterm”.
Debemos escoger la siguiente opción:

![coolterm_pagina]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/coolterm_pagina.PNG)

Esta es la pagina oficial del desarrollador de la aplicación, cuando accedamos a la pagina debemos descender y encontrar las opciones de descarga.

![descargar_coolterm]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/descargar_coolterm.png)

El sistema operativo que estamos usando es Windows 10 de 64 bits así que escogemos esa opción. Cuando tengamos completado la descarga debemos tener un archivo comprimido, este archivo lo descomprimiremos. De esta manera tendremos una carpeta que contiene los archivos necesarios para el programa, asi que no debemos instalar nada.

![carpeta collterm]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/carpeta_coolterm.PNG)

Por ultimo debemos abrir la aplicación de coolterm que aparece en la carpeta, al abrir el programa tenemos la interfaz del programa como se muestra acontinuacion.

![vista coolterm]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/vista_coolterm.PNG) 


## Entorno de Google Colaboratory.

Para la ejecución de los códigos de Python que usamos para el diseño y entrenamiento de los modelos de machine learning, hacemos uso de la herramienta en línea de Google Colaboratory o también llamado “Colab”. 
La ventaja que trae usar Colab es que no necesitas descargar nada, solo desde el navegador puedes realizar toda la programación en un servidor remoto con altas prestaciones de cómputo.
Para usar esta herramienta partimos de que tenemos que poseer una cuenta de Gmail para registrarnos. Continuando, debemos ir a Google drive y cuando estemos en nuestra unidad procedemos a ir a la parte de “Nuevo”. Ahí debemos descender y buscar la opcion “Mas” y finalmente escojemos la opcion de “Google Colaboratory” de esta manera.

![colab1]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/colab1.png)

El aspecto de la pagina de colab es similar a esta:

![conectar_colab]( https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/Clasify_color/imagenes/conectar_colab.png)

Así pues, ya podemos crear nuestros códigos de Python, por último, debemos conectarnos al servidor para poder ejecutar las celdas.

