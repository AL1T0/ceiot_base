# Ejercicio CyberKillChain - Ataque
Se realizó una copia del documento original utilizándolo como plantilla para la realización del ejercicio.

## Alumno
Alena Grebneva

## Datos del proyecto a evaluar
El proyecto propone el desarrollo de un sistema de medición de niveles de CO2, temperatura y pH basado en IoT, para monitorear en tiempo real el funcionamiento de las unidades de Green Backbone. Utiliza un dispositivo ESP-32 con un conjunto de sensores y un módulo de relés para encender y apagar la unidad a distancia. Se utiliza MQTT para la transmisión y recepción de datos, utilizando la API de la plataforma AllThingsTalk como intermediario. La aplicación de usuario se despliega en una plataforma privada dentro de Microsoft Azure, empleando el software AVEVA System Platform.

+info: [Propuesta de Trabajo Final para la CEIoT](https://docs.google.com/document/d/1cM761QXQeOm4n0afF7hkzJ554GfDWqsnW6OZaDunL1Q/edit)

## Nota Importante 
Este plan de ataque es puramente hipotético y se proporciona con fines educativos y de concientización sobre la ciberseguridad. No debe ser utilizado para llevar a cabo actividades ilegales o dañinas. El acceso no autorizado a sistemas y redes está prohibido por la ley.

## Enunciado
Armar una cyberkillchain usando técnicas de la matriz de Att&ck para un escenario relacionado al trabajo práctico de la carrera.

### Instrucciones

Debe haber un objetivo para el ataque, algunas ideas inspiradoras:

* Obtener información con algún tipo de valor.
* Alguna variante extorsiva de ransomware.
* Usar de plataforma para atacar a otros, ya sea por ancho de banda, anonimización o como desplazamiento lateral hacia un objetivo más interesante.
* Usar la plataforma para extraerle valor como criptominado o almacenamiento de información ilegal.
* Sabotear la plataforma a favor de la competencia, tipo stuxnet.

El escenario debe asumir el sistema ya funcionando en el futuro.

Debe ser en primera persona, es el punto de vista del atacante.

Es recomendable hacer dos o tres pasadas, en la primera la idea, en las siguientes refinamientos especificando las técnicas.
PURO ATAQUE, nada de andar pensando cómo corregir nada.

Para cada etapa, si hay varias medidas posibles, ordenar dejando para lo último lo que se va a hacer en el siguiente paso.

### Cyberkillchain
Objetivo del ataque: obtener acceso y control sobre el sistema de monitoreo para robar información sobre la captura de CO2 y sabotear el funcionamiento del sistema, interrumpiendo el proceso de captura de CO2.

#### Etapa 1: reconnaissance
- Identifico el objetivo como el sistema de monitoreo.
- Investigo sobre el equipo técnico y empleados involucrados para encontrar posibles contraseñas o información de acceso.
  - T1593 - Search Open Websites/Domains (búsqueda de información pública)
- Realizo un análisis de la arquitectura del sistema para identificar posibles puntos de entrada.
  - T1595 - Active Scanning (escaneo activo de red)
- Escaneo los puertos y servicios expuestos para identificar vulnerabilidades conocidas.
  - T1046 - Network Service Discovery (escaneo de servicios de red)
   T1593.001 - Search Open-Source Information (búsqueda de información de código abierto).

#### Etapa 2: weaponization
- Puedo preparar un conjunto de herramientas y malware especializado en la explotación de Azure/Windows Server utilizados en el sistema de monitoreo.
  - T1203 - Exploitation for Client Execution (explotación para ejecución de cliente)
- Decido adquirir/desarrollar un malware personalizado que pueda infectar los componentes del sistema sin ser detectado.
  - T1055 - Process Injection (inyección de proceso)
  
#### Etapa 3: delivery
- Identifico un vector de entrega efectivo a las máquinas virtuales de Azure.
  - T1218 - System Binary Proxy Execution (ejecución de proxy binario)
- Utilizo una técnica de ingeniería social para persuadir a un empleado clave o técnico para que realice una actualización de software comprometida.
  - T1204 - User Execution (ejecución del usuario)
  
#### Etapa 4: exploit
- El malware se propaga dentro del sistema, aprovechando vulnerabilidades en el firmware o posibles debilidades en la configuración de Azure y Windows Server.
  - T0866 - Exploitation of Remote Services (explotación de servicios remotos).
  - T1049 - System Network Connections Discovery (descubrimiento de conexiones de red del sistema).
  
#### Etapa 5: installation  
- Mantenimiento del acceso persistente estableciendo tareas programadas en sistemas Windows para garantizar el acceso persistente.
  - T1053 - Scheduled Task/Job (tarea o trabajo programado).
  
#### Etapa 6: command & control
- Puedo aprovechar la infraestructura de Azure y Windows Server para ocultar la comunicación.
- Decido utilizar canales de comunicación cifrada específicos para Azure y Windows Server para evitar la detección.
  - T1001 - Remote Access Software (software de acceso remoto).
  - T1001.003 - Data Obfuscation: Protocol Impersonation (suplantación de protocolo).
  
#### Etapa 7: actions on objectives
- Ejecuto sabotaje manteniendo apagada la unidad para que no procese el aire del ambiente, mientras altero las métricas del sistema.
  - T1005 - Data from Local System (datos desde el sistema local): recopilación de datos sensibles.
  - T1491 - Defacement (desfiguración): modificación de valores de medición de CO2 y comando del equipo.
  - T1641 - Data Manipulation (manipulación de datos): cambio de datos históricos.
  - T1027 - Obfuscated Files or Information (archivos o información alterada).
