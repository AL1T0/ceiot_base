# Ejercicio CiberKillChain - Defensa

## Alumno

Alena Grebneva

## Enunciado

Desarrollar la defensa en función del ataque planteado en orden inverso. No es una respuesta a un incidente, hay que detectar el ataque independientemente de la etapa.

Para cada etapa elegir una sola defensa, la más importante, considerar recursos limitados.

## Resolución

### Etapa 7: Actions on Objectives
Defensa: detección de anomalías en las métricas y comportamiento del sistema.

- Implementar un sistema de monitoreo de seguridad en tiempo real que analice constantemente las métricas y el comportamiento del sistema, incluyendo las mediciones de CO2 y los datos de los sensores.
  - Utilizar herramientas como SIEM (Security Information and Event Management) para la detección de anomalías.
  - Configurar alertas que se activen cuando se detecten cambios inusuales en las métricas o comportamientos del sistema.
- Realizar análisis forenses regulares en los datos recopilados para identificar posibles manipulaciones.
- Mantener una copia de seguridad de los datos críticos en un entorno aislado y seguro que no esté directamente conectado al sistema principal.
- Utilizar firmas y patrones de malware conocidos para identificar y bloquear posibles amenazas.

### Etapa 6: Command & Control
Defensa: segmentación de la red y control de comunicaciones.

- Segmentar la red interna en zonas o VLANs para limitar la propagación lateral del malware en caso de una brecha de seguridad.
- Implementar reglas de firewall que restrinjan comunicaciones no autorizadas dentro de la red, permitiendo solo conexiones necesarias y autorizadas.
- Utilizar herramientas de detección de tráfico malicioso para identificar posibles comunicaciones no autorizadas o sospechosas.
- Monitorear y registrar todas las comunicaciones de red para su posterior análisis forense.
- Implementar soluciones de seguridad de endpoints que detecten y bloqueen intentos de comunicación no autorizada desde dispositivos comprometidos.

### Etapa 5: Installation
Defensa: gestión de acceso y control de tareas programadas.

- Implementar políticas estrictas de gestión de acceso a los sistemas, incluyendo autenticación de múltiples factores (MFA) para acceder a sistemas críticos.
- Revisar y limitar los permisos de tareas programadas y trabajos en sistemas Windows para evitar la ejecución no autorizada de código.
- Supervisar y auditar regularmente las tareas programadas en busca de anomalías o cambios no autorizados.
- Utilizar soluciones de gestión de identidades y accesos (IAM) para garantizar que solo los usuarios y sistemas autorizados tengan acceso a recursos críticos.

### Etapa 4: Exploit
Defensa: actualización continua.

- Implementar un programa de parcheo y actualización que mantenga todos los sistemas y software actualizados con las últimas correcciones de seguridad.
- Utilizar herramientas de escaneo de vulnerabilidades para identificar y priorizar en la infraestructura las conocidas.
- Configurar alertas para informar sobre vulnerabilidades críticas y aplicar parches de emergencia cuando sea necesario.
- Limitar la exposición de servicios y puertos innecesarios en la red para reducir la superficie de ataque.

### Etapa 3: Delivery
Defensa: filtros de correo electrónico y campaña de concientización.

- Realizar programas de formación y concientización sobre ciberseguridad para todos los empleados, con énfasis en la identificación de ataques de ingeniería social, como correos electrónicos de phishing.
- Implementar filtros de correo electrónico avanzados que detecten y bloqueen correos electrónicos de phishing y adjuntos maliciosos.
- Validar todas las actualizaciones de software y firmware antes de su implementación en los dispositivos embebidos.
- Mantener una lista blanca de fuentes confiables para las actualizaciones de software y firmware.

### Etapa 2: Weaponization
Defensa: protección contra malware.

- Implementar soluciones de seguridad de endpoints que utilicen análisis heurísticos y aprendizaje automático para detectar y bloquear malware desconocido.
- Configurar reglas de firewall que limiten la comunicación saliente hacia destinos no autorizados.
- Implementar soluciones de seguridad basadas en la nube que analicen el tráfico de red en busca de indicadores de compromiso (IoC) conocidos.
- Utilizar soluciones de detección y respuesta de endpoints (EDR) para la identificación temprana de comportamientos maliciosos en dispositivos.

### Etapa 1: Reconnaissance
Defensa: supervisión de la red y protección de la información.

- Establecer políticas de seguridad que limiten la cantidad de información sensible y contraseñas almacenadas en sistemas accesibles desde la red.
- Implementar un sistema de supervisión de red que detecte y alerte sobre actividades de escaneo de red inusuales.
- Utilizar técnicas de honeypot y señuelos para detectar escaneos y exploraciones maliciosas.
- Educar al personal sobre las buenas prácticas de seguridad y la importancia de no revelar información sensible en línea.
