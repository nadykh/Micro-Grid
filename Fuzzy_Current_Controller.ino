#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>

double I_ref;//reference current
double I_meas;//measured current
//define eI as two element vector [eI1 eI2];
double eI[2] = {0,0};
double D; //duty cycle
double dd; //duty cycle increment
int FB = 10; //Fuzzy Logic Error Band (after which hyst. control takes over)
int Ib = 1; //Hysterisis Current band I_ref +- Ib

Fuzzy* CC = new Fuzzy(); //Create new instance of the fuzzy controller called CC (current controller)

  //Deffine membership functions for IE
  FuzzySet* NE = new FuzzySet(-100,-100,-30,-10); //Negative Current Error
  FuzzySet* ZE = new FuzzySet(-20,0,0,20);        //Zero Current Error
  FuzzySet* PE = new FuzzySet(10,30,100,100);     //Positive Current Error

  //Define membership functions for dIE
  FuzzySet* NDE = new FuzzySet(-10,-10,-3,-1);    //Negative Current Derivative Error
  FuzzySet* ZDE = new FuzzySet(-2,0,0,2);         //Zero Current Derivative Error
  FuzzySet* PDE = new FuzzySet(1,3,10,10);        //Positive Current Derivative Error

  //Define membership functions for output d
  FuzzySet* No = new FuzzySet(-1,-1,-0.6,-0.2);   //Negative output 
  FuzzySet* Zo = new FuzzySet(-0.2,0,0,0.2);      //Zero output
  FuzzySet* Po = new FuzzySet(0.2,0.6,1,1);       //Positive output
  
void setup() {
  
  Serial.begin(9600);

  //***Define Fuzzy Inputs***
  
  //Current Error I_ref-I_meas
  FuzzyInput* IE = new FuzzyInput(1); // id=1

  //Add membership functions to Input
  IE->addFuzzySet(NE);
  IE->addFuzzySet(ZE);
  IE->addFuzzySet(PE);
 
  CC->addFuzzyInput(IE);                          //Add IE as an input for CC
  
  //Current Error Derivative IEt2-IEt1
  FuzzyInput* dIE = new FuzzyInput(2);
 
  //Add membership functions to input
  dIE->addFuzzySet(NDE);
  dIE->addFuzzySet(ZDE);
  dIE->addFuzzySet(PDE);

  CC->addFuzzyInput(dIE);                       //Add dIE as input for CC
//**End of Fuzzy Input Definition**

//**Fuzzy Output Definition**

  FuzzyOutput* d = new FuzzyOutput(1);            //Fuzzy Outout named d with id=1 d is the value by which the duty cycle has to be changed D=D+d where D is the duty cycle

  //Add membership functions to output
  d->addFuzzySet(No);                 
  d->addFuzzySet(Zo);
  d->addFuzzySet(Po);

  CC->addFuzzyOutput(d);                        //Add d as an Output for CC
//**End of Fuzzy Output Definition**

//**Definition of Fuzzy Rules**

  FuzzyRuleAntecedent* ifNN = new FuzzyRuleAntecedent();    //if IE negative and dIE negative
  ifNN->joinWithAND(NE,NDE);                                //link corresponding membership functions to Antecedent using AND 
 
  FuzzyRuleAntecedent* ifNZ = new FuzzyRuleAntecedent();    //if IE negative and dIE zero
  ifNZ->joinWithAND(NE,ZDE);                                //link with and
  
  FuzzyRuleAntecedent* ifNP = new FuzzyRuleAntecedent();    //if IE neagtive and dIE positive
  ifNP->joinWithAND(NE,PDE);                                //link

  FuzzyRuleAntecedent* ifZN = new FuzzyRuleAntecedent();    //if IE zero and dIE negative
  ifZN->joinWithAND(ZE,NDE);                                //link

  FuzzyRuleAntecedent* ifZZ = new FuzzyRuleAntecedent();    //if IE zero and dIE zero
  ifZZ->joinWithAND(ZE,ZDE);                                //link

  FuzzyRuleAntecedent* ifZP = new FuzzyRuleAntecedent();    //if IE zero and dIE positive
  ifZP->joinWithAND(ZE,PDE);                                //link

  FuzzyRuleAntecedent* ifPN = new FuzzyRuleAntecedent();    //if IE positive and dIE negative
  ifPN->joinWithAND(PE,NDE);                                //link

  FuzzyRuleAntecedent* ifPZ = new FuzzyRuleAntecedent();    //if IE positive and dIE zero
  ifPZ->joinWithAND(PE,ZDE);                                //link

  FuzzyRuleAntecedent* ifPP = new FuzzyRuleAntecedent();    //if IE positive and dIE negative
  ifPP->joinWithAND(PE,PDE);  
                                

  FuzzyRuleConsequent* thenO_N = new FuzzyRuleConsequent();  //then d negative
  thenO_N->addOutput(No);                                   //link corresponding output membership function to Consequent
  
  FuzzyRuleConsequent* thenO_Z = new FuzzyRuleConsequent(); //then d zero
  thenO_Z->addOutput(Zo);                                   //link
  
  FuzzyRuleConsequent* thenO_P = new FuzzyRuleConsequent(); //then d positive
  thenO_P->addOutput(Zo);                                   //link
  
  //Build Rules by combining anticidents and consequents
  
  FuzzyRule* fuzzyRule01 = new FuzzyRule(1,ifNN,thenO_N); //fuzzy rule named "fuzzyRule01" consistind of antecedent ifNN and consequent thenO_N with id=1
  FuzzyRule* fuzzyRule02 = new FuzzyRule(2,ifNZ,thenO_N);
  FuzzyRule* fuzzyRule03 = new FuzzyRule(3,ifNP,thenO_Z);
  FuzzyRule* fuzzyRule04 = new FuzzyRule(4,ifZN,thenO_N);
  FuzzyRule* fuzzyRule05 = new FuzzyRule(5,ifZZ,thenO_Z);
  FuzzyRule* fuzzyRule06 = new FuzzyRule(6,ifZP,thenO_P);
  FuzzyRule* fuzzyRule07 = new FuzzyRule(7,ifPN,thenO_Z);
  FuzzyRule* fuzzyRule08 = new FuzzyRule(8,ifPZ,thenO_P);
  FuzzyRule* fuzzyRule09 = new FuzzyRule(9,ifPP,thenO_P);
  
  CC->addFuzzyRule(fuzzyRule01);                        //Add rules to CC
  CC->addFuzzyRule(fuzzyRule02);                        //Add rules to CC
  CC->addFuzzyRule(fuzzyRule03);                        //Add rules to CC
  CC->addFuzzyRule(fuzzyRule04);                        //Add rules to CC
  CC->addFuzzyRule(fuzzyRule05);                        //Add rules to CC
  CC->addFuzzyRule(fuzzyRule06);                        //Add rules to CC
  CC->addFuzzyRule(fuzzyRule07);                        //Add rules to CC
  CC->addFuzzyRule(fuzzyRule08);                        //Add rules to CC
  CC->addFuzzyRule(fuzzyRule09);                        //Add rules to CC

  //**End of Fuzzy Logic Controller definition**//

}

void loop() {
  // put your main code here, to run repeatedly:
  CC->setInput(1,-58);
  CC->setInput(2,-5);
  CC->fuzzify();
  dd = CC->defuzzify(1);

  Serial.println(dd);
  delay(5000);

}
