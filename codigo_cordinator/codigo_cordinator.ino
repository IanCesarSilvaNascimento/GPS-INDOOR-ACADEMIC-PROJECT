int envio=0;
int incomingByte=0;
unsigned long timeout = 0;
int flag=0;
int rssi[26];
int valor_rssi_r1[26];
int start_cond[26];
int answer_cond[26];
int memoria[255];
int leitura=0;
int rssi_router1=0;
float maior = -1;
int moda = 0;
int overflowtime = 0;
int buf[26];
float dist_end_coord = 0.0;
float dist_end_router = 0.0;
float x_pos = 0.0;
float y_pos = 0.0;
const float dist_coord_router = 4.1;
void(* resetFunc) (void) = 0;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 Serial2.begin(115200);
}

void loop(){
  
 
  int comparador[70] = {0};
  if(flag == 0)
  {
  Serial.println("Enviando ATDB Para End-Device");
 for(int j=0;j<257;j++){
 enviaATDB();
         

while(Serial2.read() != 0x7E)
    {
      timeout++;
      if(timeout >= 1000000)
      {
        Serial.println("Tempo de comunicação esgotado");
        Serial.println("Reinicie os dispositivos e/ou cheque alimentacoes e baterias");
        timeout = 0;
      }
    }
    timeout = 0;
 rssi[0] = 0x7E;
 for(int x=0;x<19;x++){
 leitura= Serial2.read();
 while(leitura == 0xFFFFFFFF){
 leitura= Serial2.read();
 }

 rssi[x+1] = leitura;
}
 memoria[j] = rssi[18];
 comparador[rssi[18]]++;

 if(j == 256){
 int maior = -1;
 for (int c =10; c<70; c++){
 if(comparador[c] > maior){
 maior = comparador[c];
 moda = c;
 }
 }
 dist_end_coord = db_to_meter(moda);
 Serial.println(" ");
 Serial.print("RSSI END/COORD (-dBm) = ");
 Serial.println(moda);
 Serial.print("Distancia END/COORD = ");
 Serial.print(dist_end_coord);
 Serial.println("m");

 delay(200);

 Envia_Autorizacao_R1();
 Serial.println("Enviando Senha de Autorizacao Para Roteador");
 while(Serial2.read() != 0x7E)
    {
      timeout++;
      if(timeout >= 1000000)
      {
        Envia_Autorizacao_R1();
        Serial.println("Reenviando Autorizacao_P1");
        timeout = 0;
      }
    }
 
 answer_cond[0] = 0x7E;
 for(int x=0;x<11;x++){
 leitura= Serial2.read();
 while(leitura == 0xFFFFFFFF){
 leitura= Serial2.read();
 timeout++;
      if(timeout >= 1000000)
      {
        Envia_Autorizacao_R1();
        Serial.println("Reenviando Autorizacao_P2");
        timeout = 0;
      }
  }

 answer_cond[x+1] = leitura;
 }

 while(answer_cond[3] != 0x8B)
  {
        timeout++;
      if(timeout >= 1000000)
      {
        Envia_Autorizacao_R1();
        Serial.println("Reset do Arduino");
        resetFunc();
        timeout = 0;
      }
  }

 if(answer_cond[3] == 0x8B)
 {
  while(answer_cond[8] != 0x00)
  {
      Envia_Autorizacao_R1();
      Serial.println("Preso no Envia_Autorizacao_R1");
      resetFunc();

  }
  if(answer_cond[8] == 0x00)
  {
    Serial.println("Autorizacao enviada com sucesso!");
  }

}

    }
 }
 flag = 1;
}
if(flag == 1)
{
while(Serial2.read() != 0x7E)
{
}
  valor_rssi_r1[0] = 0x7E;
    for(int x=0;x<17;x++)
    {
      leitura= Serial2.read();
      while(leitura == 0xFFFFFFFF)
      {
        leitura= Serial2.read();
      }

      valor_rssi_r1[x+1] = leitura;
    }
    if(valor_rssi_r1[3] == 0x90)
    {
     // rssi_router1 = valor_rssi_r1[15]; 

        dist_end_router = db_to_meter(valor_rssi_r1[15]);
        Serial.println(" ");
        Serial.print("RSSI ROUTER/COORD (-dBm) = ");
        Serial.println(valor_rssi_r1[15]);
        Serial.print("Distancia END/ROUTER = ");
        Serial.print(dist_end_router);
        Serial.println("m");
        flag = 0;
    }
    
}
 x_pos = calc_pos_x(dist_end_coord,dist_end_router,dist_coord_router);
 y_pos = calc_pos_y(dist_end_coord,dist_end_router,dist_coord_router);
Serial.println(" ");
Serial.println(" ");
Serial.print("Posicao X: ");
Serial.print(x_pos);
Serial.print(" Posicao Y: ");
Serial.println(y_pos);
 
 zerarbuffer();
}
  
 void enviaATDB(){
 Serial2.write(0x7E);
 Serial2.write((byte)0x0);
 Serial2.write(0x0F);
 Serial2.write(0x17);
 Serial2.write(0x01);
 Serial2.write((byte)0x0);
 Serial2.write(0x13);
 Serial2.write(0xA2);
 Serial2.write((byte)0x0);
 Serial2.write(0x41);
 Serial2.write(0xA2);
 Serial2.write(0xF2);
 Serial2.write(0x82);
 Serial2.write(0xFF);
 Serial2.write(0xFE);
 Serial2.write(0x02);
 Serial2.write(0x64);
 Serial2.write(0x62); 
 long sum = 0x17 + 0x01 + 0x13 + 0xA2 + 0x41 + 0xA2 + 0xF2 + 0x82 + 0xFF + 0xFE + 0x02 + 0x64 + 0x62;
Serial2.write(0xFF-(sum&0xFF));
 
 }

 void Envia_Autorizacao_R1(){
  
  Serial2.write(0x7E);     // START DELIMITER  
  Serial2.write((byte)0x0);    // MSB LENGHT
  Serial2.write(0x0F);    // LSB LENGHT
  Serial2.write(0x10);    // API FRAME TYPE - TRANSMIT REQUEST 0X10
  Serial2.write(0x01);   // FRAME ID
  Serial2.write((byte)0x0); //  MSB 64 BIT DEST ADRESS
  Serial2.write(0x13); //   ''
  Serial2.write(0xA2); //   ''
  Serial2.write((byte)0x0); //   ''
  Serial2.write(0x41); //   ''
  Serial2.write(0xA2); //  '' 
  Serial2.write(0xF2); //  ''
  Serial2.write(0x93); // LSB 64 BIT DEST ADRESS - CONFIGURADO PARA BROADCAST 
  Serial2.write(0xFF); // MSB 16 BIT DEST. ADRESS
  Serial2.write(0xFE); // LSB 16 BIT DEST. ADRESS
  Serial2.write((byte)0x0); // 
  Serial2.write((byte)0x0); // 
  Serial2.write(0x41); // RF DATA "A" , SINAL DE START PARA A COMUNICAÇÃO DO ROUTER DE REFERÊNCIA.
 
  long sum = 0x10 + 0x01 + 0x13 + 0xA2 + 0x41 + 0xA2 + 0xF2 + 0x93 + 0xFF + 0xFE + 0x41;
   
  Serial2.write(0xFF-(sum&0xFF)); // CHECKSUM
    
 }

