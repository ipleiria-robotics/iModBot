# ROS2 for Unity

Este projeto é baseado no projeto [UnityRos2](https://github.com/DynoRobotics/UnityRos2), tendo sido alterado para funcionar com o nó ROS2 criado no âmbito deste projeto (`ROS2 node - Stop_oncollision`).


## Utilização

- No docker:
 1. Abre o UnityHub.
	```bash
	$ cd unity && ./UnityHub.AppImage
	```
2. Ativa a licença do Unity.
3. Instala o editor mais recente.
4. Corre a simulação.

## Descrição Funcional

- Variáveis de entada utilizadas:

	- cmd_vel_SOC - `geometry_msgs/Twist`

- Variáveis de saída utilizadas:

	- col_flag - `std_msgs/Bool`


O tópico **cmd_vel_SOC** é responsável pelos dados necessários para o movimento do robô.
O tópico  **col_flag** é colocado com um valor lógico `true` sempre que for detetada uma colisão no mundo virtual.
  
Os valores recebidos no tópico **cmd_vel_SOC** são enviados pelo ROS2 Node - Stop_oncollision.
Os valores enviados no tópico **col_flag** são enviados para o ROS2 Node - Stop_oncollision.

