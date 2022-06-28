# ROS2 Node - Stop on collision 

A função deste node é parar o robô tanto no mundo real como no mundo virtual quando detetada uma colisão.


## Utilização

 - No terminal do docker:
    ```bash
    $ cd work/ROS2_Workspace && ros2 run stop_oncollision oncollision
    ```

## Descrição Funcional 

- Variáveis de entrada:
	- cmd_vel -  `geometry_msgs/Twist`
	- col_flag -  `std_msgs/Bool`
		- col_flag = `true` - colisão detetada.
		- col_flag  = `false` - colisão não detetada.

- Variáveis de saída:
	- cmd_vel_SOC-  `geometry_msgs/Twist`
	
Sempre que o valor lógico da **col_flag** for `true` irá limitar os valores da mensagem **cmd_vel_SOC** permitindo apenas o envio do valor correspondente a andar para trás.

Os valores recebidos no tópico **cmd_vel** são enviados pelo controlo remoto.
Os valores recebidos no tópico **col_flag** são enviados tanto pelo robô real como pelo virtual.
Os valores enviados no tópico **cmd_vel_SOC** são recebidos pelo robô real e pelo virtual.