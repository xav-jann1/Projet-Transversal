function [S,f] = TransFourier(s,t)

% [S,f] = TransFourier(s,t)
% Transform?e de Fourier d'un signal s. 
% Input: 
% - s: vecteur de taille N contenant les N ?chantillons s[n] du signal ?
% analyser
% - t: vecteur de taille N contenant les instants d'echantillonnage de s.
% s[n] = s(t[n]).
% Outputs:
% - S : vecteur de taille N contenant les coeffeciens de la transform?e de
% Fourier du signal s
% - f : vecteur de taille N contenant les fr?quences correspondant aux
% coefficients de S : S[n] = S(f[n])

% PG: 2017

N = length(s) ;

switch nargin
    case 1
        t  = 1:N ; 
end

if N ~= length(t)
    error('Les vecteurs "s" et "t" doivent etre de meme longueur')
end
if std(diff(t)) > 1000*eps
    error('Le vecteur ''t'' doit etre lineairement croissant et a pas constant')
end

dt = t(2)-t(1) ; Fe = 1/dt ;
sshift = [s(t>=0) s(t<0)] ;

M = N ;
S = fft(sshift,M) ; 
S = fftshift(S) ;
S = S.*dt ;
f = linspace(-Fe/2,Fe/2,M+1) ; 
f = f(1:M) ;
