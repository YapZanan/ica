#define pintriger_sungai 32
#define pinecho_sungai   33
#define pintriger_kolam  23
#define pinecho_kolam    35

//pontesio
#define input_sungai 13
#define input_kolam  12

long durasi_sungai,durasi_kolam;   //nilai real pembacaan sensor
int cm_sungai, cm_kolam;         //konfersi nilai pembacaan sensor mentah ke cm

//===========================DECLARASI================================
float river_wl, pool_wl, gate_op;     //variabel fuzzifikasi dan defuzzifikasi
float Lr, Mr, Hr;                     //nilai linguistik river_wl (ketinggian sungai)
float Lp, Mp, Hp;                     //nilai linguistik pool_wl (ketinggian kolam) 
float nar_g, midd_g, wid_g;           //nilai linguistik gate_op (pembukaan gate)
float miu30, miu35, miu65, miu70;

float X, XX, Y, Z;
float X1, Y1, Z1;
float X2, Y2, Z2;
float X3, Y3, Z3;

float cogNar_g,  cogMidd_g,  cogWid_g;
float cxNar_g,   cxMidd_g,  cxWid_g;
float COG, cogA, cogB; 


  void setup(){
  Serial.begin(57600);

  pinMode (pintriger_sungai, OUTPUT);
  pinMode (pinecho_sungai,   INPUT);
  pinMode (pintriger_kolam,  OUTPUT);
  pinMode (pinecho_kolam,    INPUT);

  //potensio
  pinMode (input_sungai,    INPUT);
  pinMode (input_kolam,     INPUT);
}





void loop()
{

//  Blynk.run();
  
//memulai pembacaan ultrasonic sungai
  digitalWrite (pintriger_sungai, 1);  //memulai pantulan
  delayMicroseconds(10);
  digitalWrite (pintriger_sungai, 0);
  delayMicroseconds(2);
  
cm_sungai = 19;
  digitalWrite (pintriger_kolam, 1);
  delayMicroseconds(10);
  digitalWrite (pintriger_kolam, 0);
  delayMicroseconds(2);
  
cm_kolam = 1;

  if (cm_sungai <= 5 && cm_kolam <=5 )
    { 
      Serial.print(" sungai : ");
      Serial.print(cm_sungai);
      Serial.print("    kolam : ");
      Serial.print(cm_kolam);
      Serial.print("    gate : ");
      Serial.print(0);
      Serial.print("     ");

      delay(500);
      }
  else 
    {
          fuzzyPwm();
      Serial.print(" sungai : ");
      Serial.print(cm_sungai);
      Serial.print("    kolam : ");
      Serial.print(cm_kolam);
//      Serial.print("    gate : ");
//      Serial.print(COG);
//      Serial.print("     ");
//      
//      Serial.print(" narr_g : ");
//      Serial.print(nar_g);
//      Serial.print("    mid : ");
//      Serial.print(midd_g);
//      Serial.print("    wid : ");
//      Serial.print(wid_g);
//      Serial.print("     ");
//      
//      Serial.print("    miu30 : ");
//      Serial.print(miu30);
//      Serial.print("    miu35 : ");
//      Serial.print(miu35);
//      Serial.print("    miu65 : ");
//      Serial.print(miu65);
//      Serial.print("    miu70 : ");
//      Serial.print(miu70);
//      Serial.print("     ");
      
      delay(500);
  } 
}


