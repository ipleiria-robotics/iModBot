
# Apresentação do Projeto 
Este projeto iModBot consiste em programar um robô de baixo custo por blocos. Para utilizadores que estão a iniciar a programar em linguagem C++.
Foi desenvolvido uma livraria no Arduíno IDE e implementamos blocos a partir dela. Com os vários blocos que criamos fizemos 4 níveis para facilitar a aprendizagem do utilizados. Os níveis são os seguintes:
 - Nível Iniciante -> Este nível o utilizador escolhe um bloco e arraste para o loop.
 - Nível Normal -> Este nível o utilizador já escolhe o caminho que o robô vai fazer arrentando blocos para o Loop conforme o pretenda.
 - Nível Médio -> Este nível é semelhante ao anterior mas só que neste nível já tem um Setup e um Loop.
 - Nível Avançado -> Este nível já tem de adicionar os pinos e a biblioteca no Setup e Loop.
    
![ROBOT](https://user-images.githubusercontent.com/61513539/82364254-f0e14d80-9a06-11ea-9d6f-fb408e07dd22.jpg)


# Elaboração do Projeto
**1º Passo do Projeto**

Para montar o robô basta seguir o ![documentos](https://github.com/ipleiria-robotics/iModBot/blob/master/3_Documentacao/4_Guia_de_montagem_offline.pdf) de mostra como montar ou então assistir o ![video](https://www.youtube.com/watch?v=i4wFh0GqzkM&feature=emb_logo). E para preceber melhor o que faz cada componente do robô pode ler o ![manuel tecnico](https://github.com/ipleiria-robotics/iModBot/blob/master/3_Documentacao/5_Manual_Tecnico_iModBot.pdf) que esplica cada componente. 

Lista de material:
 - Doit ESP 32
 - Módulo de controlo dos motores (L293D)
 - Encoder óticos (LM393)
 - Sensor de ultrassom (HC-SR04)
 - Módulo de cargo (TP4056)
 - Módulo elevador de tensão (MT3608)
 - Bateria de litio de iões  
 - Kit Carrinho Robot
 
 
**2º Passo do Projeto** 
 Downloads :
            - Livraria  ![RobotOnLine.](https://github.com/ipleiria-robotics/iModBot/tree/master/4_Biblioteca)
            
            - Programa ![Arduino IDE](https://www.arduino.cc/en/Main/Software)
           
            - Fixeiro ![java do ArduBlock]()

**3º Passo do Projeto**

Ler e seguir os tutorias para configurar o Arduíno:
 - ![Configura Arduíno IDE para o ESP32;](https://github.com/ipleiria-robotics/iModBot/blob/master/3_Documentacao/1_Como_comunicar_com_o_ESP32.pdf)

 - ![Configurar ArduBlock no Arduíno IDE;](https://github.com/ipleiria-robotics/iModBot/blob/master/3_Documentacao/2_Como_instalar_ArduBlock_no_Arduino_IDE.pdf)

 - ![Como instalar bibliotecas no Arduíno IDE.](https://github.com/ipleiria-robotics/iModBot/blob/master/3_Documentacao/3_Como_instalar_bibliotecas_no_Arduino_IDE.pdfl)

# Tutoriais 

Com o Arduíno IDE e o ArduBlock já instalados prontos para funcionar nós realizamos alguns tutorias para facilitar a aprendizagem ao utilizador.

**Tutoriais do ArduBlock**

Os ![4 nível](https://github.com/ipleiria-robotics/iModBot/tree/master/1_Tutoriais/1_Tuturiais_para_ArduBlock) a baixo iramo facilipar e guiar como deve de utilizar cada bloco e como creiara um programa.

- ![Nivel Iniciante;](https://github.com/ipleiria-robotics/iModBot/blob/master/1_Tutoriais/1_Tuturiais_para_ArduBlock/Tutorial%201%20-%20Nivel%20iniciante/1.%20Nivel%20iniciante.pdf)

- ![Nivel Normal;](https://github.com/ipleiria-robotics/iModBot/blob/master/1_Tutoriais/1_Tuturiais_para_ArduBlock/Tutorial%202%20-%20N%C3%ADvel%20Normal/2.%20N%C3%ADvel%20Normal.pdf)

- ![Nivel Médio;](https://github.com/ipleiria-robotics/iModBot/blob/master/1_Tutoriais/1_Tuturiais_para_ArduBlock/Tutorial%203%20-%20N%C3%ADvel%20M%C3%A9dio/3.%20N%C3%ADvel%20M%C3%A9dio.pdf)

- ![Nivel Avançado;]()

**Tutoriais do Arduino IDE**

Depois de já ter realizado os tutoriais do ArduBock já tá apto para começar a programa no Arduino IDE onde também elaboramos alguns tutoriais suguerindo que primeiro desse uma vista de olhos nos ![fluxogramas](https://github.com/SamueLourenc0/Ardublock/tree/master/02_TUTORIALS/5.Tutoriais%20para%20Arduino%20IDE/Recursos%20usados%20para%20elabora%C3%A7%C3%A3o%20dos%20tutoriais) que monstrao o funcionamenro do robô. 

- ![Tutorial 1 - Movimento;](https://github.com/ipleiria-robotics/iModBot/tree/master/1_Tutoriais/2_Tutoriais_para_Arduino_IDE/Tutorial%201%20-%20Movimento)

- ![Tutorial 2 - Sensores;](https://github.com/ipleiria-robotics/iModBot/tree/master/1_Tutoriais/2_Tutoriais_para_Arduino_IDE/Tutorial%202%20-%20Sensores)

- ![Tutorial 3 - Seguir trajeto.](https://github.com/ipleiria-robotics/iModBot/tree/master/1_Tutoriais/2_Tutoriais_para_Arduino_IDE/Tutorial%203%20-%20Seguir%20trajeto)

# Editar o ArduBlock

Se depois tiver interece de como se cria um bloco basta só segir os passo neste repositorio ![Editar_ArduBlock_Eclipse.](https://github.com/SamueLourenc0/Editar_ArduBlock_Eclipse)

# Autores
 
 - Abel Teixeira   - retrabalho- ![Ab-Tx](https://github.com/Ab-Tx)
 - Samuel Lourenço - retrabalho- ![SamueLourenc0](https://github.com/SamueLourenc0)
