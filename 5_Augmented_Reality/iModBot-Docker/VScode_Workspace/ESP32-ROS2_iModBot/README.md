# ESP32_ROS2 - iModBot

Este é o código necessário para controlar e monitorizar o robô.
É responsável por controlar as rodas bem como ler, interpretar e enviar os valores dos vários sensores presentes.

## Utilização

1. Coloca o nome da tua rede wifi e a palavra pass.
2. Coloca o ip do agente (geralmente é o mesmo ip que o teu computador).
3. Guarda e faz upload para o ESP32.

## Descrição Funcional

- Variáveis de entada:

	- cmd_vel_SOC - `geometry_msgs/Twist`

- Variáveis de saída:

	- col_flag - `std_msgs/Bool`
	- US_sensor_dist - `std_msgs/Int16`
	- IR_board_sensors - `std_msgs/String`
	- Enc_count_RL  - `std_msgs/String`


O tópico **cmd_vel_SOC** é responsável pelos dados necessários para o movimento do robô.  
O tópico  **col_flag** é colocado com um valor logico `true` sempre que for detetado uma colisão no mundo virtual e real.  
O tópico  **US_sensor_dist** é onde é publicada a distancia medida pelo sensor de ultra sons.  
O tópico  **IR_board_sensors** é onde é publicado os valores dos vários sensores presentes na placa de infra vermelhos.  
O tópico  **Enc_count_RL** é onde é publicado o valor dos encoders, sendo que o primeiro valor é da roda direita e o segundo da roda esquerda.  