void fuzzyPwm(){
  river_wl = cm_sungai;
  pool_wl  = cm_kolam;
  
//==================== FUZZYFIKASI KETINGGIAN SUNGAI ====================
//=======================================================================
 if (river_wl <= 5){
    Hr = 0;
    Mr = 0;
    Lr = 1;
    }
  else if (river_wl >= 8 && river_wl <= 14){
    Hr = 0;
    Mr = 1;
    Lr = 0;
    }
  else if (river_wl >= 17){
    Hr = 1;
    Mr = 0;
    Lr = 0;
    }
  else if (river_wl > 5 && river_wl < 8){
    Hr = 0;
    Mr = (river_wl-5)/3;
    Lr = (8 - river_wl)/3;
    }
    
  else if ((river_wl > 14) && (river_wl < 17 )){
    Hr = (river_wl - 14)/3;
    Mr = (17 - river_wl)/3;
    Lr = 0;
    }

//====================== FUZZYFIKASI KETINGGIAN KOLAM ===================
//=======================================================================
  if (pool_wl <= 5){
    Hp = 0;
    Mp = 0;
    Lp = 1;
    }
  else if (pool_wl >= 8 && pool_wl <= 14){
    Hp = 0;
    Mp = 1;
    Lp = 0;
    }
  else if (pool_wl >= 17){
    Hp = 1;
    Mp = 0;
    Lp = 0;
    }
  else if (pool_wl > 5 && pool_wl < 8){
    Hp = 0;
    Mp = (pool_wl-5)/3;
    Lp = (8 - pool_wl)/3;
    }
    
  else if ((pool_wl > 14) && (pool_wl <17)){
    Hp = (pool_wl - 14)/3;
    Mp = (17 - pool_wl)/3;
    Lp = 0;
    }

//====================== FUZZY RULE ======================

//=================== THE RULE OF 1.1 ====================
  if ((river_wl <= 5)&&(pool_wl <= 5)){
     
         X = min (Lr,Lp);
     nar_g = X;
    midd_g = 0;
     wid_g = 0;
  }
//================= THE RULE OF 1.2 ======================
 else if ((river_wl <= 5) && ((pool_wl > 5)&&(pool_wl < 8))){

         X = min (Lr, Lp);
         Y = min (Lr, Mp);
     nar_g = X;
    midd_g = Y;
     wid_g = 0;
  }
//================= THE RULE OF 1.3 ======================
 else if ((river_wl <= 5) && ((pool_wl >= 8) && (pool_wl <= 14))){
  
         Y = min(Lr, Mp);
     nar_g = 0;
    midd_g = Y;
     wid_g = 0;
  }
//================= THE RULE OF 1.4 ======================
 else if ((river_wl <= 5) && ((pool_wl > 14) && (pool_wl < 17))){
  
         Y = min(Lr, Mp);
         Z = min (Lr, Hp);
     nar_g = 0;
    midd_g = Y;
     wid_g = Z;
  }
//================= THE RULE OF 1.5 ======================
 else if ((river_wl <= 5)&&(pool_wl >= 17)){
  
         Z = min(Lr, Hp);
     nar_g = 0;
    midd_g = 0;
     wid_g = Z;
  }
  
//================= THE RULE OF 2.1======================
else if (((river_wl > 5)&&(river_wl < 8)) && (pool_wl <= 5)){
  
        X1 = min(Lr, Lp);
        X2 = min(Mr, Lp);
         X = max (X1, X2);
     nar_g = X;
    midd_g = 0;
     wid_g = 0;
  }
//================= THE RULE OF 2.2======================
else if (((river_wl > 5)&&(river_wl < 8)) && ((pool_wl > 5)&&(pool_wl < 8))){
  
        X1 = min(Lr, Lp);
         Y = min(Lr, Mp);
        X2 = min (Mr, Lp);
        X3 = min (Mr, Mp);
        XX = max (X1, X2);
       // XX = max ((max (X1, X2) , X3))
         X = max (XX, X3);
     nar_g = X;
    midd_g = Y;
     wid_g = 0;
  }
//================= THE RULE OF 2.3======================
else if (((river_wl > 5)&&(river_wl < 8)) && ((pool_wl >= 8)&&(pool_wl <= 14))){
  
         Y = min(Lr, Mp);
         X = min (Mr, Mp);
     nar_g = X;
    midd_g = Y;
     wid_g = 0;
  }
//================= THE RULE OF 2.4======================
else if (((river_wl > 5)&&(river_wl < 8)) && ((pool_wl > 14)&&(pool_wl < 17))){
  
         Y = min(Lr, Mp);
         Z = min(Lr, Hp);
        X1 = min(Mr, Mp);
        X2 = min(Mr, Hp);
         X = max(X1, X2);
     nar_g = X;
    midd_g = Y;
     wid_g = Z;
  }
//================= THE RULE OF 2.5======================
else if (((river_wl > 5)&&(river_wl < 8)) && (pool_wl >= 17)){
  
         Z = min(Lr, Hp); 
         X = min(Mr, Hp);
     nar_g = X;
    midd_g = 0;
     wid_g = Z;
  }
  
//================= THE RULE OF 3.1======================
else if (((river_wl >= 8)&&(river_wl <= 14)) && (pool_wl <= 5)){
  
         X = min(Mr, Lp);
     nar_g = X;
    midd_g = 0;
     wid_g = 0;
  }
//================= THE RULE OF 3.2======================
else if (((river_wl >= 8)&&(river_wl <= 14)) && ((pool_wl > 5)&&(pool_wl < 8))){
  
        X1 = min(Mr, Lp);
        X2 = min(Mr, Mp);
         X = max(X1, X2);
     nar_g = X;
    midd_g = 0;
     wid_g = 0;
  }
//================= THE RULE OF 3.3======================
else if (((river_wl >= 8)&&(river_wl <= 14)) && ((pool_wl >= 8)&&(pool_wl <= 14))){
  
         X = min(Mr, Mp);
     nar_g = X;
    midd_g = 0;
     wid_g = 0;
  }
//================= THE RULE OF 3.4======================
else if (((river_wl >= 8)&&(river_wl <= 14)) && ((pool_wl > 14)&&(pool_wl < 17))){
  
        X1 = min(Mr, Mp);
        X2 = min(Mr, Hp);
         X = (X1, X2);
     nar_g = X;
    midd_g = 0;
     wid_g = 0;
  }
//================= THE RULE OF 3.5======================
else if (((river_wl >= 8)&&(river_wl <= 14)) && (pool_wl >= 17)){
  
         X = min(Mr, Hp);
     nar_g = X;
    midd_g = 0;
     wid_g = 0;
  }

//================= THE RULE OF 4.1======================
else if (((river_wl > 14)&&(river_wl < 17)) && ((pool_wl <= 5))){
  
         X = min(Mr, Lp);
         Z = min(Hr, Lp);
     nar_g = X;
    midd_g = 0;
     wid_g = Z;
  }
//================= THE RULE OF 4.2======================
else if (((river_wl > 14)&&(river_wl < 17)) && ((pool_wl > 5)&&(pool_wl < 8))){
  
        X1 = min(Mr, Lp);
        X2 = min(Mr, Mp);
         Z = min(Hr, Lp);
         Y = min(Hr, Mp);
         X = max(X1, X2);
     nar_g = X;
    midd_g = Y;
     wid_g = Z;
  }
//================= THE RULE OF 4.3======================
else if (((river_wl > 14)&&(river_wl < 17)) && ((pool_wl >= 8)&&(pool_wl <= 14))){
  
         X = min(Mr, Mp);
         Y = min(Hr, Mp);
     nar_g = X;
    midd_g = Y;
     wid_g = 0;
  }
//================= THE RULE OF 4.4======================
else if (((river_wl > 14)&&(river_wl < 17)) && ((pool_wl > 14)&&(pool_wl < 17))){
  
        X1 = min(Mr, Mp);
        X2 = min(Mr, Hp);
         Y = min(Hr, Mp);
         Z = min(Hr, Hp);
         X = max(X1, X2);
     nar_g = X;
    midd_g = Y;
     wid_g = Z;
  }
//================= THE RULE OF 4.5======================
else if (((river_wl > 14) && (river_wl < 17)) && (pool_wl >= 17)){
  
         X = min(Mr, Hp);
         Z = min(Hr, Hp);
     nar_g = X;
    midd_g = 0;
     wid_g = Z;
  }

//================= THE RULE OF 5.1======================
else if ((river_wl >= 17) && (pool_wl <= 5)){
  
         Z = min(Hr, Lp);
     nar_g = 0;
    midd_g = 0;
     wid_g = Z;
  }
//================= THE RULE OF 5.2======================
else if ((river_wl >= 17) && ((pool_wl > 5)&&(pool_wl < 8))){
  
         Z = min(Hr, Lp);
         Y = min(Hr, Mp);
     nar_g = 0;
    midd_g = Y;
     wid_g = Z;
  }
//================= THE RULE OF 5.3======================
else if ((river_wl >= 17) && ((pool_wl >= 8)&&(pool_wl <= 14))){
  
         Y = min(Hr, Mp);
     nar_g = 0;
    midd_g = Y;
     wid_g = 0;
  }
//================= THE RULE OF 5.4======================
else if ((river_wl >= 17) && ((pool_wl > 14)&&(pool_wl < 17))){

         Y = min(Hr, Mp);
         Z = min(Hr, Hp);
     nar_g = 0;
    midd_g = Y;
     wid_g = Z;
  }
//================= THE RULE OF 5.5======================
else if ((river_wl >= 17) && (pool_wl >= 17)){

         Z = min(Hr, Hp);
     nar_g = 0;
    midd_g = 0;
     wid_g = Z;
  }

//========================================================
//               Defuzzifikasi                            
//========================================================



//nar_g      (5 - 25)    5 + 10 + 15 + 20 + 25 = 75/5
//p.nar_midd (30 - 35)   
//midd_g     (40 - 60)   40 + 45 + 50 + 55 + 60 = 250/5
//p.midd_wid (65 - 70)
//wid_g      (75 - 100)  75 + 80 + 85 + 90 + 95 + 100 = 525/6


cogNar_g  = 75;     cxNar_g  = 5;
cogMidd_g = 250;    cxMidd_g = 5;
cogWid_g  = 525;    cxWid_g  = 6;

int interval, nar_bB, nar_bA, midd_bB, midd_bA, wid_bB, wid_bA;

interval =  5;
nar_bB   =  0;    midd_bB = 25;   wid_bB =  60;
nar_bA   = 40;    midd_bA = 75;   wid_bA = 100;

float naik(float y, int a, int b){
  return (y*b)-(y*a)+a;
}
float turun(float y, int a, int b){
  return (y*a)-(y*b)+b;
}

for(x <= 4){
  if(naik){
    x1(y, a, b);
  }
  else if(turun){
    x2(y, a, b);
  }
}

sumA = 0; sumB = 0;
for (i=x1; i<=x2; i++) {
  sumA = sum+5;
  sumB = sum+1;
}



}
