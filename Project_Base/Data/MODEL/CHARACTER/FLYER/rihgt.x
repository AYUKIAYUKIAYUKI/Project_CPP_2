xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 248;
 1.14991;0.87714;0.65335;,
 1.71705;1.89074;0.96224;,
 1.74338;1.76025;1.16313;,
 1.18440;0.77327;0.87770;,
 2.17823;2.64008;1.23017;,
 2.13638;2.51025;1.38483;,
 1.96320;2.88008;1.20062;,
 1.44392;2.13762;0.90106;,
 2.99370;3.44921;1.74253;,
 3.22813;3.37010;1.89864;,
 3.09584;3.19845;1.95625;,
 2.91018;3.21044;1.64235;,
 4.73729;3.43071;2.80545;,
 4.64185;3.27720;2.86326;,
 3.63995;3.26713;2.27592;,
 3.67413;3.41866;2.16319;,
 4.71595;3.56229;2.85347;,
 3.61954;3.55652;2.20010;,
 5.93586;3.33667;3.58488;,
 5.93385;3.21748;3.53833;,
 5.84724;3.08120;3.57717;,
 7.31200;2.73206;4.42423;,
 7.17492;2.77471;4.31468;,
 7.20201;2.85924;4.35158;,
 7.32276;2.77655;4.44073;,
 7.06549;2.67223;4.33575;,
 7.33176;2.71304;4.48947;,
 7.24447;2.66959;4.43907;,
 4.66138;3.61790;2.94399;,
 3.54717;3.61019;2.30086;,
 5.89571;3.39216;3.65737;,
 3.28294;3.54247;2.00024;,
 2.62634;3.34232;1.48176;,
 2.66061;3.03448;1.70023;,
 2.47930;3.08339;1.42411;,
 7.30862;2.78301;4.47611;,
 2.27294;3.27692;1.39054;,
 2.37512;2.81839;1.52507;,
 5.26727;0.32780;3.19620;,
 5.11663;0.26722;3.19553;,
 4.66285;1.10781;2.88805;,
 4.83369;1.18746;2.89411;,
 5.34171;0.38959;3.32541;,
 4.92215;1.26276;3.03756;,
 2.42579;-1.18265;1.64967;,
 2.37105;-1.14691;1.61808;,
 2.45690;-1.14621;1.60814;,
 5.28621;0.18276;3.29380;,
 5.21844;0.18063;3.25458;,
 5.29932;0.21082;3.24662;,
 5.32866;0.24322;3.31819;,
 2.48991;-1.14352;1.68669;,
 2.56880;-0.99309;1.73224;,
 2.49311;-0.99849;1.59186;,
 2.61278;0.03496;1.59952;,
 2.70706;0.03930;1.75918;,
 2.33108;-0.99988;1.59500;,
 2.42616;0.02862;1.59617;,
 2.69226;1.31581;1.59530;,
 2.48595;1.29695;1.61179;,
 2.45000;2.39914;1.57928;,
 2.65680;2.48312;1.54483;,
 2.77961;1.32347;1.78590;,
 2.74636;2.48231;1.75779;,
 4.26523;2.26701;2.65037;,
 4.19218;2.17574;2.49160;,
 3.33817;2.91876;1.96676;,
 3.42026;2.99895;2.15273;,
 4.02230;2.07198;2.50970;,
 3.18944;2.77602;2.01797;,
 4.26958;0.24022;2.71193;,
 4.04274;1.00446;2.52952;,
 3.28301;0.64647;2.08532;,
 3.28976;-0.16845;2.15005;,
 3.32881;1.74329;2.10665;,
 2.17521;3.30955;1.50856;,
 1.82412;2.92703;1.30586;,
 7.18797;2.90387;4.40646;,
 3.22608;3.58942;2.11537;,
 0.81844;1.16110;0.54311;,
 4.51907;2.77153;2.79354;,
 5.58554;1.36375;3.47084;,
 5.07167;1.93970;3.12270;,
 0.61423;-0.08379;0.35417;,
 0.23551;0.23510;0.20541;,
 0.59752;-0.11201;0.59754;,
 2.09210;1.54905;1.37469;,
 2.04503;0.46491;1.37353;,
 1.19703;-0.25262;0.94305;,
 1.60930;0.67778;1.12190;,
 0.37192;-0.19502;0.30618;,
 0.16153;-0.02786;0.20017;,
 0.38229;-0.25120;0.49036;,
 0.00000;0.04573;0.26985;,
 0.04397;0.32005;0.27810;,
 0.64306;1.23327;0.62397;,
 2.36095;4.29427;1.61581;,
 2.40628;4.27962;1.55442;,
 2.35486;4.20196;1.50128;,
 2.26945;4.21138;1.56297;,
 5.44844;2.64655;3.34175;,
 6.31316;2.32109;3.89857;,
 1.81107;-0.53105;1.29642;,
 2.19597;3.37325;1.52054;,
 2.30619;3.37899;1.43236;,
 2.64830;4.18990;1.78169;,
 2.65817;4.16437;1.68655;,
 2.51205;4.25814;1.60836;,
 2.48849;4.28445;1.68942;,
 2.21336;4.04714;1.53059;,
 2.31354;4.04762;1.47335;,
 2.30144;3.80212;1.45611;,
 2.18565;3.79874;1.51459;,
 2.17580;3.37859;1.66073;,
 2.19328;3.80198;1.64433;,
 2.18565;3.79874;1.51459;,
 2.19597;3.37325;1.52054;,
 3.17110;3.62844;1.94947;,
 3.14179;3.64146;2.06666;,
 2.49575;4.05496;1.47359;,
 2.55640;3.81048;1.46593;,
 2.41697;4.09261;1.86812;,
 2.30544;4.05487;1.80380;,
 2.32925;3.81038;1.86021;,
 2.54229;3.87663;1.98296;,
 2.47601;4.17761;1.53519;,
 2.60727;4.09267;1.53804;,
 2.84850;4.00330;1.89729;,
 2.85634;3.97630;1.78256;,
 2.76925;3.87673;1.58905;,
 2.34882;4.17756;1.75582;,
 2.43013;4.25811;1.75042;,
 2.33054;4.27957;1.68580;,
 2.25888;4.20191;1.66787;,
 1.60538;1.75951;1.40658;,
 1.47449;1.89017;1.38581;,
 0.92198;0.87679;1.04814;,
 1.09753;0.77331;1.02816;,
 1.98472;2.50784;1.66197;,
 1.94354;2.63703;1.65493;,
 1.80512;2.87907;1.48044;,
 1.28401;2.13734;1.17979;,
 2.95283;3.19850;2.20395;,
 3.04052;3.37033;2.22638;,
 2.78783;3.44925;2.10092;,
 2.66152;3.20984;2.08003;,
 3.50147;3.26707;2.51577;,
 4.52566;3.27720;3.06580;,
 4.58149;3.42960;3.08018;,
 3.49300;3.41876;2.48097;,
 4.61195;3.56165;3.03748;,
 3.49737;3.55666;2.41482;,
 5.81301;3.21643;3.75067;,
 5.85382;3.33606;3.72886;,
 5.76652;3.08103;3.71786;,
 7.14748;2.85924;4.44605;,
 7.10343;2.77360;4.44188;,
 7.26538;2.73206;4.50497;,
 7.28505;2.77655;4.50605;,
 7.06549;2.67223;4.33575;,
 7.24447;2.66959;4.43907;,
 7.33176;2.71304;4.48947;,
 4.66138;3.61790;2.94399;,
 3.54717;3.61019;2.30086;,
 5.89571;3.39216;3.65737;,
 2.51737;3.03348;1.95688;,
 2.37897;3.34194;1.91347;,
 2.25706;3.08248;1.81583;,
 7.30862;2.78301;4.47611;,
 2.12328;3.27638;1.65302;,
 2.22707;2.81580;1.79949;,
 4.58997;1.10634;3.02541;,
 5.11663;0.26722;3.19553;,
 5.19286;0.32754;3.32577;,
 4.71515;1.18569;3.10660;,
 5.34171;0.38959;3.32541;,
 4.84340;1.26455;3.17120;,
 2.37105;-1.14691;1.61808;,
 2.42579;-1.18265;1.64967;,
 2.40537;-1.14621;1.69738;,
 5.21844;0.18063;3.25458;,
 5.28621;0.18276;3.29380;,
 5.25195;0.21072;3.32855;,
 5.32866;0.24322;3.31819;,
 2.48991;-1.14352;1.68669;,
 2.47524;0.03447;1.84054;,
 2.40937;-0.99849;1.73688;,
 2.56880;-0.99309;1.73224;,
 2.62434;0.03922;1.90460;,
 2.34295;0.02814;1.74301;,
 2.33108;-0.99988;1.59500;,
 2.31277;2.39716;1.83808;,
 2.36929;1.29528;1.82745;,
 2.51376;1.31454;1.91569;,
 2.45464;2.48188;1.91194;,
 2.66404;1.32263;1.99320;,
 2.61017;2.48118;2.00289;,
 3.15618;2.91807;2.28936;,
 4.04081;2.17388;2.76408;,
 4.15401;2.26660;2.84346;,
 3.28479;2.99891;2.38717;,
 3.05800;2.77469;2.25776;,
 3.91748;2.06971;2.70781;,
 4.26958;0.24022;2.71193;,
 3.96521;1.00315;2.67384;,
 3.19670;0.64581;2.24120;,
 3.28976;-0.16845;2.15005;,
 3.21834;1.74202;2.30937;,
 2.17521;3.30955;1.50856;,
 1.82412;2.92703;1.30586;,
 7.18797;2.90387;4.40646;,
 3.15582;3.54263;2.22288;,
 3.22608;3.58942;2.11537;,
 0.66071;1.16110;0.81629;,
 4.40386;2.77167;2.99279;,
 5.58554;1.36375;3.47084;,
 4.98835;1.94062;3.26514;,
 0.07680;0.23510;0.48030;,
 0.39503;-0.08381;0.73384;,
 0.59752;-0.11201;0.59754;,
 1.96524;1.54745;1.60406;,
 1.95857;0.46474;1.52503;,
 1.19703;-0.25262;0.94305;,
 1.52169;0.67786;1.27364;,
 0.02179;-0.02573;0.44563;,
 0.21920;-0.19350;0.57475;,
 0.38229;-0.25120;0.49036;,
 0.04397;0.32005;0.27810;,
 0.00000;0.04573;0.26985;,
 0.64306;1.23327;0.62397;,
 2.36095;4.29427;1.61581;,
 2.26945;4.21138;1.56297;,
 5.36408;2.64695;3.48687;,
 6.31316;2.32109;3.89857;,
 1.81107;-0.53105;1.29642;,
 2.57092;4.16435;1.83788;,
 2.64830;4.18990;1.78169;,
 2.48849;4.28445;1.68942;,
 2.21411;4.04753;1.64610;,
 2.21336;4.04714;1.53059;,
 3.14179;3.64146;2.06666;,
 2.84850;4.00330;1.89729;,
 2.75321;3.97626;1.96152;,
 3.05569;3.62853;2.15110;,
 1.28860;2.19454;0.99713;,
 1.28860;2.19454;0.99713;,
 0.15291;-0.16096;0.36660;,
 0.15291;-0.16096;0.36660;;
 
 242;
 4;0,1,2,3;,
 4;4,5,2,1;,
 4;6,4,1,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,12,15,17;,
 4;18,19,12,16;,
 4;19,20,13,12;,
 4;21,22,23,24;,
 4;22,25,20,19;,
 4;23,22,19,18;,
 3;26,27,21;,
 4;28,16,17,29;,
 4;30,18,16,28;,
 4;31,17,15,9;,
 4;15,14,10,9;,
 4;32,11,33,34;,
 3;26,24,35;,
 4;36,34,4,6;,
 4;34,37,5,4;,
 4;38,39,40,41;,
 4;42,38,41,43;,
 3;44,45,46;,
 3;47,48,49;,
 3;47,49,50;,
 3;44,46,51;,
 4;52,53,54,55;,
 4;53,56,57,54;,
 4;58,59,60,61;,
 4;62,58,61,63;,
 4;64,65,66,67;,
 4;65,68,69,66;,
 4;66,69,33,11;,
 4;67,66,11,10;,
 4;40,39,70,71;,
 4;63,61,34,33;,
 4;61,60,37,34;,
 4;72,71,70,73;,
 3;68,74,69;,
 4;54,57,59,58;,
 3;69,63,33;,
 4;55,54,58,62;,
 4;43,41,65,64;,
 4;41,40,68,65;,
 4;75,36,6,76;,
 4;77,23,18,30;,
 4;29,17,31,78;,
 4;79,7,1,0;,
 3;14,67,10;,
 3;80,67,14;,
 3;72,74,71;,
 4;81,42,43,82;,
 4;83,84,79,0;,
 4;85,83,0,3;,
 3;59,86,60;,
 3;59,87,86;,
 4;88,85,3,89;,
 4;90,91,84,83;,
 4;92,90,83,85;,
 4;91,93,94,84;,
 4;84,94,95,79;,
 4;96,97,98,99;,
 3;89,86,87;,
 3;86,89,2;,
 3;86,2,5;,
 3;82,80,100;,
 3;100,80,13;,
 3;13,80,14;,
 3;69,74,63;,
 3;71,68,40;,
 4;72,73,52,55;,
 4;25,101,100,20;,
 4;56,102,87,57;,
 3;64,67,80;,
 4;101,81,82,100;,
 3;3,2,89;,
 3;68,71,74;,
 3;55,62,72;,
 3;74,72,62;,
 3;43,64,82;,
 3;80,82,64;,
 3;5,37,60;,
 3;57,87,59;,
 3;5,60,86;,
 3;13,20,100;,
 3;62,63,74;,
 4;87,102,88,89;,
 4;75,103,104,36;,
 4;105,106,107,108;,
 4;108,107,97,96;,
 4;34,36,104,32;,
 4;109,110,111,112;,
 4;113,114,115,116;,
 4;78,31,117,118;,
 4;111,110,119,120;,
 4;121,122,123,124;,
 4;125,107,106,126;,
 4;98,125,119,110;,
 4;99,98,110,109;,
 4;127,128,106,105;,
 4;128,129,126,106;,
 4;130,131,132,133;,
 3;126,119,125;,
 4;51,46,53,52;,
 4;50,49,38,42;,
 4;49,48,39,38;,
 4;46,45,56,53;,
 4;35,24,23,77;,
 4;21,27,25,22;,
 3;21,24,26;,
 4;134,135,136,137;,
 4;134,138,139,135;,
 4;135,139,140,141;,
 4;142,143,144,145;,
 4;146,147,148,149;,
 4;149,148,150,151;,
 4;148,152,153,150;,
 4;147,154,152,148;,
 4;155,156,157,158;,
 4;154,159,156,152;,
 4;152,156,155,153;,
 3;160,161,157;,
 4;151,150,162,163;,
 4;150,153,164,162;,
 4;32,120,129,8;,
 4;142,146,149,143;,
 4;165,145,166,167;,
 3;158,161,168;,
 4;139,167,169,140;,
 4;138,170,167,139;,
 4;171,172,173,174;,
 4;174,173,175,176;,
 3;177,178,179;,
 3;180,181,182;,
 3;182,181,183;,
 3;179,178,184;,
 4;185,186,187,188;,
 4;189,190,186,185;,
 4;191,192,193,194;,
 4;194,193,195,196;,
 4;197,198,199,200;,
 4;201,202,198,197;,
 4;165,201,197,145;,
 4;145,197,200,142;,
 4;203,172,171,204;,
 4;167,194,196,165;,
 4;170,191,194,167;,
 4;203,204,205,206;,
 3;207,202,201;,
 4;192,189,185,193;,
 3;196,201,165;,
 4;193,185,188,195;,
 4;198,174,176,199;,
 4;202,171,174,198;,
 4;140,169,208,209;,
 4;153,155,210,164;,
 4;211,151,163,212;,
 4;135,141,213,136;,
 3;200,146,142;,
 3;200,214,146;,
 3;207,205,204;,
 4;176,175,215,216;,
 4;213,217,218,136;,
 4;136,218,219,137;,
 3;220,192,191;,
 3;221,192,220;,
 4;137,219,222,223;,
 4;217,224,225,218;,
 4;218,225,226,219;,
 4;227,228,224,217;,
 4;229,227,217,213;,
 4;133,132,230,231;,
 3;220,223,221;,
 3;223,220,134;,
 3;134,220,138;,
 3;214,216,232;,
 3;214,232,147;,
 3;214,147,146;,
 3;207,201,196;,
 3;202,204,171;,
 4;187,206,205,188;,
 4;232,233,159,154;,
 4;221,234,190,189;,
 3;200,199,214;,
 4;216,215,233,232;,
 3;134,137,223;,
 3;204,202,207;,
 3;195,188,205;,
 3;205,207,195;,
 3;199,176,216;,
 3;216,214,199;,
 3;170,138,191;,
 3;221,189,192;,
 3;191,138,220;,
 3;154,147,232;,
 3;196,195,207;,
 4;222,234,221,223;,
 4;113,116,208,169;,
 4;131,235,236,237;,
 4;132,131,237,230;,
 4;113,169,167,166;,
 4;114,238,239,115;,
 4;240,241,242,243;,
 4;243,211,212,240;,
 4;122,238,114,123;,
 3;166,145,144;,
 4;235,131,130,121;,
 4;8,129,128,117;,
 4;122,130,133,238;,
 4;238,133,231,239;,
 4;235,242,241,236;,
 4;121,124,242,235;,
 3;122,121,130;,
 4;186,179,184,187;,
 4;173,182,183,175;,
 4;172,180,182,173;,
 4;190,177,179,186;,
 4;155,158,168,210;,
 4;159,160,157,156;,
 3;158,157,161;,
 4;31,9,8,117;,
 4;118,117,128,127;,
 4;144,143,211,243;,
 4;103,112,111,104;,
 4;104,111,120,32;,
 4;120,119,126,129;,
 4;113,166,123,114;,
 4;143,149,151,211;,
 4;124,144,243,242;,
 3;32,8,11;,
 4;123,166,144,124;,
 4;97,107,125,98;,
 4;7,79,95,244;,
 4;229,213,141,245;,
 4;245,141,140,209;,
 4;6,7,244,76;,
 3;246,224,228;,
 3;246,226,225;,
 3;247,91,90;,
 3;246,225,224;,
 3;247,93,91;,
 3;247,90,92;;
 
 MeshMaterialList {
  1;
  242;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "Data\\TEXTURE\\flyer.png";
   }
  }
 }
 MeshNormals {
  249;
  0.744249;-0.257081;-0.616444;,
  0.701094;-0.188747;-0.687635;,
  0.710750;-0.195508;-0.675730;,
  0.779517;-0.261090;-0.569373;,
  0.711466;-0.207509;-0.671384;,
  0.853347;-0.452967;-0.258107;,
  0.556612;-0.323600;-0.765157;,
  0.484717;-0.224783;-0.845294;,
  0.508256;-0.099755;-0.855409;,
  0.453463;-0.263281;-0.851501;,
  0.497340;-0.113407;-0.860111;,
  0.433355;-0.379690;-0.817337;,
  0.525859;-0.191226;-0.828797;,
  0.813976;-0.443052;-0.375697;,
  0.517296;-0.117132;-0.847753;,
  0.581311;-0.244243;-0.776159;,
  0.507170;-0.310789;-0.803859;,
  0.742570;-0.277045;-0.609783;,
  0.512781;-0.126530;-0.849144;,
  0.287785;-0.816364;-0.500729;,
  0.138569;-0.062832;-0.988358;,
  0.594836;-0.752633;-0.282337;,
  0.553508;-0.131896;-0.822334;,
  0.256554;-0.246845;-0.934477;,
  0.787241;-0.152161;-0.597577;,
  0.768832;0.065968;-0.636039;,
  0.872902;-0.192958;-0.448119;,
  0.692743;-0.011773;-0.721089;,
  0.622102;0.146396;-0.769127;,
  0.370431;-0.156975;-0.915500;,
  0.608556;0.161370;-0.776930;,
  0.286075;-0.015720;-0.958078;,
  -0.071252;-0.016499;-0.997322;,
  0.615153;0.206227;-0.760958;,
  0.698146;0.509540;-0.502953;,
  -0.143696;-0.122188;-0.982050;,
  0.694640;0.023944;-0.718959;,
  0.724537;-0.055896;-0.686965;,
  0.515838;-0.025473;-0.856307;,
  0.673489;0.117751;-0.729759;,
  0.325088;-0.160380;-0.931985;,
  0.240567;-0.011328;-0.970566;,
  0.877252;-0.074489;-0.474216;,
  0.081777;0.295559;-0.951818;,
  0.021253;0.347908;-0.937288;,
  -0.022191;0.425873;-0.904511;,
  0.497234;0.676464;-0.543282;,
  0.426191;0.659520;-0.619189;,
  0.530882;0.578636;-0.619148;,
  0.654789;0.506711;-0.560799;,
  0.349233;0.696348;-0.627006;,
  -0.132198;0.406476;-0.904047;,
  0.587882;-0.074012;-0.805554;,
  0.714157;-0.524075;-0.464031;,
  0.361355;-0.021373;-0.932183;,
  0.054831;0.134201;-0.989436;,
  -0.480374;-0.304978;-0.822332;,
  -0.240021;-0.409422;-0.880207;,
  -0.070588;-0.700548;-0.710106;,
  -0.220397;0.464162;-0.857892;,
  -0.644550;0.671292;-0.365954;,
  0.219758;0.968134;0.120094;,
  0.067367;0.996915;0.040265;,
  0.448456;0.808222;-0.381660;,
  0.534053;0.844123;0.047379;,
  -0.019621;0.999740;-0.011645;,
  -0.716821;0.565698;-0.407620;,
  -0.253498;0.956281;-0.145828;,
  -0.262545;0.275205;-0.924842;,
  0.018412;-0.999593;0.021809;,
  0.506547;-0.005454;-0.862195;,
  0.397549;-0.212270;-0.892690;,
  0.502354;-0.251109;-0.827397;,
  0.504723;-0.006822;-0.863254;,
  0.270200;-0.060386;-0.960909;,
  0.508425;-0.018253;-0.860913;,
  0.515221;-0.026363;-0.856652;,
  0.520203;-0.011198;-0.853969;,
  0.542541;-0.020832;-0.839771;,
  0.488642;-0.028348;-0.872024;,
  0.510149;-0.054082;-0.858384;,
  0.486165;-0.047570;-0.872571;,
  0.506672;-0.025079;-0.861774;,
  0.517432;-0.014909;-0.855594;,
  0.530958;-0.032551;-0.846773;,
  0.492725;-0.034667;-0.869494;,
  0.530508;-0.056351;-0.845805;,
  0.475492;-0.043837;-0.878627;,
  0.398973;-0.009860;-0.916910;,
  0.581906;0.741115;0.334865;,
  -0.865453;0.060543;-0.497318;,
  0.086187;0.804589;-0.587544;,
  0.775330;0.538910;-0.329302;,
  -0.235831;0.048117;-0.970602;,
  0.252629;0.956507;0.145854;,
  -0.708518;0.575724;-0.408098;,
  0.462128;0.748721;-0.475241;,
  -0.094341;0.496909;-0.862660;,
  -0.845673;0.225491;-0.483726;,
  -0.233045;0.174555;-0.956672;,
  0.737473;0.553956;-0.386350;,
  0.535837;0.788205;0.302675;,
  0.285035;0.063276;-0.956426;,
  0.627180;0.077546;-0.775005;,
  0.635781;0.222846;-0.739001;,
  0.247629;0.255906;-0.934447;,
  0.522744;0.422076;-0.740669;,
  0.657684;0.658976;-0.364969;,
  0.476736;0.834928;0.274989;,
  -0.210860;0.208660;-0.954986;,
  -0.720987;0.263918;-0.640722;,
  0.354423;0.455716;-0.816521;,
  0.497064;-0.663117;-0.559646;,
  0.712241;-0.357724;-0.603942;,
  0.847631;0.283599;-0.448435;,
  0.405101;-0.584706;-0.702859;,
  0.206739;-0.668065;-0.714806;,
  0.631575;-0.534787;-0.561352;,
  0.915436;-0.284273;-0.284897;,
  0.767375;-0.550045;-0.329525;,
  -0.073445;-0.457937;-0.885945;,
  0.689797;0.604641;0.398234;,
  -0.277829;-0.231709;0.932267;,
  -0.418381;-0.107552;0.901881;,
  -0.341821;-0.140578;0.929191;,
  -0.473795;-0.028318;0.880180;,
  -0.450788;-0.053047;0.891054;,
  -0.529084;0.042041;0.847528;,
  -0.468273;0.043613;0.882507;,
  -0.469975;0.017302;0.882510;,
  -0.456142;0.219081;0.862518;,
  -0.474938;-0.104529;0.873789;,
  -0.460654;0.074294;0.884465;,
  -0.441378;-0.138851;0.886513;,
  -0.478484;0.262392;0.837976;,
  -0.489131;-0.047147;0.870935;,
  -0.460912;0.006294;0.887424;,
  -0.454097;-0.022466;0.890669;,
  -0.551625;0.034689;0.833371;,
  -0.429210;-0.144377;0.891591;,
  -0.490153;-0.169577;0.854981;,
  -0.464589;-0.038408;0.884693;,
  -0.485312;-0.046273;0.873116;,
  -0.468740;-0.018838;0.883135;,
  -0.481714;-0.028523;0.875864;,
  -0.516753;-0.040862;0.855159;,
  -0.487271;-0.034026;0.872588;,
  -0.491935;-0.050914;0.869142;,
  -0.473055;-0.042476;0.880008;,
  -0.477695;-0.019020;0.878320;,
  -0.484061;-0.022112;0.874755;,
  -0.366987;-0.088080;0.926046;,
  -0.471616;-0.053744;0.880165;,
  -0.395144;-0.012851;0.918529;,
  -0.521200;-0.105747;0.846858;,
  -0.606252;-0.060878;0.792939;,
  -0.492367;-0.065569;0.867914;,
  -0.783095;0.295104;0.547427;,
  -0.799577;0.346411;0.490587;,
  -0.793126;0.422425;0.438757;,
  -0.220228;0.681203;0.698185;,
  -0.323740;0.658443;0.679445;,
  -0.272875;0.577897;0.769139;,
  -0.159598;0.506945;0.847074;,
  -0.368122;0.696689;0.615720;,
  -0.848757;0.402653;0.342755;,
  -0.440642;0.230471;0.867593;,
  -0.265403;-0.360593;0.894167;,
  -0.808815;0.176345;0.561000;,
  -0.873479;-0.226663;0.430881;,
  -0.738387;-0.429493;0.519924;,
  -0.476832;-0.038631;0.878145;,
  -0.467316;-0.012108;0.884007;,
  -0.485715;0.045176;0.872949;,
  -0.469093;-0.025379;0.882784;,
  -0.479775;-0.025793;0.877013;,
  -0.503950;-0.050572;0.862251;,
  -0.447695;-0.083498;0.890279;,
  -0.431571;-0.044457;0.900983;,
  -0.533999;-0.076402;0.842026;,
  -0.471377;-0.084425;0.877882;,
  -0.447391;-0.054255;0.892691;,
  -0.518537;-0.084683;0.850851;,
  -0.609012;0.060656;0.790839;,
  -0.465955;0.804223;0.368933;,
  0.103089;0.539632;0.835565;,
  -0.957935;0.047459;0.283035;,
  -0.180561;0.748700;0.637845;,
  -0.794238;0.496395;0.350396;,
  -0.944254;0.172784;0.280233;,
  0.036676;0.558546;0.828662;,
  -0.685747;0.062967;0.725111;,
  -0.322104;0.223040;0.920055;,
  -0.380080;0.422081;0.823035;,
  -0.685473;0.255729;0.681711;,
  0.012917;0.659143;0.751906;,
  -0.932237;0.208201;0.295951;,
  -0.529993;0.455595;0.715221;,
  -0.166183;-0.358432;0.918646;,
  0.035608;0.283698;0.958252;,
  -0.406080;-0.584739;0.702267;,
  -0.171481;-0.533290;0.828370;,
  -0.375307;0.164727;0.912146;,
  -0.295945;0.519412;-0.801641;,
  -0.501679;-0.821615;-0.270681;,
  0.323802;0.165833;-0.931478;,
  0.319103;0.146905;-0.936265;,
  0.669168;0.221286;0.709398;,
  0.890100;0.350087;0.291823;,
  -0.033067;-0.070479;-0.996965;,
  -0.015997;0.005365;-0.999858;,
  0.901548;-0.017696;-0.432316;,
  0.899972;0.068296;-0.430565;,
  0.151777;-0.443690;-0.883234;,
  0.189041;-0.498472;-0.846044;,
  0.679697;0.571633;-0.459618;,
  -0.175064;-0.203851;-0.963222;,
  0.710312;-0.681594;-0.175745;,
  0.508152;-0.843924;-0.171970;,
  -0.465345;-0.335533;0.819067;,
  -0.234902;-0.664317;0.709580;,
  -0.043577;-0.718112;0.694562;,
  -0.108480;0.807303;0.580081;,
  0.697824;0.350092;0.624881;,
  -0.630213;-0.251816;0.734453;,
  -0.190246;0.009445;0.981691;,
  -0.803905;-0.458013;0.379421;,
  -0.289653;-0.816406;0.499582;,
  -0.517477;-0.666822;0.536252;,
  0.052358;-0.752204;0.656846;,
  0.210783;-0.282434;0.935843;,
  0.098317;-0.550055;0.829321;,
  -0.163076;-0.184817;0.969149;,
  -0.752423;-0.103220;0.650543;,
  -0.459937;-0.138805;0.877035;,
  -0.411458;-0.139812;0.900641;,
  -0.081774;0.759452;0.645404;,
  -0.387562;-0.064879;0.919558;,
  -0.082734;-0.520062;0.850112;,
  -0.525909;-0.116204;0.842565;,
  -0.120276;-0.951577;0.282905;,
  -0.919221;0.306256;0.247468;,
  -0.964043;-0.264764;0.022819;,
  -0.853089;0.463705;0.239200;,
  -0.423068;-0.092766;0.901337;,
  -0.553316;-0.108998;0.825809;,
  -0.994914;0.095851;0.030955;,
  -0.842146;0.517857;0.150382;,
  -0.754497;-0.635292;-0.164737;;
  242;
  4;2,3,1,0;,
  4;5,4,1,3;,
  4;45,205,206,44;,
  4;103,52,6,16;,
  4;8,7,72,14;,
  4;47,8,14,50;,
  4;48,10,8,47;,
  4;10,9,7,8;,
  4;113,12,49,114;,
  4;12,11,9,10;,
  4;49,12,10,48;,
  3;13,112,113;,
  4;62,47,50,65;,
  4;63,48,47,62;,
  4;46,50,14,52;,
  4;14,72,6,52;,
  4;88,16,15,18;,
  3;207,208,121;,
  4;51,18,205,45;,
  4;18,17,4,5;,
  4;22,23,40,38;,
  4;25,22,38,39;,
  3;19,120,115;,
  3;21,116,117;,
  3;21,117,118;,
  3;19,115,119;,
  4;24,26,42,37;,
  4;209,20,41,210;,
  4;32,31,74,35;,
  4;27,211,212,36;,
  4;28,30,34,33;,
  4;213,29,71,214;,
  4;214,71,15,16;,
  4;33,34,215,215;,
  4;40,23,80,82;,
  4;36,212,18,15;,
  4;35,74,216,18;,
  4;76,82,80,86;,
  3;29,70,71;,
  4;210,41,31,32;,
  3;71,36,15;,
  4;37,42,211,27;,
  4;39,38,30,28;,
  4;38,40,29,30;,
  4;66,51,45,60;,
  4;64,49,48,63;,
  4;65,50,46,61;,
  4;43,44,3,2;,
  3;72,33,6;,
  3;73,33,72;,
  3;76,70,82;,
  4;78,25,39,77;,
  4;54,55,43,2;,
  4;53,217,2,0;,
  3;31,75,74;,
  3;31,79,75;,
  4;81,53,0,85;,
  4;58,57,55,54;,
  4;69,218,217,53;,
  4;57,56,68,55;,
  4;55,68,59,43;,
  4;67,91,97,95;,
  3;85,75,79;,
  3;75,85,1;,
  3;75,1,4;,
  3;77,73,83;,
  3;83,73,7;,
  3;7,73,72;,
  3;71,70,36;,
  3;82,29,40;,
  4;76,86,24,37;,
  4;11,84,83,9;,
  4;20,87,79,41;,
  3;28,33,73;,
  4;84,78,77,83;,
  3;0,1,85;,
  3;29,82,70;,
  3;37,27,76;,
  3;70,76,27;,
  3;39,28,77;,
  3;73,77,28;,
  3;4,17,74;,
  3;41,79,31;,
  3;4,74,75;,
  3;7,9,83;,
  3;27,36,70;,
  4;79,87,81,85;,
  4;66,98,99,51;,
  4;108,107,96,94;,
  4;94,96,91,67;,
  4;18,51,99,88;,
  4;110,109,93,90;,
  4;189,186,90,98;,
  4;61,46,100,101;,
  4;93,109,105,102;,
  4;193,194,191,192;,
  4;111,96,107,106;,
  4;97,111,105,109;,
  4;95,97,109,110;,
  4;89,92,107,108;,
  4;92,104,106,107;,
  4;197,187,184,188;,
  3;106,105,111;,
  4;119,115,26,24;,
  4;118,117,22,25;,
  4;117,116,23,22;,
  4;115,120,20,209;,
  4;121,114,49,64;,
  4;113,112,11,12;,
  3;113,114,13;,
  4;123,125,124,122;,
  4;123,126,127,125;,
  4;125,127,159,158;,
  4;128,166,202,136;,
  4;173,129,130,134;,
  4;134,130,161,164;,
  4;130,132,162,161;,
  4;129,131,132,130;,
  4;163,133,198,199;,
  4;131,219,133,132;,
  4;132,133,163,162;,
  3;220,221,198;,
  4;164,161,62,65;,
  4;161,162,222,62;,
  4;88,102,104,103;,
  4;128,173,134,166;,
  4;135,136,183,138;,
  3;223,207,121;,
  4;127,138,165,159;,
  4;126,137,138,127;,
  4;154,224,139,152;,
  4;152,139,225,153;,
  3;226,227,200;,
  3;228,229,201;,
  3;201,229,230;,
  3;200,227,231;,
  4;156,140,232,151;,
  4;155,233,140,156;,
  4;175,145,146,149;,
  4;149,146,141,150;,
  4;148,144,142,147;,
  4;172,143,144,148;,
  4;135,172,148,136;,
  4;136,148,147,128;,
  4;234,224,154,180;,
  4;138,149,150,135;,
  4;137,175,149,138;,
  4;234,180,177,235;,
  3;171,143,172;,
  4;145,155,156,146;,
  3;150,172,135;,
  4;146,156,151,141;,
  4;144,152,153,142;,
  4;143,154,152,144;,
  4;159,165,66,60;,
  4;162,163,236,222;,
  4;160,164,65,61;,
  4;125,158,157,124;,
  3;147,173,128;,
  3;147,174,173;,
  3;171,177,180;,
  4;153,225,237,178;,
  4;157,168,167,124;,
  4;124,167,238,122;,
  3;176,145,175;,
  3;179,145,176;,
  4;122,238,239,182;,
  4;168,169,170,167;,
  4;167,240,69,238;,
  4;241,242,169,168;,
  4;243,241,168,157;,
  4;188,184,67,95;,
  3;176,182,179;,
  3;182,176,123;,
  3;123,176,126;,
  3;174,178,181;,
  3;174,181,129;,
  3;174,129,173;,
  3;171,172,150;,
  3;143,180,154;,
  4;232,235,177,151;,
  4;181,244,219,131;,
  4;179,245,233,155;,
  3;147,142,174;,
  4;178,237,244,181;,
  3;123,122,182;,
  3;180,143,171;,
  3;141,151,177;,
  3;177,171,141;,
  3;142,153,178;,
  3;178,174,142;,
  3;137,126,175;,
  3;179,155,145;,
  3;175,126,176;,
  3;131,129,181;,
  3;150,141,171;,
  4;239,245,179,182;,
  4;189,98,66,165;,
  4;187,195,108,94;,
  4;184,187,94,67;,
  4;189,165,138,183;,
  4;186,196,246,90;,
  4;101,89,185,190;,
  4;190,160,61,101;,
  4;194,196,186,191;,
  3;183,136,202;,
  4;195,187,197,193;,
  4;103,104,92,100;,
  4;194,197,188,196;,
  4;196,188,95,110;,
  4;195,185,89,108;,
  4;193,192,185,195;,
  3;194,193,197;,
  4;140,200,231,232;,
  4;139,201,230,225;,
  4;224,228,201,139;,
  4;233,226,200,140;,
  4;163,199,121,64;,
  4;219,220,198,133;,
  3;199,198,207;,
  4;46,52,103,100;,
  4;101,100,92,89;,
  4;202,166,160,190;,
  4;98,90,93,99;,
  4;99,93,102,88;,
  4;102,105,106,104;,
  4;189,183,191,186;,
  4;166,134,164,160;,
  4;192,202,190,185;,
  3;88,103,16;,
  4;191,183,202,192;,
  4;91,96,111,97;,
  4;44,43,59,203;,
  4;243,157,158,247;,
  4;247,158,159,60;,
  4;45,44,203,60;,
  3;204,248,242;,
  3;204,69,240;,
  3;204,57,58;,
  3;204,170,169;,
  3;204,56,57;,
  3;204,58,69;;
 }
 MeshTextureCoords {
  248;
  0.676200;0.449150;,
  0.708920;0.388430;,
  0.695710;0.384360;,
  0.663340;0.443200;,
  0.731050;0.341310;,
  0.719340;0.341350;,
  0.748870;0.344050;,
  0.729290;0.395020;,
  0.743530;0.261910;,
  0.732500;0.248180;,
  0.724010;0.257470;,
  0.733810;0.275810;,
  0.684010;0.160770;,
  0.677150;0.168710;,
  0.709240;0.225520;,
  0.719390;0.221180;,
  0.690470;0.156270;,
  0.727410;0.217900;,
  0.639770;0.094350;,
  0.634580;0.099230;,
  0.629000;0.106350;,
  0.568800;0.035910;,
  0.574690;0.043100;,
  0.577840;0.038620;,
  0.570360;0.034090;,
  0.570480;0.050480;,
  0.566050;0.032850;,
  0.565210;0.039600;,
  0.696060;0.152560;,
  0.734220;0.214270;,
  0.644220;0.090890;,
  0.739490;0.237770;,
  0.750170;0.290550;,
  0.727230;0.292640;,
  0.740780;0.308030;,
  0.570690;0.031940;,
  0.758990;0.312990;,
  0.726060;0.320270;,
  0.541550;0.217480;,
  0.541170;0.226560;,
  0.584590;0.227000;,
  0.585160;0.216170;,
  0.540530;0.208390;,
  0.585200;0.205660;,
  0.528710;0.418950;,
  0.532120;0.421560;,
  0.531480;0.417120;,
  0.531580;0.218530;,
  0.533660;0.222910;,
  0.534720;0.218200;,
  0.533340;0.213780;,
  0.528170;0.413990;,
  0.532540;0.404890;,
  0.537440;0.412270;,
  0.583350;0.380410;,
  0.578240;0.371130;,
  0.541050;0.420970;,
  0.588130;0.390130;,
  0.643980;0.342740;,
  0.648700;0.353650;,
  0.704060;0.326570;,
  0.703140;0.313830;,
  0.638490;0.332220;,
  0.696820;0.302400;,
  0.643760;0.218330;,
  0.642390;0.229510;,
  0.703090;0.258670;,
  0.703140;0.245990;,
  0.640040;0.240570;,
  0.697470;0.269430;,
  0.549920;0.274590;,
  0.592180;0.267760;,
  0.591830;0.321650;,
  0.550120;0.342050;,
  0.642580;0.289300;,
  0.767810;0.316420;,
  0.758600;0.348020;,
  0.580300;0.035460;,
  0.745350;0.232130;,
  0.700560;0.458850;,
  0.658680;0.189950;,
  0.566720;0.165700;,
  0.607260;0.180670;,
  0.649260;0.504200;,
  0.672600;0.515750;,
  0.636490;0.499660;,
  0.673770;0.369850;,
  0.621380;0.401020;,
  0.611200;0.468630;,
  0.645410;0.420890;,
  0.649260;0.517200;,
  0.661380;0.524480;,
  0.637810;0.517570;,
  0.668050;0.533400;,
  0.682700;0.523470;,
  0.711190;0.464360;,
  0.803410;0.269930;,
  0.799230;0.270670;,
  0.798090;0.275360;,
  0.803160;0.277660;,
  0.623790;0.140380;,
  0.577890;0.100340;,
  0.579030;0.439690;,
  0.769910;0.313200;,
  0.763610;0.307130;,
  0.790720;0.254950;,
  0.788530;0.259880;,
  0.796240;0.265950;,
  0.799500;0.262530;,
  0.798510;0.286500;,
  0.793940;0.282230;,
  0.784180;0.291220;,
  0.788860;0.296780;,
  0.757560;0.512170;,
  0.738390;0.496240;,
  0.733600;0.501430;,
  0.751390;0.517990;,
  0.747630;0.242230;,
  0.750330;0.235640;,
  0.788210;0.275010;,
  0.776080;0.279620;,
  0.737390;0.474240;,
  0.735350;0.480350;,
  0.746760;0.485490;,
  0.750970;0.472780;,
  0.793670;0.271080;,
  0.786490;0.268580;,
  0.776190;0.247100;,
  0.774310;0.253380;,
  0.772580;0.266110;,
  0.730270;0.476230;,
  0.728090;0.471090;,
  0.724890;0.475500;,
  0.725730;0.480130;,
  0.822780;0.590520;,
  0.812370;0.592840;,
  0.842330;0.654950;,
  0.853100;0.650620;,
  0.798930;0.545880;,
  0.791290;0.544080;,
  0.772770;0.547630;,
  0.791810;0.598850;,
  0.797720;0.469250;,
  0.791710;0.459210;,
  0.778930;0.470960;,
  0.787300;0.484600;,
  0.814580;0.436420;,
  0.849350;0.380590;,
  0.844200;0.373200;,
  0.806460;0.432500;,
  0.837890;0.368460;,
  0.798570;0.428860;,
  0.896030;0.313480;,
  0.890980;0.308400;,
  0.900410;0.320150;,
  0.954920;0.254880;,
  0.958010;0.259420;,
  0.964060;0.252490;,
  0.962580;0.250620;,
  0.961860;0.267010;,
  0.967530;0.256310;,
  0.966930;0.249530;,
  0.832400;0.364570;,
  0.791920;0.424960;,
  0.886620;0.304810;,
  0.790520;0.499510;,
  0.770520;0.496940;,
  0.778760;0.512960;,
  0.962320;0.248450;,
  0.761980;0.517820;,
  0.791530;0.522790;,
  0.939590;0.440450;,
  0.983100;0.443760;,
  0.983150;0.434710;,
  0.939710;0.430710;,
  0.984650;0.425760;,
  0.939860;0.421340;,
  0.983850;0.635290;,
  0.987290;0.632820;,
  0.984620;0.630930;,
  0.990700;0.440490;,
  0.992970;0.436260;,
  0.989890;0.435780;,
  0.991470;0.431470;,
  0.987980;0.627960;,
  0.934680;0.592450;,
  0.978880;0.625940;,
  0.983990;0.618850;,
  0.939130;0.584380;,
  0.930140;0.600780;,
  0.975060;0.634440;,
  0.815490;0.530570;,
  0.871300;0.561060;,
  0.875470;0.552160;,
  0.816100;0.520110;,
  0.880060;0.543610;,
  0.821690;0.511270;,
  0.818910;0.467540;,
  0.881490;0.441160;,
  0.880720;0.431660;,
  0.819810;0.457150;,
  0.822940;0.477090;,
  0.882860;0.450870;,
  0.971220;0.490270;,
  0.929540;0.480910;,
  0.927320;0.534700;,
  0.968280;0.556460;,
  0.877740;0.500060;,
  0.753420;0.521170;,
  0.763140;0.551750;,
  0.952570;0.251630;,
  0.785470;0.448020;,
  0.780020;0.442080;,
  0.817680;0.663280;,
  0.866920;0.402670;,
  0.960430;0.382060;,
  0.918870;0.395440;,
  0.841910;0.721570;,
  0.865840;0.711710;,
  0.878620;0.707640;,
  0.845290;0.576450;,
  0.896480;0.610110;,
  0.904220;0.677740;,
  0.871750;0.628990;,
  0.852520;0.730330;,
  0.864590;0.723970;,
  0.875500;0.724780;,
  0.831270;0.728350;,
  0.845420;0.738370;,
  0.806930;0.668150;,
  0.720870;0.474540;,
  0.720670;0.482090;,
  0.904070;0.354450;,
  0.952140;0.316450;,
  0.936850;0.650660;,
  0.735960;0.465670;,
  0.734160;0.460730;,
  0.725120;0.467560;,
  0.729390;0.487040;,
  0.724710;0.490950;,
  0.774900;0.445120;,
  0.748870;0.454100;,
  0.750230;0.460340;,
  0.777050;0.451750;,
  0.739270;0.399700;,
  0.781790;0.603250;,
  0.862340;0.736090;,
  0.650540;0.530470;;
 }
}