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



void loop(){


}

struct FuzzyResult {
  float Hr;
  float Mr;
  float Lr;
};

FuzzyResult tinggiSungai(float cm_sungai) {
    float river_wl = cm_sungai;
    FuzzyResult resultSungai;

    if (river_wl <= 5) {
        resultSungai.Hr = 0;
        resultSungai.Mr = 0;
        resultSungai.Lr = 1;
    } else if (river_wl >= 8 && river_wl <= 14) {
        resultSungai.Hr = 0;
        resultSungai.Mr = 1;
        resultSungai.Lr = 0;
    } else if (river_wl >= 17) {
        resultSungai.Hr = 1;
        resultSungai.Mr = 0;
        resultSungai.Lr = 0;
    } else if (river_wl > 5 && river_wl < 8) {
        resultSungai.Hr = 0;
        resultSungai.Mr = (river_wl-5)/3;
        resultSungai.Lr = (8 - river_wl)/3;
    } else if (river_wl > 14 && river_wl < 17) {
        resultSungai.Hr = (river_wl - 14)/3;
        resultSungai.Mr = (17 - river_wl)/3;
        resultSungai.Lr = 0;
    }

    return resultSungai;
}

FuzzyResult tinggiKolam(float cm_kolam){
    float pool_wl = cm_kolam;
    FuzzyResult resultKolam;
    //====================== FUZZYFIKASI KETINGGIAN KOLAM ===================
    //=======================================================================
  if (pool_wl <= 5){
    resultKolam.Hp = 0;
    resultKolam.Mp = 0;
    resultKolam.Lp = 1;
    }
  else if (pool_wl >= 8 && pool_wl <= 14){
    resultKolam.Hp = 0;
    resultKolam.Mp = 1;
    resultKolam.Lp = 0;
    }
  else if (pool_wl >= 17){
    resultKolam.Hp = 1;
    resultKolam.Mp = 0;
    resultKolam.Lp = 0;
    }
  else if (pool_wl > 5 && pool_wl < 8){
    resultKolam.Hp = 0;
    resultKolam.Mp = (pool_wl-5)/3;
    resultKolam.Lp = (8 - pool_wl)/3;
    }
    
  else if ((pool_wl > 14) && (pool_wl <17)){
    resultKolam.Hp = (pool_wl - 14)/3;
    resultKolam.Mp = (17 - pool_wl)/3;
    resultKolam.Lp = 0;
    }
}

struct FuzzyRule {
  float narr_g
  float midd_g;
  float wid_g;
};

FuzzyRule rulesFuzzy(float cm_sungai, float cm_kolam){
    FuzzyRule resultRules;
    river_wl = cm_sungai;
    pool_wl  = cm_kolam;

    if ((river_wl <= 5)&&(pool_wl <= 5)){
        // X = min (Lr,Lp);
        resultRules.nar_g = min (Lr,Lp);
        resultRules.midd_g = 0;
        resultRules.wid_g = 0;
    }

    //================= THE RULE OF 1.2 ======================
    else if ((river_wl <= 5) && ((pool_wl > 5)&&(pool_wl < 8))){
        // X = min (Lr, Lp);
        // Y = min (Lr, Mp);
        resultRules.nar_g = min (Lr, Lp);
        resultRules.midd_g = Y = min (Lr, Mp);
        resultRules.wid_g = 0;
    }

    //================= THE RULE OF 1.3 ======================
    else if ((river_wl <= 5) && ((pool_wl >= 8) && (pool_wl <= 14))){
    
        // Y = min(Lr, Mp);
        resultRules.nar_g = 0;
        resultRules.midd_g = min(Lr, Mp);
        resultRules.wid_g = 0;
    }
    //================= THE RULE OF 1.4 ======================
    else if ((river_wl <= 5) && ((pool_wl > 14) && (pool_wl < 17))){
    
        // Y = min(Lr, Mp);
        // Z = min (Lr, Hp);
        resultRules.nar_g = 0;
        resultRules.midd_g = Y = min(Lr, Mp);
        resultRules.wid_g = Z = min (Lr, Hp);
    }

//================= THE RULE OF 1.5 ======================
    else if ((river_wl <= 5)&&(pool_wl >= 17)){
    
        // Z = min(Lr, Hp);
        resultRules.nar_g = 0;
        resultRules.midd_g = 0;
        resultRules.wid_g = min(Lr, Hp);
    }
  
//================= THE RULE OF 2.1======================
    else if (((river_wl > 5)&&(river_wl < 8)) && (pool_wl <= 5)){
    
        // X1 = min(Lr, Lp);
        // X2 = min(Mr, Lp);
        // X = max (X1, X2);
        resultRules.nar_g = max (min(Lr, Lp), min(Mr, Lp));
        resultRules.midd_g = 0;
        resultRules.wid_g = 0;
    }
//================= THE RULE OF 2.2======================
    else if (((river_wl > 5)&&(river_wl < 8)) && ((pool_wl > 5)&&(pool_wl < 8))){
    
        // X1 = min(Lr, Lp);
        // Y = min(Lr, Mp);
        // X2 = min (Mr, Lp);
        // X3 = min (Mr, Mp);
        // XX = max (X1, X2);
        // // XX = max ((max (X1, X2) , X3))
        // X = max (XX, X3);

        resultRules.nar_g = max(max (min(Lr, Lp), min (Mr, Lp)), min (Mr, Mp));
        resultRules.midd_g = min(Lr, Mp);;
        resultRules.wid_g = 0;
    }
//================= THE RULE OF 2.3======================
    else if (((river_wl > 5)&&(river_wl < 8)) && ((pool_wl >= 8)&&(pool_wl <= 14))){

        // Y = min(Lr, Mp);
        // X = min (Mr, Mp);
        resultRules.nar_g = min (Mr, Mp);
        resultRules.midd_g = min(Lr, Mp);
        resultRules.wid_g = 0;
    }
//================= THE RULE OF 2.4======================
    else if (((river_wl > 5)&&(river_wl < 8)) && ((pool_wl > 14)&&(pool_wl < 17))){
    
        // Y = min(Lr, Mp);
        // Z = min(Lr, Hp);
        // X1 = min(Mr, Mp);
        // X2 = min(Mr, Hp);
        // X = max(X1, X2);
        resultRules.nar_g = max(min(Mr, Mp), min(Mr, Hp));
        resultRules.midd_g = min(Lr, Mp);
        resultRules.wid_g = min(Lr, Hp);
    }
//================= THE RULE OF 2.5======================
    else if (((river_wl > 5)&&(river_wl < 8)) && (pool_wl >= 17)){
    
        // Z = min(Lr, Hp); 
        // X = min(Mr, Hp);
        resultRules.nar_g = min(Mr, Hp);
        resultRules.midd_g = 0;
        resultRules.wid_g = min(Lr, Hp); 
    }
  
//================= THE RULE OF 3.1======================
    else if (((river_wl >= 8)&&(river_wl <= 14)) && (pool_wl <= 5)){
    
        // X = min(Mr, Lp);
        resultRules.nar_g = min(Mr, Lp);
        resultRules.midd_g = 0;
        resultRules.wid_g = 0;
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
}