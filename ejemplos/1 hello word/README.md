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
![fff](https://github.com/udenarML/TinyML_Udenar/blob/main/ejemplos/1%20hello%20word/imagenes/seno%20ruido.png)
