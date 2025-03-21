# 2024-2025-4GP-MelissandE46-SofiaManzueta

# Projet Capteur

## Sommaire 
*  Objectifs de ce projet
*  Diagramme du projet
*  Simulation du capteur graphite sur LTSpice
*  Shield PCB sur Kicad
*  Code Arduino
*  App Android
*  La Datasheet

## Objectifs de ce projet

<details>
<summary>  Création d'un capteur de contrainte en graphite  </summary>
   Nous allons utiliser les traces de crayon sur du papier comme une jauge de contrainte pour détecter les déformations en compression et en traction.<br/>
   Les mines de crayon standard sont composées de fines particules de graphite liées entre elles par des liants argileux. Lorsqu'on utilise un crayon, le frottement entre la mine et le papier entraîne le frottement de particules de graphite qui adhèrent aux fibres du papier. Ces traces de crayon peuvent être considérées comme de minces films conducteurs constitués de réseau de particules de graphite percolées sur le papier qui peuvent prendre des formes et des motifs arbitraires. L'expansion et la contraction de ce réseau de particules de graphite, induites par des contraintes mécaniques ou des interactions chimiques, devraient grandement affecter la qualité des contacts interparticulaires et donc le conductivité électrique globale. Il est important de noter que les mines de crayon plus dures contiennent une proportion plus élevée de liants argileux, alors que les mines plus molles contiennent une proportion plus grande de particules de graphite. Ces dernières permettent d'obtenir une résistance de base plus faible qu'avec les mines plus dures. Il a été observé que les variations de résistance moyennes étaient linéairement liées aux amplitudes moyennes de déflexion, avec des écarts d'environ 1%.<br/>
En utilisant la théorie de la percolation, la conductance totale G_{tot} d'un réseau de conducteurs hautement désordonné est :<br/>
   $$G_{tot}=G_0e^{-\xi_c}$$, ici $$\xi_c$$ est la valeur de $$\xi$$ au seuil de percolation qui est le point où un cluster connecté infiniment grand commence à émerger lorsque des paires de nanoparticules voisines choisie au hasard sont connectées par des conducteurs dans un ordre décroissant de valeurs de G<br/>
   If $$-\delta\lambda/2\leq\delta\lambda/2$$,<br/>
   $$\xi_c= \left( {\frac{2f_c\delta\lambda\delta\epsilon}{1-f_v}} \right)^{1/2}+\delta_M-\delta\lambda/2+\epsilon_M-\delta\epsilon/2$$ <br/>
   If $$\delta\lambda/2-\delta\epsilon/2\leq\delta\epsilon/2-\delta\lambda/2$$, <br/>
   $$\xi_c=\lambda_M+\epsilon_M-\left( \frac{1}{2}-\frac{f_c}{1-f_v} \right)\delta\epsilon$$<br/>
   *  $$f_c$$ est la fraction des conducteurs<br/>
   *  $$f_v$$ est la fraction de vide<br/>
   *  $$\lambda_M=L_M\beta$$ et $$E_M=E_{CM}/kT$$ sont les valeurs moyennes des distributions, avec $$L_M$$ l'espace moyen (hors vide) séparant les nanoparticules voisines et $$E_{CM}$$ l'énergie coulombienne de charge moyenne<br/>
   *  $$\delta\lambda$$ et $$\delta\epsilon$$ sont les largeurs des distributions
  
</details>

<details>
<summary>  Réalisation d'un shield PCB pour Arduino UNO  </summary>
   Nous allons créer un shield PCB afin de relier à l'arduino UNO :<br/>  
        *  Un écran OLED  <br/>
        *  Un potentiomètre digital en SPI   <br/>
        *  Un amplificateur transimpédance, lui-même relié au capteur graphite décrit plus haut et au potentiomètre digital  <br/>
        *  Un module Bluetooth  <br/>
        *  Un flex sensor  <br/>
        *  Un encodeur rotatoir auquel on ajoute un déparasite (une capacité entre la clock et le ground)   <br/>
  
</details>

<details>
<summary>  Création d'un code Arduino  </summary>
  - hola
</details>

<details>
<summary>  Élaboration d'une app Android  </summary>
  - hola
</details>

<details>
<summary>  Rédaction datasheet  </summary>
  - hola
</details>


## Diagramme du projet 
Image
<details>
<summary>  Etapes du projet :  </summary>
  
  - [x] Attribution des inputs de l'arduino aux différents composants
  - [X] Electronique Analogique
  - [x] Schématique et routage KiCad  
  - [] Impression du circuit
  - [] Percage
  - [] Montage du circuit
  - [] Code Arduino
  - [] App Android
  - [] Création du banc de test
  - [] Création de la datasheet
</details>


## Simulation du capteur graphite sur LTSpice 
* Création d'un capteur de contrainte en graphite
  - hola

## Code Arduino

## App Android

## La Datasheet




