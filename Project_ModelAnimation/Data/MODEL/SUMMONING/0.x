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
 184;
 1.72603;6.00622;0.00000;,
 1.81535;6.00359;-0.08931;,
 1.81535;5.59371;-0.08931;,
 1.72603;5.59371;0.00000;,
 1.73127;6.36132;0.00000;,
 1.82035;6.35514;-0.08931;,
 1.90466;6.00098;0.00000;,
 1.90466;5.59371;0.00000;,
 1.90966;6.34895;0.00000;,
 1.81535;6.00359;0.08931;,
 1.81535;5.59371;0.08931;,
 1.82035;6.35514;0.08931;,
 1.74723;6.60165;0.00000;,
 1.83583;6.58974;-0.08931;,
 1.77390;6.77337;0.00000;,
 1.86131;6.75479;-0.08931;,
 1.92419;6.57783;0.00000;,
 1.94872;6.73621;0.00000;,
 1.83583;6.58974;0.08931;,
 1.86131;6.75479;0.08931;,
 1.81130;6.92294;0.00000;,
 1.89704;6.89769;-0.08931;,
 1.86250;7.06847;0.00000;,
 1.94562;7.03583;-0.08931;,
 1.98254;6.87244;0.00000;,
 2.02875;7.00321;0.00000;,
 1.89704;6.89769;0.08931;,
 1.94562;7.03583;0.08931;,
 1.93086;7.22828;0.00000;,
 2.01017;7.18708;-0.08931;,
 2.01612;7.38309;0.00000;,
 2.08996;7.33211;-0.08931;,
 2.08948;7.14563;0.00000;,
 2.16355;7.28115;0.00000;,
 2.01017;7.18708;0.08931;,
 2.08996;7.33211;0.08931;,
 2.11925;7.51337;0.00000;,
 2.18451;7.45215;-0.08931;,
 2.23048;7.61697;0.00000;,
 2.28407;7.54600;-0.08931;,
 2.24977;7.39118;0.00000;,
 2.33789;7.47479;0.00000;,
 2.18451;7.45215;0.08931;,
 2.28407;7.54600;0.08931;,
 2.34052;7.69246;0.00000;,
 2.37934;7.61172;-0.08931;,
 2.45412;7.73748;0.00000;,
 2.47341;7.64984;-0.08931;,
 2.41792;7.53100;0.00000;,
 2.49270;7.56218;0.00000;,
 2.37934;7.61172;0.08931;,
 2.47341;7.64984;0.08931;,
 2.57535;7.74938;0.00000;,
 2.57035;7.66008;-0.08931;,
 2.69253;7.72820;0.00000;,
 2.66133;7.64436;-0.08931;,
 2.56535;7.57100;0.00000;,
 2.63013;7.56052;0.00000;,
 2.57035;7.66008;0.08931;,
 2.66133;7.64436;0.08931;,
 2.79304;7.67365;0.00000;,
 2.73707;7.60411;-0.08931;,
 2.86901;7.58958;0.00000;,
 2.79375;7.53956;-0.08931;,
 2.68110;7.53455;0.00000;,
 2.71825;7.48932;0.00000;,
 2.73707;7.60411;0.08931;,
 2.79375;7.53956;0.08931;,
 2.91260;7.47931;0.00000;,
 2.82757;7.45050;-0.08931;,
 2.93046;7.35237;0.00000;,
 2.84186;7.34426;-0.08931;,
 2.74231;7.42191;0.00000;,
 2.75350;7.33617;0.00000;,
 2.82757;7.45050;0.08931;,
 2.84186;7.34426;0.08931;,
 2.92951;7.21850;0.00000;,
 2.84067;7.22756;-0.08931;,
 2.90903;7.09013;0.00000;,
 2.82186;7.11277;-0.08931;,
 2.75184;7.23660;0.00000;,
 2.73493;7.13538;0.00000;,
 2.84067;7.22756;0.08931;,
 2.82186;7.11277;0.08931;,
 2.86783;6.98010;0.00000;,
 2.78423;7.01248;-0.08931;,
 2.81185;6.88007;0.00000;,
 2.70730;6.88030;-0.08931;,
 2.70063;7.04488;0.00000;,
 2.60298;6.88054;0.00000;,
 2.78423;7.01248;0.08931;,
 2.70730;6.88030;0.08931;,
 1.81535;5.59371;-0.08931;,
 1.81535;6.00359;-0.08931;,
 1.90466;6.00098;0.00000;,
 1.90466;5.59371;0.00000;,
 1.82035;6.35514;-0.08931;,
 1.90966;6.34895;0.00000;,
 1.72603;5.59371;0.00000;,
 1.72603;6.00622;0.00000;,
 1.73127;6.36132;0.00000;,
 1.81535;5.59371;0.08931;,
 1.81535;6.00359;0.08931;,
 1.82035;6.35514;0.08931;,
 1.79248;6.58974;-0.08931;,
 1.88108;6.60165;0.00000;,
 1.76700;6.75479;-0.08931;,
 1.85441;6.77337;0.00000;,
 1.70388;6.57783;0.00000;,
 1.67959;6.73621;0.00000;,
 1.79248;6.58974;0.08931;,
 1.76700;6.75479;0.08931;,
 1.73127;6.89769;-0.08931;,
 1.81130;6.92294;0.00000;,
 1.68245;7.03583;-0.08931;,
 1.76557;7.06847;0.00000;,
 1.64553;6.87244;0.00000;,
 1.59956;7.00321;0.00000;,
 1.73127;6.89769;0.08931;,
 1.68245;7.03583;0.08931;,
 1.61814;7.18708;-0.08931;,
 1.69745;7.22828;0.00000;,
 1.53835;7.33211;-0.08931;,
 1.61195;7.38309;0.00000;,
 1.53883;7.14563;0.00000;,
 1.46475;7.28115;0.00000;,
 1.61814;7.18708;0.08931;,
 1.53835;7.33211;0.08931;,
 1.44380;7.45215;-0.08931;,
 1.50906;7.51337;0.00000;,
 1.34400;7.54600;-0.08931;,
 1.39783;7.61697;0.00000;,
 1.37854;7.39118;0.00000;,
 1.29041;7.47479;0.00000;,
 1.44380;7.45215;0.08931;,
 1.34400;7.54600;0.08931;,
 1.24897;7.61172;-0.08931;,
 1.28756;7.69246;0.00000;,
 1.15489;7.64984;-0.08931;,
 1.17419;7.73748;0.00000;,
 1.21039;7.53100;0.00000;,
 1.13560;7.56218;0.00000;,
 1.24897;7.61172;0.08931;,
 1.15489;7.64984;0.08931;,
 1.05772;7.66008;-0.08931;,
 1.05272;7.74938;0.00000;,
 0.96698;7.64436;-0.08931;,
 0.93578;7.72820;0.00000;,
 1.06272;7.57100;0.00000;,
 0.99818;7.56052;0.00000;,
 1.05772;7.66008;0.08931;,
 0.96698;7.64436;0.08931;,
 0.89124;7.60411;-0.08931;,
 0.83527;7.67365;0.00000;,
 0.83455;7.53956;-0.08931;,
 0.75905;7.58958;0.00000;,
 0.94721;7.53455;0.00000;,
 0.90982;7.48932;0.00000;,
 0.89124;7.60411;0.08931;,
 0.83455;7.53956;0.08931;,
 0.80073;7.45050;-0.08931;,
 0.71547;7.47931;0.00000;,
 0.78620;7.34426;-0.08931;,
 0.69784;7.35237;0.00000;,
 0.88576;7.42191;0.00000;,
 0.87480;7.33617;0.00000;,
 0.80073;7.45050;0.08931;,
 0.78620;7.34426;0.08931;,
 0.78763;7.22756;-0.08931;,
 0.69880;7.21850;0.00000;,
 0.80621;7.11277;-0.08931;,
 0.71928;7.09013;0.00000;,
 0.87647;7.23660;0.00000;,
 0.89338;7.13538;0.00000;,
 0.78763;7.22756;0.08931;,
 0.80621;7.11277;0.08931;,
 0.84408;7.01248;-0.08931;,
 0.76024;6.98010;0.00000;,
 0.92077;6.88030;-0.08931;,
 0.81645;6.88007;0.00000;,
 0.92768;7.04488;0.00000;,
 1.02533;6.88054;0.00000;,
 0.84408;7.01248;0.08931;,
 0.92077;6.88030;0.08931;;
 
 176;
 4;0,1,2,3;,
 4;4,5,1,0;,
 4;1,6,7,2;,
 4;5,8,6,1;,
 4;6,9,10,7;,
 4;8,11,9,6;,
 4;9,0,3,10;,
 4;11,4,0,9;,
 4;12,13,5,4;,
 4;14,15,13,12;,
 4;13,16,8,5;,
 4;15,17,16,13;,
 4;16,18,11,8;,
 4;17,19,18,16;,
 4;18,12,4,11;,
 4;19,14,12,18;,
 4;20,21,15,14;,
 4;22,23,21,20;,
 4;21,24,17,15;,
 4;23,25,24,21;,
 4;24,26,19,17;,
 4;25,27,26,24;,
 4;26,20,14,19;,
 4;27,22,20,26;,
 4;28,29,23,22;,
 4;30,31,29,28;,
 4;29,32,25,23;,
 4;31,33,32,29;,
 4;32,34,27,25;,
 4;33,35,34,32;,
 4;34,28,22,27;,
 4;35,30,28,34;,
 4;36,37,31,30;,
 4;38,39,37,36;,
 4;37,40,33,31;,
 4;39,41,40,37;,
 4;40,42,35,33;,
 4;41,43,42,40;,
 4;42,36,30,35;,
 4;43,38,36,42;,
 4;44,45,39,38;,
 4;46,47,45,44;,
 4;45,48,41,39;,
 4;47,49,48,45;,
 4;48,50,43,41;,
 4;49,51,50,48;,
 4;50,44,38,43;,
 4;51,46,44,50;,
 4;52,53,47,46;,
 4;54,55,53,52;,
 4;53,56,49,47;,
 4;55,57,56,53;,
 4;56,58,51,49;,
 4;57,59,58,56;,
 4;58,52,46,51;,
 4;59,54,52,58;,
 4;60,61,55,54;,
 4;62,63,61,60;,
 4;61,64,57,55;,
 4;63,65,64,61;,
 4;64,66,59,57;,
 4;65,67,66,64;,
 4;66,60,54,59;,
 4;67,62,60,66;,
 4;68,69,63,62;,
 4;70,71,69,68;,
 4;69,72,65,63;,
 4;71,73,72,69;,
 4;72,74,67,65;,
 4;73,75,74,72;,
 4;74,68,62,67;,
 4;75,70,68,74;,
 4;76,77,71,70;,
 4;78,79,77,76;,
 4;77,80,73,71;,
 4;79,81,80,77;,
 4;80,82,75,73;,
 4;81,83,82,80;,
 4;82,76,70,75;,
 4;83,78,76,82;,
 4;84,85,79,78;,
 4;86,87,85,84;,
 4;85,88,81,79;,
 4;87,89,88,85;,
 4;88,90,83,81;,
 4;89,91,90,88;,
 4;90,84,78,83;,
 4;91,86,84,90;,
 4;92,93,94,95;,
 4;93,96,97,94;,
 4;98,99,93,92;,
 4;99,100,96,93;,
 4;101,102,99,98;,
 4;102,103,100,99;,
 4;95,94,102,101;,
 4;94,97,103,102;,
 4;96,104,105,97;,
 4;104,106,107,105;,
 4;100,108,104,96;,
 4;108,109,106,104;,
 4;103,110,108,100;,
 4;110,111,109,108;,
 4;97,105,110,103;,
 4;105,107,111,110;,
 4;106,112,113,107;,
 4;112,114,115,113;,
 4;109,116,112,106;,
 4;116,117,114,112;,
 4;111,118,116,109;,
 4;118,119,117,116;,
 4;107,113,118,111;,
 4;113,115,119,118;,
 4;114,120,121,115;,
 4;120,122,123,121;,
 4;117,124,120,114;,
 4;124,125,122,120;,
 4;119,126,124,117;,
 4;126,127,125,124;,
 4;115,121,126,119;,
 4;121,123,127,126;,
 4;122,128,129,123;,
 4;128,130,131,129;,
 4;125,132,128,122;,
 4;132,133,130,128;,
 4;127,134,132,125;,
 4;134,135,133,132;,
 4;123,129,134,127;,
 4;129,131,135,134;,
 4;130,136,137,131;,
 4;136,138,139,137;,
 4;133,140,136,130;,
 4;140,141,138,136;,
 4;135,142,140,133;,
 4;142,143,141,140;,
 4;131,137,142,135;,
 4;137,139,143,142;,
 4;138,144,145,139;,
 4;144,146,147,145;,
 4;141,148,144,138;,
 4;148,149,146,144;,
 4;143,150,148,141;,
 4;150,151,149,148;,
 4;139,145,150,143;,
 4;145,147,151,150;,
 4;146,152,153,147;,
 4;152,154,155,153;,
 4;149,156,152,146;,
 4;156,157,154,152;,
 4;151,158,156,149;,
 4;158,159,157,156;,
 4;147,153,158,151;,
 4;153,155,159,158;,
 4;154,160,161,155;,
 4;160,162,163,161;,
 4;157,164,160,154;,
 4;164,165,162,160;,
 4;159,166,164,157;,
 4;166,167,165,164;,
 4;155,161,166,159;,
 4;161,163,167,166;,
 4;162,168,169,163;,
 4;168,170,171,169;,
 4;165,172,168,162;,
 4;172,173,170,168;,
 4;167,174,172,165;,
 4;174,175,173,172;,
 4;163,169,174,167;,
 4;169,171,175,174;,
 4;170,176,177,171;,
 4;176,178,179,177;,
 4;173,180,176,170;,
 4;180,181,178,176;,
 4;175,182,180,173;,
 4;182,183,181,180;,
 4;171,177,182,175;,
 4;177,179,183,182;;
 
 MeshMaterialList {
  2;
  176;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   100.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.047000;0.361000;0.902000;1.000000;;
   100.000000;
   1.000000;1.000000;1.000000;;
   0.047000;0.361000;0.902000;;
  }
 }
 MeshNormals {
  172;
  -0.707067;0.000000;-0.707146;,
  0.707107;0.000000;-0.707107;,
  0.707107;0.000000;0.707107;,
  -0.707150;-0.004809;-0.707047;,
  0.706864;0.003954;-0.707338;,
  0.706864;0.003954;0.707338;,
  -0.692768;0.139942;-0.707453;,
  0.693312;-0.139671;-0.706973;,
  0.693312;-0.139671;0.706973;,
  -0.659153;0.256431;-0.706937;,
  0.659258;-0.256507;-0.706812;,
  0.659258;-0.256507;0.706812;,
  -0.588132;0.390199;-0.708410;,
  0.589318;-0.389286;-0.707927;,
  0.589318;-0.389286;0.707927;,
  -0.444022;0.551685;-0.706037;,
  0.446645;-0.549217;-0.706307;,
  0.446645;-0.549217;0.706307;,
  -0.168516;0.684882;-0.708900;,
  0.175560;-0.683496;-0.708528;,
  0.175560;-0.683496;0.708528;,
  0.231596;0.667410;-0.707762;,
  -0.224216;-0.670214;-0.707489;,
  -0.224216;-0.670214;0.707489;,
  0.599609;0.366649;-0.711363;,
  -0.606464;-0.354194;-0.711863;,
  -0.606464;-0.354194;0.711863;,
  0.707365;0.045396;-0.705389;,
  -0.707937;-0.041948;-0.705029;,
  -0.707937;-0.041948;0.705029;,
  0.681032;-0.180737;-0.709598;,
  -0.680995;0.182791;-0.709108;,
  -0.680995;0.182791;0.709108;,
  0.613065;-0.349884;-0.708331;,
  -0.609980;0.358707;-0.706579;,
  -0.609980;0.358707;0.706579;,
  -0.707170;0.005124;-0.707025;,
  0.706973;-0.005052;-0.707223;,
  0.706973;-0.005052;0.707223;,
  -0.702651;0.077502;-0.707301;,
  0.702954;-0.076953;-0.707060;,
  0.702954;-0.076953;0.707060;,
  -0.677004;0.203229;-0.707364;,
  0.677442;-0.203303;-0.706923;,
  0.677442;-0.203303;0.706923;,
  -0.635465;0.309690;-0.707302;,
  0.635812;-0.309017;-0.707285;,
  0.635812;-0.309017;0.707285;,
  -0.520447;0.478325;-0.707347;,
  0.522373;-0.476638;-0.707066;,
  0.522373;-0.476638;0.707066;,
  -0.333850;0.622966;-0.707430;,
  0.338307;-0.620612;-0.707382;,
  0.338307;-0.620612;0.707382;,
  0.026023;0.705996;-0.707738;,
  -0.018744;-0.706563;-0.707402;,
  -0.018744;-0.706563;0.707402;,
  0.436307;0.553362;-0.709526;,
  -0.438036;-0.549742;-0.711272;,
  -0.438036;-0.549742;0.711272;,
  0.684218;0.176947;-0.707486;,
  -0.686247;-0.169478;-0.707349;,
  -0.686247;-0.169478;0.707349;,
  0.704558;-0.059564;-0.707142;,
  -0.704773;0.062582;-0.706667;,
  -0.704773;0.062582;0.706667;,
  0.638053;-0.299301;-0.709441;,
  -0.636623;0.304532;-0.708499;,
  -0.636623;0.304532;0.708499;,
  0.696950;0.147476;-0.701792;,
  -0.692837;-0.139607;-0.707451;,
  -0.692837;-0.139607;0.707451;,
  0.666566;0.252501;-0.701379;,
  -0.659559;-0.256361;-0.706584;,
  -0.659559;-0.256361;0.706584;,
  0.588482;0.390444;-0.707984;,
  -0.589305;-0.389277;-0.707942;,
  -0.589305;-0.389277;0.707942;,
  0.443770;0.551669;-0.706208;,
  -0.446837;-0.549460;-0.705996;,
  -0.446837;-0.549460;0.705996;,
  0.168586;0.684944;-0.708823;,
  -0.175437;-0.683520;-0.708536;,
  -0.175437;-0.683520;0.708536;,
  -0.231734;0.667373;-0.707752;,
  0.224393;-0.670168;-0.707476;,
  0.224393;-0.670168;0.707476;,
  -0.598866;0.366585;-0.712022;,
  0.606705;-0.354943;-0.711283;,
  0.606705;-0.354943;0.711283;,
  -0.707649;0.045100;-0.705124;,
  0.707665;-0.041489;-0.705329;,
  0.707665;-0.041489;0.705329;,
  -0.681309;-0.180460;-0.709403;,
  0.680536;0.182698;-0.709572;,
  0.680536;0.182698;0.709572;,
  -0.612889;-0.349953;-0.708449;,
  0.609842;0.358065;-0.707024;,
  0.609842;0.358065;0.707024;,
  0.701841;0.094965;-0.705975;,
  -0.701911;-0.096995;-0.705629;,
  -0.701911;-0.096995;0.705629;,
  0.687909;0.207112;-0.695619;,
  -0.677138;-0.203514;-0.707154;,
  -0.677138;-0.203514;0.707154;,
  0.635730;0.309551;-0.707124;,
  -0.636106;-0.308894;-0.707074;,
  -0.636106;-0.308894;0.707074;,
  0.520488;0.478339;-0.707307;,
  -0.522317;-0.476890;-0.706937;,
  -0.522317;-0.476890;0.706937;,
  0.334007;0.622964;-0.707357;,
  -0.338565;-0.620624;-0.707247;,
  -0.338565;-0.620624;0.707247;,
  -0.026242;0.705981;-0.707744;,
  0.019036;-0.706548;-0.707409;,
  0.019036;-0.706548;0.707409;,
  -0.435932;0.553436;-0.709699;,
  0.437790;-0.550273;-0.711013;,
  0.437790;-0.550273;0.711013;,
  -0.683848;0.176857;-0.707865;,
  0.686609;-0.169549;-0.706980;,
  0.686609;-0.169549;0.706980;,
  -0.705048;-0.059637;-0.706648;,
  0.704339;0.062583;-0.707099;,
  0.704339;0.062583;0.707099;,
  -0.637938;-0.299359;-0.709520;,
  0.636208;0.304474;-0.708897;,
  0.636208;0.304474;0.708897;,
  -0.707170;0.005124;0.707025;,
  -0.707067;0.000000;0.707146;,
  -0.707150;-0.004809;0.707047;,
  -0.702651;0.077502;0.707301;,
  -0.692768;0.139942;0.707453;,
  -0.677004;0.203229;0.707364;,
  -0.659153;0.256431;0.706937;,
  -0.635465;0.309690;0.707302;,
  -0.588132;0.390199;0.708410;,
  -0.520447;0.478325;0.707347;,
  -0.444022;0.551685;0.706037;,
  -0.333850;0.622966;0.707430;,
  -0.168516;0.684882;0.708900;,
  0.026023;0.705996;0.707738;,
  0.231596;0.667410;0.707762;,
  0.436307;0.553362;0.709526;,
  0.599609;0.366649;0.711363;,
  0.684218;0.176947;0.707486;,
  0.707365;0.045396;0.705389;,
  0.704558;-0.059564;0.707142;,
  0.681032;-0.180737;0.709598;,
  0.638053;-0.299301;0.709441;,
  0.613065;-0.349884;0.708331;,
  0.701841;0.094965;0.705975;,
  0.696950;0.147476;0.701792;,
  0.687909;0.207112;0.695619;,
  0.666566;0.252501;0.701379;,
  0.635730;0.309551;0.707124;,
  0.588482;0.390444;0.707984;,
  0.520488;0.478339;0.707307;,
  0.443770;0.551669;0.706208;,
  0.334007;0.622964;0.707357;,
  0.168586;0.684944;0.708823;,
  -0.026242;0.705981;0.707744;,
  -0.231734;0.667373;0.707752;,
  -0.435932;0.553436;0.709699;,
  -0.598866;0.366585;0.712022;,
  -0.683848;0.176857;0.707865;,
  -0.707649;0.045100;0.705124;,
  -0.705048;-0.059637;0.706648;,
  -0.681309;-0.180460;0.709403;,
  -0.637938;-0.299359;0.709520;,
  -0.612889;-0.349953;0.708449;;
  176;
  4;36,36,0,0;,
  4;3,3,36,36;,
  4;37,37,1,1;,
  4;4,4,37,37;,
  4;38,38,2,2;,
  4;5,5,38,38;,
  4;129,129,130,130;,
  4;131,131,129,129;,
  4;39,39,3,3;,
  4;6,6,39,39;,
  4;40,40,4,4;,
  4;7,7,40,40;,
  4;41,41,5,5;,
  4;8,8,41,41;,
  4;132,132,131,131;,
  4;133,133,132,132;,
  4;42,42,6,6;,
  4;9,9,42,42;,
  4;43,43,7,7;,
  4;10,10,43,43;,
  4;44,44,8,8;,
  4;11,11,44,44;,
  4;134,134,133,133;,
  4;135,135,134,134;,
  4;45,45,9,9;,
  4;12,12,45,45;,
  4;46,46,10,10;,
  4;13,13,46,46;,
  4;47,47,11,11;,
  4;14,14,47,47;,
  4;136,136,135,135;,
  4;137,137,136,136;,
  4;48,48,12,12;,
  4;15,15,48,48;,
  4;49,49,13,13;,
  4;16,16,49,49;,
  4;50,50,14,14;,
  4;17,17,50,50;,
  4;138,138,137,137;,
  4;139,139,138,138;,
  4;51,51,15,15;,
  4;18,18,51,51;,
  4;52,52,16,16;,
  4;19,19,52,52;,
  4;53,53,17,17;,
  4;20,20,53,53;,
  4;140,140,139,139;,
  4;141,141,140,140;,
  4;54,54,18,18;,
  4;21,21,54,54;,
  4;55,55,19,19;,
  4;22,22,55,55;,
  4;56,56,20,20;,
  4;23,23,56,56;,
  4;142,142,141,141;,
  4;143,143,142,142;,
  4;57,57,21,21;,
  4;24,24,57,57;,
  4;58,58,22,22;,
  4;25,25,58,58;,
  4;59,59,23,23;,
  4;26,26,59,59;,
  4;144,144,143,143;,
  4;145,145,144,144;,
  4;60,60,24,24;,
  4;27,27,60,60;,
  4;61,61,25,25;,
  4;28,28,61,61;,
  4;62,62,26,26;,
  4;29,29,62,62;,
  4;146,146,145,145;,
  4;147,147,146,146;,
  4;63,63,27,27;,
  4;30,30,63,63;,
  4;64,64,28,28;,
  4;31,31,64,64;,
  4;65,65,29,29;,
  4;32,32,65,65;,
  4;148,148,147,147;,
  4;149,149,148,148;,
  4;66,66,30,30;,
  4;33,33,66,66;,
  4;67,67,31,31;,
  4;34,34,67,67;,
  4;68,68,32,32;,
  4;35,35,68,68;,
  4;150,150,149,149;,
  4;151,151,150,150;,
  4;1,37,37,1;,
  4;37,4,4,37;,
  4;0,36,36,0;,
  4;36,3,3,36;,
  4;130,129,129,130;,
  4;129,131,131,129;,
  4;2,38,38,2;,
  4;38,5,5,38;,
  4;4,99,99,4;,
  4;99,69,69,99;,
  4;3,100,100,3;,
  4;100,70,70,100;,
  4;131,101,101,131;,
  4;101,71,71,101;,
  4;5,152,152,5;,
  4;152,153,153,152;,
  4;69,102,102,69;,
  4;102,72,72,102;,
  4;70,103,103,70;,
  4;103,73,73,103;,
  4;71,104,104,71;,
  4;104,74,74,104;,
  4;153,154,154,153;,
  4;154,155,155,154;,
  4;72,105,105,72;,
  4;105,75,75,105;,
  4;73,106,106,73;,
  4;106,76,76,106;,
  4;74,107,107,74;,
  4;107,77,77,107;,
  4;155,156,156,155;,
  4;156,157,157,156;,
  4;75,108,108,75;,
  4;108,78,78,108;,
  4;76,109,109,76;,
  4;109,79,79,109;,
  4;77,110,110,77;,
  4;110,80,80,110;,
  4;157,158,158,157;,
  4;158,159,159,158;,
  4;78,111,111,78;,
  4;111,81,81,111;,
  4;79,112,112,79;,
  4;112,82,82,112;,
  4;80,113,113,80;,
  4;113,83,83,113;,
  4;159,160,160,159;,
  4;160,161,161,160;,
  4;81,114,114,81;,
  4;114,84,84,114;,
  4;82,115,115,82;,
  4;115,85,85,115;,
  4;83,116,116,83;,
  4;116,86,86,116;,
  4;161,162,162,161;,
  4;162,163,163,162;,
  4;84,117,117,84;,
  4;117,87,87,117;,
  4;85,118,118,85;,
  4;118,88,88,118;,
  4;86,119,119,86;,
  4;119,89,89,119;,
  4;163,164,164,163;,
  4;164,165,165,164;,
  4;87,120,120,87;,
  4;120,90,90,120;,
  4;88,121,121,88;,
  4;121,91,91,121;,
  4;89,122,122,89;,
  4;122,92,92,122;,
  4;165,166,166,165;,
  4;166,167,167,166;,
  4;90,123,123,90;,
  4;123,93,93,123;,
  4;91,124,124,91;,
  4;124,94,94,124;,
  4;92,125,125,92;,
  4;125,95,95,125;,
  4;167,168,168,167;,
  4;168,169,169,168;,
  4;93,126,126,93;,
  4;126,96,96,126;,
  4;94,127,127,94;,
  4;127,97,97,127;,
  4;95,128,128,95;,
  4;128,98,98,128;,
  4;169,170,170,169;,
  4;170,171,171,170;;
 }
 MeshTextureCoords {
  184;
  2.605110;5.506430;,
  2.230110;5.517250;,
  2.230110;7.238440;,
  2.605110;7.238440;,
  2.582890;4.015450;,
  2.208700;4.041400;,
  1.855120;5.528070;,
  1.855120;7.238440;,
  1.834510;4.067340;,
  2.230110;5.517250;,
  2.230110;7.238440;,
  2.208700;4.041400;,
  2.516200;3.006510;,
  2.144440;3.056190;,
  2.404490;2.285270;,
  2.037360;2.363050;,
  1.772690;3.105880;,
  1.670230;2.440830;,
  2.144440;3.056190;,
  2.037360;2.363050;,
  2.247180;1.657370;,
  1.887430;1.763360;,
  2.031670;1.046120;,
  1.683000;1.183140;,
  1.527690;1.869350;,
  1.334330;1.320160;,
  1.887430;1.763360;,
  1.683000;1.183140;,
  1.745300;0.374830;,
  1.412360;0.548410;,
  1.386630;-0.274900;,
  1.077470;-0.061020;,
  1.079410;0.721990;,
  0.768300;0.152870;,
  1.412360;0.548410;,
  1.077470;-0.061020;,
  0.954180;-0.821430;,
  0.680270;-0.565300;,
  0.487140;-1.257060;,
  0.261660;-0.958600;,
  0.406360;-0.309170;,
  0.036180;-0.660140;,
  0.680270;-0.565300;,
  0.261660;-0.958600;,
  0.024690;-1.574060;,
  -0.137490;-1.235080;,
  -0.451860;-1.762520;,
  -0.532850;-1.394860;,
  -0.299660;-0.896100;,
  -0.613840;-1.027190;,
  -0.137490;-1.235080;,
  -0.532850;-1.394860;,
  -0.961200;-1.812520;,
  -0.940120;-1.438040;,
  -1.452490;-1.723490;,
  -1.321640;-1.371630;,
  -0.919050;-1.063560;,
  -1.190790;-1.019780;,
  -0.940120;-1.438040;,
  -1.321640;-1.371630;,
  -1.874900;-1.494860;,
  -1.639720;-1.202640;,
  -2.194440;-1.141630;,
  -1.877910;-0.931420;,
  -1.404550;-0.910430;,
  -1.561370;-0.721220;,
  -1.639720;-1.202640;,
  -1.877910;-0.931420;,
  -2.377140;-0.678800;,
  -2.019710;-0.558320;,
  -2.451990;-0.146340;,
  -2.080290;-0.112250;,
  -1.662280;-0.437830;,
  -1.708590;-0.078150;,
  -2.019710;-0.558320;,
  -2.080290;-0.112250;,
  -2.447990;0.415830;,
  -2.074780;0.377880;,
  -2.361620;0.954690;,
  -1.996310;0.859750;,
  -1.701580;0.339940;,
  -1.631000;0.764810;,
  -2.074780;0.377880;,
  -1.996310;0.859750;,
  -2.189370;1.417240;,
  -1.837980;1.281040;,
  -1.953870;1.836860;,
  -1.515330;1.835780;,
  -1.486590;1.144840;,
  -1.076800;1.834700;,
  -1.837980;1.281040;,
  -1.515330;1.835780;,
  2.240400;7.238440;,
  2.240400;5.517250;,
  1.865400;5.506430;,
  1.865400;7.238440;,
  2.261820;4.041400;,
  1.887630;4.015450;,
  2.615400;7.238440;,
  2.615400;5.528070;,
  2.636000;4.067340;,
  2.240400;7.238440;,
  2.240400;5.517250;,
  2.261820;4.041400;,
  2.326070;3.056190;,
  1.954320;3.006510;,
  2.433160;2.363050;,
  2.066030;2.285270;,
  2.697820;3.105880;,
  2.800290;2.440830;,
  2.326070;3.056190;,
  2.433160;2.363050;,
  2.583080;1.763360;,
  2.223330;1.657370;,
  2.787520;1.183140;,
  2.438850;1.046120;,
  2.942830;1.869350;,
  3.136190;1.320160;,
  2.583080;1.763360;,
  2.787520;1.183140;,
  3.058160;0.548410;,
  2.725210;0.374830;,
  3.393050;-0.061020;,
  3.083880;-0.274900;,
  3.391100;0.721990;,
  3.702210;0.152870;,
  3.058160;0.548410;,
  3.393050;-0.061020;,
  3.790240;-0.565300;,
  3.516330;-0.821430;,
  4.208860;-0.958600;,
  3.983380;-1.257060;,
  4.064150;-0.309170;,
  4.434340;-0.660140;,
  3.790240;-0.565300;,
  4.208860;-0.958600;,
  4.608000;-1.235080;,
  4.445830;-1.574060;,
  5.003360;-1.394860;,
  4.922370;-1.762520;,
  4.770170;-0.896100;,
  5.084350;-1.027190;,
  4.608000;-1.235080;,
  5.003360;-1.394860;,
  5.410640;-1.438040;,
  5.431710;-1.812520;,
  5.792150;-1.371630;,
  5.923000;-1.723490;,
  5.389570;-1.063560;,
  5.661310;-1.019780;,
  5.410640;-1.438040;,
  5.792150;-1.371630;,
  6.110240;-1.202640;,
  6.345410;-1.494860;,
  6.348420;-0.931420;,
  6.664960;-1.141630;,
  5.875070;-0.910430;,
  6.031880;-0.721220;,
  6.110240;-1.202640;,
  6.348420;-0.931420;,
  6.490230;-0.558320;,
  6.847660;-0.678800;,
  6.550810;-0.112250;,
  6.922510;-0.146340;,
  6.132800;-0.437830;,
  6.179100;-0.078150;,
  6.490230;-0.558320;,
  6.550810;-0.112250;,
  6.545300;0.377880;,
  6.918500;0.415830;,
  6.466820;0.859750;,
  6.832130;0.954690;,
  6.172090;0.339940;,
  6.101510;0.764810;,
  6.545300;0.377880;,
  6.466820;0.859750;,
  6.308490;1.281040;,
  6.659890;1.417240;,
  5.985850;1.835780;,
  6.424380;1.836860;,
  5.957100;1.144840;,
  5.547310;1.834700;,
  6.308490;1.281040;,
  5.985850;1.835780;;
 }
}