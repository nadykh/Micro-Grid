#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>

Fuzzy* fuzzy = new Fuzzy();

double inu = -0.3;
double fipo;
void setup() {
Serial.begin(9600);

//Fuzzy Input
FuzzyInput* eroro = new FuzzyInput(1); //id 1

FuzzySet* NB = new FuzzySet(-1,-1,-0.5,-0.125);
eroro->addFuzzySet(NB);

FuzzySet* NS = new FuzzySet(-0.4,-0.2,-0.2,0);
eroro->addFuzzySet(NS);

FuzzySet* Z = new FuzzySet(-0.05,0,0,0.05);
eroro->addFuzzySet(Z);

FuzzySet* PS = new FuzzySet(0,0.2,0.2,0.4);
eroro->addFuzzySet(PS);

FuzzySet* PB = new FuzzySet(0.125,0.5,1,1);
eroro->addFuzzySet(PB);

fuzzy->addFuzzyInput(eroro);
//End of Fuzzy Input definotion

//Fuzzy Output
FuzzyOutput* dKD = new FuzzyOutput(1);

FuzzySet* NBo = new FuzzySet(-1,-1,-0.6,-0.3);
dKD->addFuzzySet(NBo);

FuzzySet* NSo = new FuzzySet(-0.4,-0.2,-0.2,-0.01);
dKD->addFuzzySet(NSo);

FuzzySet* Zo = new FuzzySet(-0.01,0,0,0.0);
dKD->addFuzzySet(Zo);

FuzzySet* PSo = new FuzzySet(0.01,0.2,0.2,0.4);
dKD->addFuzzySet(PSo);

FuzzySet* PBo = new FuzzySet(0.3,0.6,1,1);
dKD->addFuzzySet(PBo);

fuzzy->addFuzzyOutput(dKD);
//End of Output definition

// tikotiko Fuzzy Rules

//Fuzzy Rule if eroro PB then dKD PB
FuzzyRuleAntecedent* ifEPB = new FuzzyRuleAntecedent();
ifEPB->joinSingle(PB);
FuzzyRuleConsequent* thendKPB = new FuzzyRuleConsequent();
thendKPB->addOutput(PBo);

FuzzyRule* fuzzyRule01 = new FuzzyRule(1,ifEPB,thendKPB);
fuzzy->addFuzzyRule(fuzzyRule01);
//

//FuzzyRule if eroro PS then dKD PS
FuzzyRuleAntecedent* ifEPS = new FuzzyRuleAntecedent();
ifEPS->joinSingle(PS);
FuzzyRuleConsequent* thendKPS = new FuzzyRuleConsequent();
thendKPS->addOutput(PSo);

FuzzyRule* fuzzyRule02 = new FuzzyRule(2,ifEPS,thendKPS);
fuzzy->addFuzzyRule(fuzzyRule02);
//

//FuzzyRule if eroro Z then dKD Z
FuzzyRuleAntecedent* ifEZE = new FuzzyRuleAntecedent();
ifEZE->joinSingle(Z);
FuzzyRuleConsequent* thendKZo = new FuzzyRuleConsequent();
thendKZo->addOutput(Zo);

FuzzyRule* fuzzyRule03 = new FuzzyRule(3,ifEZE,thendKZo);
fuzzy->addFuzzyRule(fuzzyRule03);
//

//FuzzyRule if eroro NS then dKD NS
FuzzyRuleAntecedent* ifENS = new FuzzyRuleAntecedent();
ifENS->joinSingle(NS);
FuzzyRuleConsequent* thendKNS = new FuzzyRuleConsequent();
thendKNS->addOutput(NSo);

FuzzyRule* fuzzyRule04 = new FuzzyRule(4,ifENS,thendKNS);
fuzzy->addFuzzyRule(fuzzyRule04);
//

//FuzzyRule if eroro NB then dKD NB
FuzzyRuleAntecedent* ifENB = new FuzzyRuleAntecedent();
ifENB->joinSingle(NB);
FuzzyRuleConsequent* thendKNB = new FuzzyRuleConsequent();
thendKNB->addOutput(NBo);

FuzzyRule* fuzzyRule05 = new FuzzyRule(5,ifENB,thendKNB);
fuzzy->addFuzzyRule(fuzzyRule05);
//

}

void loop() {
  // put your main code here, to run repeatedly:

  
  //inu = Serial.read();
  fuzzy->setInput(1,inu);

  fuzzy->fuzzify();


  fipo = fuzzy->defuzzify(1);
  Serial.println(fipo);
  delay(5000);
 
}
