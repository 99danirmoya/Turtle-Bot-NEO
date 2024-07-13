![Static Badge](https://img.shields.io/badge/stability-v1.0-3314333)

# <p align="center"> Turtle-Bot NEO </p>

<div align="center">

_Hecho con_

[![made-in-ArduinoIDE](https://img.shields.io/badge/Arduino_IDE-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)

## Robot con dos ruedas controladas por dos servos de rotación continua que cuenta con dos modos de funcionamiento: manual con joystick y sigue-líneas

</div>

<div align="justify">

___

## Modo de funcionamiento
1. Se selecciona entre los modos manual o sigue-líneas por medio del switch ON1-OFF-ON2
1. Se enciende el robot (conectándolo a la alimentación o dándole al botón de reset posteriormente)
1. Si se enciende en el modo sigue-líneas, simplemente colocar entre las líneas a seguir y el proceso será automático
   - ALTERNATIVAMENTE: Si se enciende en el modo manual, se deben calibrar los sensores de pre-colisión durante los intervalos de 2500ms para cada uno, indicados por el LED L
1. Ya en el modo manual, después del típico sonido de abrir un coche con la llave a distancia, se controla con el joystick (movimientos y bocina). Las intermitencias, pitido de marcha atrás y sistemas de pre-colisión son automáticos

___

</div>

## <p align="justify">  Lista de componentes </p>

<div align="center">

| Componente | Unidades |
| ------------- | ------------- |
| Arduino UNO | 1 |
| Protoboard | 1 |
| Chasis | 1 |
| Joystick | 1 |
| Switch ON1-OFF-ON2 | 1 |
| Fotoresistencia | 2 |
| Resistencia 10kohm | 2 |
| LED | 2 |
| Resistencia 220ohm | 2 |
| Piezo | 1 |
| Sensor IR | 2 |
| Servo de rotación continua | 2 |
| Ruedas | 2 |
| Jumpers | Los que hagan falta |

</div>

<div align="justify">

___

## Diagrama de conexiones

A continuación, se muestra el diagrama de conexiones del robot. Hacer clic en el botón morado de la imagen para acceder a la simulación de TinkerCAD.

</div>

<div align="center">
  <img src="https://github.com/99danirmoya/turtle-bot-neo/blob/main/pics/t725.png" width="750"  style="margin: 10px;"/>
  
  <em>Circuito del Turtle-Bot NEO</em>

  [![Static Badge](https://img.shields.io/badge/TINKER_THIS!-8A2BE2?logo=autodesk)](https://www.tinkercad.com/things/kZLjRCSkkIt-turtle-bot-neo)
</div>
<br/>

> [!CAUTION]
> Debido al número limitado de I/O de TinkerCAD, algunos elementos han sido sustituidos por las alternativas más parecidas, como el joystick por un botón y dos potenciómetros, y los sensores de infrarrojos, por sensores de proximidad (PIR)

<div align="justify">

___

## Modo de implementación

Toda la explicación del código de Arduino viene dada en el propio código, [`99danirmoya/turtle-bot-neo/turtle_bot_neo/turtle_bot_neo.ino`](https://github.com/99danirmoya/turtle-bot-neo/blob/main/turtle_bot_neo/turtle_bot_neo.ino), en formato de comentarios al lado de cada línea

[`PANEL A DISTANCIA`](https://4f566df1fed52c6e7fd5f661f64ae3eb.balena-devices.com/ui/#!/1?socketid=BDvahPaq06OiHTluAAA4)

___
___

## Ejemplo de montaje

</div>

<div align="center">
  <img src="https://github.com/99danirmoya/turtle-bot-neo/blob/main/pics/IMG_20240110_163731.jpg" width="750"  style="margin: 10px;"/>

  <em>Muestra de montaje y cableado</em>
</div>
<br/>

> [!NOTE]
> Esta imagen es únicamente una muestra orientativa. Lo razonable sería montar tanto el Arduino UNO como la protoboard sobre el chasis y distanciar el switch de tres posiciones y el joystick a modo de mando. El chasis está hecho en impresión 3D y vale cualquiera que venga preparado para servos y los sensores elegidos para cada modo

___

___

## <p align="justify"> Licencia </p>

Este proyecto está licenciado bajo la [GPL-3.0 license](https://github.com/99danirmoya/turtle-bot-neo?tab=GPL-3.0-1-ov-file).

___

## <p align="justify"> Contacto </p>

> [!IMPORTANT]
> Respondo amablemente a dudas y leo sugerencias: [![Gmail Badge](https://img.shields.io/badge/-Gmail-c14438?style=for-the-badge&logo=Gmail&logoColor=white&link=mailto:daniel.rodriguezm99@gmail.com)](mailto:daniel.rodriguezm99@gmail.com)
> 
> Más información sobre mis proyectos: [![Linkedin Badge](https://img.shields.io/badge/-LinkedIn-blue?style=for-the-badge&logo=Linkedin&logoColor=white&link=https://www.linkedin.com/in/daniel-rodr%C3%ADguez-moya-510a35167)](https://www.linkedin.com/in/daniel-rodr%C3%ADguez-moya-510a35167)

_<p align="justify"> Autor: Daniel Rodríguez Moya :shipit: </p>_
