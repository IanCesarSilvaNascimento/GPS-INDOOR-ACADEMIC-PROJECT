int envio=0;
int incomingByte=0;
unsigned long timeout = 0;
int flag=0;
int rssi[26];
int start_cond[26];
int answer_cond[26];
int memoria[255];
int leitura=0;
float maior = -1;
int moda = 0;
int overflowtime = 0;
int teste = 0;

void(* resetFunc) (void) = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
if(flag==0||flag==2)
{
  if(flag==0)
  {
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
  }

  if(flag==2)
  {
    while(Serial2.read() != 0x7E)
    {
      timeout++;
      if(timeout >= 1000000)
      {
        Serial.println("Aguardando Autorização");
        timeout = 0;
        overflowtime++;
      }
      
      if(overflowtime >=3)
      {
        flag = 0;
        overflowtime = 0;
        resetFunc();
        break;
      }
    }
  }
    timeout = 0;
    start_cond[0] = 0x7E;
    for(int x=0;x<17;x++)
    {
      leitura= Serial2.read();
      while(leitura == 0xFFFFFFFF)
      {
        leitura= Serial2.read();
      }

      start_cond[x+1] = leitura;
    }
    if(start_cond[3] == 0x90)
    {
      if(start_cond[15] == 'A')
      {
        Serial.println("Autorização concedida!");
        overflowtime = 0;
        flag = 1;
      }
    }
}

//delay(5000);
if(flag==1){ 
  int comparador[70] = {0};
 for(int j=0;j<257;j++){
 enviaATDB();
while(Serial2.read() != 0x7E)
    {
      timeout++;
      if(timeout >= 100000)
      {
        Serial.println("Tempo de comunicação esgotado");
        Serial.println("Reinicie os dispositivos e/ou cheque alimentacoes e baterias");
        timeout = 0;
        flag = 0;
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
 if(j == 256)
 {
    flag=2;
 int maior = -1;
 for (int c =10; c<70; c++){
 if(comparador[c] > maior){
 maior = comparador[c];
 moda = c;
 }
 }
 Serial.println(" ");
 Serial.print("RSSI END/ROUTER (-dBm) = ");
 Serial.println(moda);
 Serial.print("Distancia END/ROUTER = ");
 Serial.print(db_to_meter(moda));
 Serial.println("m");

 Resposta_DB();
while(Serial2.read() != 0x7E)
{
  timeout++;
      if(timeout >= 1000000)
      {
        Resposta_DB();
        Serial.println("Reenviando Resposta_P1");
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
        Resposta_DB();
        Serial.println("Reenviando Resposta_P2");
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
        Resposta_DB();
        Serial.println("Reset do Arduino");
        resetFunc();

        timeout = 0;
      }
  }

 if(answer_cond[3] == 0x8B)
 {
  while(answer_cond[8] != 0x00)
  {
      Resposta_DB();
      Serial.println("Preso no Resposta_DB");
      resetFunc();

  }
  if(answer_cond[8] == 0x00)
  {
    Serial.println("Resposta enviada com sucesso!");
  }
 }
 }
}


}
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
 long sum = 0x17 + 0x01 + 0x13 + 0xA2 + 0x41 + 0xA2 + 0xF2 + 0x82 + 0xFF+ 0xFE + 0x02 + 0x64 + 0x62;
 Serial2.write(0xFF-(sum&0xFF));
 }

 void Resposta_DB(){
 Serial2.write(0x7E);
 Serial2.write((byte)0x0);
 Serial2.write(0x0F);
 Serial2.write(0x10);
 Serial2.write(0x01);
 Serial2.write((byte)0x0);
 Serial2.write(0x13);
 Serial2.write(0xA2);
 Serial2.write((byte)0x0);
 Serial2.write(0x41);
 Serial2.write(0xA2);
 Serial2.write(0xF2);
 Serial2.write(0x6B);
 Serial2.write(0xFF);
 Serial2.write(0xFE);
 Serial2.write((byte)0x0);
 Serial2.write((byte)0x0);
 Serial2.write(moda);
 long sum = 0x10 + 0x01 + 0x13 + 0xA2 + 0x41 + 0xA2 + 0xF2 + 0x6B + 0xFF+ 0xFE + moda;
 Serial2.write(0xFF-(sum&0xFF));
 }

 float db_to_meter(int rssi){
    const float db_1m = 31;
    const float prop_meio_x10 = 24.6;
    float meters = 0;

    meters = pow(10,((rssi-db_1m)/prop_meio_x10));
    return meters;
  }
