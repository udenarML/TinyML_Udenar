# Hello world.
Este ejemplo se ha creado para la demostración del uso de la herramienta Tensorflow lite para ejecutar modelos de aprendizaje profundo en microcontroladores. Hemos desarrollado un cuaderno de júpiter que nos guía paso a paso en este ejemplo.

## Jupyter Notebook.
El ejemplo consiste en realizar el entrenamiento de un modelo de ml que pueda hacer predicciones de los datos de una onda seno.
Entonces el fujo de trabajo es el siguiente:
- Crear un conjunto de datos.
- Diseñar y entrenar un modelo de Tensorflow.
- Evaluación del desempeño.
- Convertir el modelo con Tensorflow lite.
- Implementar el modelo en Arduino.

### Crear un conjunto de datos.
Este ejemplo trata de obtener el valor de la función seno en un ángulo (x) especifico. A través del aprendizaje de máquina.

Asi pues debemos tener un conjunto de datos para entrenar este modelo. Hacemos uso de las librerías de Python para lograrlo.
#### Inclusion de librerias.
Importamos las librerias necesarias.
```
!pip install tensorflow
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import math
```
#### Generacion de datos.
En esta parte usaremos la librería de numpy y math para generar una onda seno. Esta señal no es continua si no que se ha creado a tramos en este caso 1000 que van desde [0 a 2π].
```
muestras=1000
semilla =1234
np.random.seed(semilla)
tf.random.set_seed(semilla)

x_valores = np.random.uniform(low=0, high=2*math.pi, size=muestras)
np.random.shuffle(x_valores)
y_valores = np.sin(x_valores)
plt.rcParams["figure.figsize"] = (10,5)
plt.plot(x_valores, y_valores,'b.')
plt.show()
```
![fff](https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/1%20hello%20word/imagenes/imagen%20seno.png)

#### Agregamos un poco de ruido.
Esto con el fin de corroborar la potencia y la eficacia de el modelo al tratar de predecir estos datos.
Se agregó una señal pequeña de ruido aleatorio.

```
y_valores += 0.1*np.random.rand(*y_valores.shape)
plt.rcParams["figure.figsize"] = (10,5)
plt.plot(x_valores, y_valores, 'b.')
plt.show()
```
![fff](https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/1%20hello%20word/imagenes/seno%20ruido.png)

#### Dividir los datos.
Para evaluar la precisión del modelo que entrenamos, necesitaremos comparar sus predicciones con datos reales y verificar qué tan bien coinciden. Esta evaluación ocurre durante el entrenamiento (donde se lo denomina validación) y después del entrenamiento (lo que se denomina prueba). En ambos casos, es importante que usemos datos nuevos que no se hayan usado para entrenar el modelo.

Para asegurarnos de tener datos para usar en la evaluación, reservaremos algunos antes de comenzar la capacitación. Reservaremos el 20 % de nuestros datos para la validación y otro 20 % para las pruebas. El 60% restante se utilizará para entrenar el modelo. Esta es una división típica utilizada cuando se entrenan modelos.
```
TRAIN_SPLIT = int(0.6 * muestras)
TEST_SPLIT = int(0.2 * muestras + TRAIN_SPLIT)

x_train, x_validate, x_test = np.split(x_valores,[TRAIN_SPLIT, TEST_SPLIT])
y_train, y_validate, y_test = np.split(y_valores,[TRAIN_SPLIT, TEST_SPLIT])

assert (x_train.size + x_validate.size + x_test.size)== muestras
plt.rcParams["figure.figsize"] = (10,5)
plt.plot(x_train, y_train, 'b.', label="Train")
plt.plot(x_validate, y_validate, 'y.', label="Validate")
plt.plot(x_test, y_test, 'r.', label="Test")
plt.legend()
plt.show()
```
![fff](https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/1%20hello%20word/imagenes/seno%20separado.png)
#### Diseñar y entrenar el modelo.

Vamos a construir un modelo que tomará un valor de entrada (en este caso, x) y lo usaremos para predecir un valor de salida numérico (el seno de x). Este tipo de problema se llama regresión.
Para lograr esto, vamos a crear una red neuronal simple. Utilizará capas de neuronas para intentar aprender cualquier patrón subyacente a los datos de entrenamiento, de modo que pueda hacer predicciones.
Para empezar, definiremos dos capas. La primera capa toma una sola entrada (nuestro valor x) y la ejecuta a través de 16 neuronas. Según esta entrada, cada neurona se activará hasta cierto punto en función de su estado interno (sus valores de peso y sesgo). El grado de activación de una neurona se expresa como un número.
Los números de activación de nuestra primera capa se alimentarán como entradas a nuestra segunda capa, que es una sola neurona. Aplicará sus propios pesos y sesgos a estas entradas y calculará su propia activación, que se generará como nuestro valor y.

**Nota:** Para obtener más información sobre cómo funcionan las redes neuronales, puede explorar los laboratorios de código Learn TensorFlow.
El código de la siguiente celda define nuestro modelo usando Keras, la API de alto nivel de TensorFlow para crear redes de aprendizaje profundo. Una vez definida la red, la compilamos, especificando parámetros que determinan cómo será entrenada:
```
from tensorflow.keras import layers
modelo1= tf.keras.Sequential()
modelo1.add(layers.Dense(16, activation='relu', input_shape=(1,)))
modelo1.add(layers.Dense(1))
modelo1.compile(optimizer='rmsprop', loss='mse', metrics=['mae'])
modelo1.summary()
```
![fff](https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/1%20hello%20word/imagenes/tabla%20arquitectura.PNG)

#### Entrenamiento.
Una vez que hemos definido el modelo, podemos usar nuestros datos para entrenarlo. El entrenamiento consiste en pasar un valor x a la red neuronal, comprobar en qué medida se desvía la salida de la red del valor y esperado y ajustar los pesos y sesgos de las neuronas para que sea más probable que la salida sea correcta la próxima vez.
El entrenamiento ejecuta este proceso en el conjunto de datos completo varias veces, y cada ejecución completa se conoce como una época. El número de épocas a ejecutar durante el entrenamiento es un parámetro que podemos configurar.
Durante cada época, los datos se ejecutan a través de la red en múltiples lotes. Cada lote, varias piezas de datos se pasan a la red, produciendo valores de salida. La corrección de estas salidas se mide en conjunto y los pesos y sesgos de la red se ajustan en consecuencia, una vez por lote. El tamaño del lote también es un parámetro que podemos establecer.

El código de la siguiente celda usa los valores x e y de nuestros datos de entrenamiento para entrenar el modelo. Funciona durante 800 épocas, con 16 piezas de datos en cada lote. También pasamos algunos datos para usarlos para la validación. 
```
history_1 = modelo1.fit(x_train, y_train, epochs=800, batch_size=16, validation_data=(x_validate, y_validate))
```

![fff](https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/1%20hello%20word/imagenes/tabla%20arquitectura.PNG)
