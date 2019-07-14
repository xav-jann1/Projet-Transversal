close all;
clear variables;

%% image d'entrée

%im = imread('prise1.jpg');
%im = imread('prise2.jpg');
%im = imread('prise3.jpg');
%im = imread('prise4.jpg');
%im = imread('prise5.jpg');
%im = imread('prise6.jpg');
%im = imread('prise7.jpg');
%im = imread('prise8.jpg');
%im = imread('prise9.jpg');
%im = imread('prise10.jpg');
%im = imread('prise11.jpg');
%im = imread('prise12.jpg');
%im = imread('prise13.jpg');

inversee=0;
im = imresize(im,0.09);
im = im2double(im);
im = rgb2gray(im);
if(inversee==1)
    im = im';
end
im = histeq(im);

%% motifs
size_motif=378;

motifs=zeros(6,size_motif,size_motif);

for i=1:6
    if i==1, f='croix.png'; end
    if i==2, f='coeur.png'; end
    if i==3, f='carrenoir.png'; end
    if i==4, f='carrecreux.png'; end
    if i==5, f='trianglenoir.png'; end
    if i==6, f='cerclenoir.png'; end   
    m = imread(f);
    m = im2double(m);
    m = rgb2gray(m);
    m = imresize(m,[size_motif size_motif]);
    motifs(i,:,:)=m;
end

%% pré-traitements de l'image d'entrée 
mask_size=3;
H = (1/mask_size^2)*ones(mask_size,mask_size);
im=imfilter(im,H);

%% calcul des différents tableaux de corrélation pour des échelles différentes
valeur_pics=zeros(1,6);
scale_min=0.11;
scale_step=0.01;
scale_max = min(size(im))/size_motif;
nbr_scales=ceil((scale_max-scale_min)/scale_step);
tab_croix=zeros(6,3,nbr_scales); 
for i_motif=1:6
    disp(['motif ' num2str(i_motif) '/6']);
    croix=squeeze(motifs(i_motif, :, :));
    i=0;
    for scale=scale_min:scale_step:scale_max
        i=i+1;
        croix_scaled=imresize(croix,scale);
        corr_croix=normxcorr2(croix_scaled,im);
        corr_croix=imresize(corr_croix,size(im));
        [max_val,max_idx] = max(corr_croix(:)); %max_idx = index 1D
        tab_croix(i_motif,:,i)=[scale; max_val; max_idx];
    end
    tabtest=squeeze(tab_croix(i_motif,:,:));
    valeur_pics(i_motif)=max(tabtest(2,:));
end

%% interprétation des donnés des différents produits de corrélation
[~,motif_trouve]=max(valeur_pics);
disp(['trouvé le motif ' num2str(motif_trouve)])
tab_motif=squeeze(tab_croix(motif_trouve,:,:));

[~,idx_scale_trouve]=max(tab_motif(2,:));
scale_t = tab_motif(1,idx_scale_trouve);
pos_t = tab_motif(3,idx_scale_trouve);
size_motif_t = size(imresize(croix,scale_t));
[X,Y] = ind2sub(size(im),pos_t);

%% Affichage
figure(1);
imshow(im);
hold on;
r=ceil(max(size_motif_t)/2);
xx = [X-r, X+r, X+r, X-r, X-r];
yy = [Y-r, Y-r, Y+r, Y+r, Y-r];
plot(yy, xx, 'g-', 'LineWidth', 2);
title(['motif ' num2str(motif_trouve)])
