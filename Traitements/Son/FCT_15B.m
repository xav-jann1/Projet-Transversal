clear variables;
close all;


%% importation de l'échantillon sonore
%load handel.mat;
%sound(y,Fs);
%Fs=8192;

Fs=44100;

%y= audioread('Sinus_440Hz_4X_1s_1s_.wav');
%y= audioread('Sinus_440Hz_4X_1s_1s_0dB.wav');
%y= audioread('Sinus_440Hz_4X_1s_1s_3dB.wav');
%y= audioread('Sinus_440Hz_4X_1s_1s_10dB.wav');
%y= audioread('Sinus_440Hz_5s.wav');
%y= audioread('Sinus_440Hz_5s_0dB.wav');
%y= audioread('Sinus_440Hz_5s_3dB.wav');
%y= audioread('Sinus_440Hz_5s_10dB.wav');
%y= audioread('Sinus_440Hz_5X_200ms_500ms.wav');
%y= audioread('Sinus_440Hz_5X_200ms_500ms_0dB.wav');
%y= audioread('Sinus_440Hz_5X_200ms_500ms_3dB.wav');
%y= audioread('Sinus_440Hz_5X_200ms_500ms_10dB.wav');
%y= audioread('Sinus_523Hz_5s.wav');
%y= audioread('Sinus_523Hz_5s_10dB.wav');
%y= audioread('Sinus_880Hz_5s.wav');
%y= audioread('Sinus_880Hz_5s_10dB.wav');
%y= audioread('Sinus_1760Hz_5s.wav');
%y= audioread('Sinus_1760Hz_5s_10dB.wav');
%y= audioread('Sinus_1975Hz_5s.wav');
%y= audioread('Sinus_1975Hz_5s_10dB.wav');
%y= audioread('Carr_440Hz_5s_10dB.wav');
%y= audioread('Carr_440Hz_5s.wav');


%% definition des paramètres d'analyse
seuil = 0.8; % (0<seuil<1) valeur de signal au dela de laquelle on considère une émission plutôt qu'un silence.


%% calcul des caractéristiques de l'échantillon
N=length(y);
dureeSignal = N / Fs;
vecteurTemps=0:1/Fs:(N-1)/Fs;

%% calcul des durées d'emission et de silence
M=max(abs(y));
m=min(abs(y));
seuilActif = seuil*(M-m);
ecartMax = 100;
yFiltreMax=zeros(1,N);
for k=ecartMax+1:N-ecartMax-1
    yFiltreMax(k) = max(abs(y(k-ecartMax:k+ecartMax)));
end



i=0;
kActifs=zeros(1,15000);
nbreEchantActifs=0;
for k=1:N
    if (yFiltreMax(k)>seuilActif)
        i=i+1;
        kActifs(i)=k;
        nbreEchantActifs=nbreEchantActifs+1;
    end
end
dureeEmission = dureeSignal * nbreEchantActifs/N;
dureeSilence = dureeSignal * (1-(nbreEchantActifs/N));

disp(['Duree Emission :' num2str(dureeEmission)]);
disp(['Duree Silence :' num2str(dureeSilence)]);

%% analyse spectrale : calcul d'un spectre lissé judicieusement

[S,freq] = TransFourier(y,vecteurTemps);
energie = abs(S.^2);
freqPos=freq(ceil(N/2)+1:N-1);
energiePos=energie(ceil(N/2)+1:N-1);


masque=ones(1,150);
energieLiss = conv(energiePos,masque,'same');
energieLiss = conv(energieLiss,masque,'same');


%% recherche des maxima locaux du spectre lissé
[pks,locs] = findpeaks(energieLiss);
nbrPics=3; % nombre de composantes principales à rechercher.

composantesFreq=zeros(1,nbrPics);
valComposantesFreq=zeros(1,nbrPics);
for np = 1:nbrPics
      [valMax,i]=max(pks);
      indicePic = find(energieLiss==valMax);
      composantesFreq(np)=freqPos(indicePic);
      valComposantesFreq(np)=valMax;
      pks(i)=0;
end


epsFreq=0.05;
espEnergie = 0.05;
if   (abs(   composantesFreq(2)/composantesFreq(1) - 3   ) < epsFreq) && (abs(composantesFreq(3)/composantesFreq(1) - 5) < epsFreq)  && (abs(valComposantesFreq(1)/valComposantesFreq(2) - 9) < epsFreq)
    disp([ 'carré de fréquence : ' num2str(composantesFreq(1))]);
else
    disp([ 'sinusoïde de fréquence : ' num2str(composantesFreq(1))]);
end

