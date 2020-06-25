
# Apresentação do Projeto 
Este projeto iModBot consiste num robô modular de baixo custo que pode ser programado através de liguagem de programação por blocos e/ou através de linguagem C/C++. Este projeto dispõe de uma biblioteca para facilitar o controlo do robô. 

A programação por blocos divide-se em 4 níveis, para facilitar a aprendizagem do utilizadores. Os níveis são os seguintes:
 - Nível Iniciante -> É possível colocar o robô a efetuar uma terefa com 1 a 2 blocos.
 - Nível Normal -> O utilizador tem mais controlo sob o movimento do robô, dispõe apenas da função loop.
 - Nível Médio -> O utilizador pode agora colocar blocos na função Setup e Loop.
 - Nível Avançado -> O utilizador dispõe de todas as funções da biblioteca e necessita de configurar todos os aspetos do robô.
    
![ROBOT](https://user-images.githubusercontent.com/61513539/82364254-f0e14d80-9a06-11ea-9d6f-fb408e07dd22.jpg)


# Elaboração do Projeto
**1º Passo do Projeto**

Para montar o robô basta seguir o ![documentos](https://github.com/ipleiria-robotics/iModBot/blob/master/3_Documentacao/4_Guia_de_montagem_offline.pdf) que mostra como montar ou então assistir o ![video](https://www.youtube.com/watch?v=i4wFh0GqzkM&feature=emb_logo). Para perceber melhor o que faz cada componente do robô pode ler o ![manual técnico](https://github.com/ipleiria-robotics/iModBot/blob/master/3_Documentacao/5_Manual_Tecnico_iModBot.pdf) que explica vários aspetos do robô. 

Lista de material:
 - ![ESP32 wroom32 devkit](https://user-images.githubusercontent.com/60508542/85723093-cf047600-b6ea-11ea-81e3-f72a01ec48b6.png) ;
 - l293d 4 channel module ;
 - 2x LM393 speed sensor ;
 - BFD-1000 5CH ;
 - HC-SR04 ;
 - I2C level shifter ;
 - TP4056 ;
 - MT3608 ;
 - li-ion rechargable battery 2500 mAh ;
 - 2x Breadboard 400 ;
 - Jumper wire male male, Jumper wire male female ;
 - 2wd smart robot kit .
 
 
**2º Passo do Projeto** 
 
 Downloads:
 
- Biblioteca  ![RobotOnLine.](https://github.com/ipleiria-robotics/iModBot/tree/master/4_Biblioteca)
            
- ArduinoIDE ![Arduino IDE](https://www.arduino.cc/en/Main/Software)
           
- ArduBlock ![java do ArduBlock](https://github.com/ipleiria-robotics/iModBot/blob/master/5_AduBlock/ardublock-beta-20200614.jar)

**3º Passo do Projeto**

Ler e seguir os tutoriais para configurar o Arduíno:
 - ![Configurar Arduíno IDE para o ESP32;](https://github.com/ipleiria-robotics/iModBot/blob/master/3_Documentacao/1_Como_comunicar_com_o_ESP32.pdf)

 - ![Configurar ArduBlock no Arduíno IDE;](https://github.com/ipleiria-robotics/iModBot/blob/master/3_Documentacao/2_Como_instalar_ArduBlock_no_Arduino_IDE.pdf)

 - ![Como instalar bibliotecas no Arduíno IDE.](https://github.com/ipleiria-robotics/iModBot/blob/master/3_Documentacao/3_Como_instalar_bibliotecas_no_Arduino_IDE.pdfl)

# Documentação

Após ter o Arduino IDE e o ArduBlock instalados pode usufruir dos tutoriais disponibilizados. Estes tutoriais ensinam a trabalhar com o robô sendo os mesmos divididos em duas categorias: tutoriais para ArduBlock e tutoriais para Arduino IDE.

**Documentação do ArduBlock**

Os ![4 níveis](https://github.com/ipleiria-robotics/iModBot/tree/master/1_Tutoriais/1_Tuturiais_para_ArduBlock) a baixo irão facilipar e guiar como deve de utilizar cada bloco e como creiara um programa.

- ![Nivel Iniciante;](https://github.com/ipleiria-robotics/iModBot/blob/master/1_Tutoriais/1_Tuturiais_para_ArduBlock/Tutorial%201%20-%20Nivel%20iniciante/1.%20Nivel%20iniciante.pdf)

- ![Nivel Normal;](https://github.com/ipleiria-robotics/iModBot/blob/master/1_Tutoriais/1_Tuturiais_para_ArduBlock/Tutorial%202%20-%20N%C3%ADvel%20Normal/2.%20N%C3%ADvel%20Normal.pdf)

- ![Nivel Médio;](https://github.com/ipleiria-robotics/iModBot/blob/master/1_Tutoriais/1_Tuturiais_para_ArduBlock/Tutorial%203%20-%20N%C3%ADvel%20M%C3%A9dio/3.%20N%C3%ADvel%20M%C3%A9dio.pdf)

- ![Nivel Avançado;]()

Depois de ler a documentação temos uma atividade para cada ![nivel](https://github.com/ipleiria-robotics/iModBot/tree/master/2_Atividades/1_para_ArduBlock).

**Documentação do Arduino IDE**

Depois de já ter realizado os tutoriais do ArduBock deverá ter uma noção de como programar no Arduino IDE, também elaboramos alguns tutoriais sugerindo que primeiro desse uma vista de olhos nos ![fluxogramas](https://github.com/SamueLourenc0/Ardublock/tree/master/02_TUTORIALS/5.Tutoriais%20para%20Arduino%20IDE/Recursos%20usados%20para%20elabora%C3%A7%C3%A3o%20dos%20tutoriais) que monstrao o funcionamenro do robô. 

- ![Tutorial 1 - Movimento;](https://github.com/ipleiria-robotics/iModBot/tree/master/1_Tutoriais/2_Tutoriais_para_Arduino_IDE/Tutorial%201%20-%20Movimento)

- ![Tutorial 2 - Sensores;](https://github.com/ipleiria-robotics/iModBot/tree/master/1_Tutoriais/2_Tutoriais_para_Arduino_IDE/Tutorial%202%20-%20Sensores)

- ![Tutorial 3 - Seguir trajeto.](https://github.com/ipleiria-robotics/iModBot/tree/master/1_Tutoriais/2_Tutoriais_para_Arduino_IDE/Tutorial%203%20-%20Seguir%20trajeto)

Depois de ler a documentação temos uma atividade para cada ![tutorial](https://github.com/ipleiria-robotics/iModBot/tree/master/2_Atividades/2_para_Arduino_IDE).


# Editar o ArduBlock

Caso tenha interesse em criar um bloco deve seguir os passos neste repositório ![Editar_ArduBlock_Eclipse.](https://github.com/SamueLourenc0/Editar_ArduBlock_Eclipse)

# Autores
 
 - Abel Teixeira   - estudante do Politécnico de Leiria do curso TeSP em Eletrónica e Redes de Telecomunicações ![Ab-Tx](https://github.com/Ab-Tx)
 - Samuel Lourenço - estudante do Politécnico de Leuria do curso TeSP em Automação, Robótica e manutenção Industrial ![SamueLourenc0](https://github.com/SamueLourenc0)