void zerarbuffer(){
  for(int i=0;i<26;i++){
        rssi[i]=0;
        buf[i]=0;
        valor_rssi_r1[i]=0;
        start_cond[i]=0;
        answer_cond[i]=0;
        memoria[i]=0;
    }

  }
  float db_to_meter(int rssi){
    const float db_1m = 31;
    const float prop_meio_x10 = 24.6;
    float meters = 0;

    meters = pow(10,((rssi-db_1m)/prop_meio_x10));
    return meters;
  }

  float calc_pos_x(float _end_coord,float _end_router,float _coord_router){
  
  float pos_x = 0.0;
  float cos_angle = 0.0;
  cos_angle = ((pow(_end_router,2)-pow(_end_coord,2)-pow(_coord_router,2))/(-2*_end_coord*_coord_router));
  
  pos_x = _end_coord*cos_angle;
  
  return pos_x;
 }

 float calc_pos_y(float _end_coord,float _end_router,float _coord_router){
 
  float pos_y = 0.0;
  float cos_angle = 0.0;
  float sin_angle = 0.0;
  cos_angle = ((pow(_end_router,2)-pow(_end_coord,2)-pow(_coord_router,2))/(-2*_end_coord*_coord_router));
  sin_angle = sqrt(1 - pow(cos_angle,2));

  Serial.println(cos_angle);
  Serial.println(sin_angle);
  
  pos_y = _end_coord*sin_angle;

  return pos_y;
 }
