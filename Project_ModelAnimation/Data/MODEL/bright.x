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
 167;
 0.00000;10.00000;0.00000;,
 1.40866;9.59493;-2.43988;,
 0.00000;9.59493;-2.81733;,
 0.00000;10.00000;0.00000;,
 2.43988;9.59493;-1.40866;,
 0.00000;10.00000;0.00000;,
 2.81733;9.59493;0.00000;,
 0.00000;10.00000;0.00000;,
 2.43988;9.59493;1.40866;,
 0.00000;10.00000;0.00000;,
 1.40866;9.59493;2.43988;,
 0.00000;10.00000;0.00000;,
 0.00000;9.59493;2.81733;,
 0.00000;10.00000;0.00000;,
 -1.40866;9.59493;2.43988;,
 0.00000;10.00000;0.00000;,
 -2.43988;9.59493;1.40866;,
 0.00000;10.00000;0.00000;,
 -2.81733;9.59493;0.00000;,
 0.00000;10.00000;0.00000;,
 -2.43988;9.59493;-1.40866;,
 0.00000;10.00000;0.00000;,
 -1.40866;9.59493;-2.43987;,
 0.00000;10.00000;0.00000;,
 0.00000;9.59493;-2.81733;,
 2.70320;8.41254;-4.68209;,
 0.00000;8.41254;-5.40641;,
 4.68209;8.41254;-2.70320;,
 5.40641;8.41254;0.00000;,
 4.68209;8.41254;2.70320;,
 2.70320;8.41254;4.68209;,
 0.00000;8.41254;5.40641;,
 -2.70320;8.41254;4.68209;,
 -4.68209;8.41254;2.70321;,
 -5.40641;8.41254;0.00000;,
 -4.68209;8.41254;-2.70320;,
 -2.70321;8.41254;-4.68209;,
 0.00000;8.41254;-5.40641;,
 3.77875;6.54861;-6.54498;,
 0.00000;6.54861;-7.55750;,
 6.54498;6.54861;-3.77875;,
 7.55750;6.54861;0.00000;,
 6.54498;6.54861;3.77875;,
 3.77875;6.54861;6.54498;,
 0.00000;6.54861;7.55750;,
 -3.77875;6.54861;6.54498;,
 -6.54498;6.54861;3.77875;,
 -7.55750;6.54861;0.00000;,
 -6.54499;6.54861;-3.77874;,
 -3.77875;6.54861;-6.54498;,
 0.00000;6.54861;-7.55750;,
 4.54816;4.15415;-7.87764;,
 0.00000;4.15415;-9.09632;,
 7.87764;4.15415;-4.54816;,
 9.09632;4.15415;0.00000;,
 7.87764;4.15415;4.54816;,
 4.54816;4.15415;7.87764;,
 0.00000;4.15415;9.09632;,
 -4.54816;4.15415;7.87765;,
 -7.87764;4.15415;4.54816;,
 -9.09632;4.15415;0.00000;,
 -7.87765;4.15415;-4.54816;,
 -4.54817;4.15415;-7.87764;,
 0.00000;4.15415;-9.09632;,
 4.94911;1.42315;-8.57211;,
 0.00000;1.42315;-9.89821;,
 8.57211;1.42315;-4.94911;,
 9.89821;1.42315;0.00000;,
 8.57210;1.42315;4.94911;,
 4.94911;1.42315;8.57210;,
 0.00000;1.42315;9.89821;,
 -4.94911;1.42315;8.57211;,
 -8.57210;1.42315;4.94911;,
 -9.89821;1.42315;0.00000;,
 -8.57211;1.42315;-4.94910;,
 -4.94911;1.42315;-8.57210;,
 0.00000;1.42315;-9.89821;,
 4.94911;-1.42315;-8.57211;,
 0.00000;-1.42315;-9.89821;,
 8.57211;-1.42315;-4.94911;,
 9.89821;-1.42315;0.00000;,
 8.57210;-1.42315;4.94911;,
 4.94911;-1.42315;8.57210;,
 0.00000;-1.42315;9.89821;,
 -4.94911;-1.42315;8.57211;,
 -8.57210;-1.42315;4.94911;,
 -9.89821;-1.42315;0.00000;,
 -8.57211;-1.42315;-4.94910;,
 -4.94911;-1.42315;-8.57210;,
 0.00000;-1.42315;-9.89821;,
 4.54816;-4.15415;-7.87764;,
 0.00000;-4.15415;-9.09632;,
 7.87764;-4.15415;-4.54816;,
 9.09632;-4.15415;0.00000;,
 7.87764;-4.15415;4.54816;,
 4.54816;-4.15415;7.87764;,
 0.00000;-4.15415;9.09632;,
 -4.54816;-4.15415;7.87765;,
 -7.87764;-4.15415;4.54816;,
 -9.09632;-4.15415;0.00000;,
 -7.87765;-4.15415;-4.54816;,
 -4.54817;-4.15415;-7.87764;,
 0.00000;-4.15415;-9.09632;,
 3.77875;-6.54861;-6.54498;,
 0.00000;-6.54861;-7.55749;,
 6.54498;-6.54861;-3.77875;,
 7.55749;-6.54861;0.00000;,
 6.54498;-6.54861;3.77875;,
 3.77875;-6.54861;6.54498;,
 0.00000;-6.54861;7.55749;,
 -3.77875;-6.54861;6.54498;,
 -6.54498;-6.54861;3.77875;,
 -7.55749;-6.54861;0.00000;,
 -6.54498;-6.54861;-3.77874;,
 -3.77875;-6.54861;-6.54498;,
 0.00000;-6.54861;-7.55749;,
 2.70320;-8.41254;-4.68209;,
 0.00000;-8.41254;-5.40641;,
 4.68209;-8.41254;-2.70320;,
 5.40641;-8.41254;0.00000;,
 4.68209;-8.41254;2.70320;,
 2.70320;-8.41254;4.68209;,
 0.00000;-8.41254;5.40641;,
 -2.70320;-8.41254;4.68209;,
 -4.68209;-8.41254;2.70321;,
 -5.40641;-8.41254;0.00000;,
 -4.68209;-8.41254;-2.70320;,
 -2.70321;-8.41254;-4.68208;,
 0.00000;-8.41254;-5.40641;,
 1.40866;-9.59493;-2.43988;,
 0.00000;-9.59493;-2.81733;,
 2.43988;-9.59493;-1.40866;,
 2.81733;-9.59493;0.00000;,
 2.43988;-9.59493;1.40866;,
 1.40866;-9.59493;2.43988;,
 0.00000;-9.59493;2.81733;,
 -1.40866;-9.59493;2.43988;,
 -2.43988;-9.59493;1.40866;,
 -2.81733;-9.59493;0.00000;,
 -2.43988;-9.59493;-1.40866;,
 -1.40866;-9.59493;-2.43988;,
 0.00000;-9.59493;-2.81733;,
 0.00000;-9.59493;-2.81733;,
 1.40866;-9.59493;-2.43988;,
 0.00000;-10.00000;-0.00000;,
 2.43988;-9.59493;-1.40866;,
 0.00000;-10.00000;-0.00000;,
 2.81733;-9.59493;0.00000;,
 0.00000;-10.00000;-0.00000;,
 2.43988;-9.59493;1.40866;,
 0.00000;-10.00000;-0.00000;,
 1.40866;-9.59493;2.43988;,
 0.00000;-10.00000;-0.00000;,
 0.00000;-9.59493;2.81733;,
 0.00000;-10.00000;-0.00000;,
 -1.40866;-9.59493;2.43988;,
 0.00000;-10.00000;-0.00000;,
 -2.43988;-9.59493;1.40866;,
 0.00000;-10.00000;-0.00000;,
 -2.81733;-9.59493;0.00000;,
 0.00000;-10.00000;-0.00000;,
 -2.43988;-9.59493;-1.40866;,
 0.00000;-10.00000;-0.00000;,
 -1.40866;-9.59493;-2.43988;,
 0.00000;-10.00000;-0.00000;,
 0.00000;-9.59493;-2.81733;,
 0.00000;-10.00000;-0.00000;;
 
 132;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 3;17,18,16;,
 3;19,20,18;,
 3;21,22,20;,
 3;23,24,22;,
 4;2,1,25,26;,
 4;1,4,27,25;,
 4;4,6,28,27;,
 4;6,8,29,28;,
 4;8,10,30,29;,
 4;10,12,31,30;,
 4;12,14,32,31;,
 4;14,16,33,32;,
 4;16,18,34,33;,
 4;18,20,35,34;,
 4;20,22,36,35;,
 4;22,24,37,36;,
 4;26,25,38,39;,
 4;25,27,40,38;,
 4;27,28,41,40;,
 4;28,29,42,41;,
 4;29,30,43,42;,
 4;30,31,44,43;,
 4;31,32,45,44;,
 4;32,33,46,45;,
 4;33,34,47,46;,
 4;34,35,48,47;,
 4;35,36,49,48;,
 4;36,37,50,49;,
 4;39,38,51,52;,
 4;38,40,53,51;,
 4;40,41,54,53;,
 4;41,42,55,54;,
 4;42,43,56,55;,
 4;43,44,57,56;,
 4;44,45,58,57;,
 4;45,46,59,58;,
 4;46,47,60,59;,
 4;47,48,61,60;,
 4;48,49,62,61;,
 4;49,50,63,62;,
 4;52,51,64,65;,
 4;51,53,66,64;,
 4;53,54,67,66;,
 4;54,55,68,67;,
 4;55,56,69,68;,
 4;56,57,70,69;,
 4;57,58,71,70;,
 4;58,59,72,71;,
 4;59,60,73,72;,
 4;60,61,74,73;,
 4;61,62,75,74;,
 4;62,63,76,75;,
 4;65,64,77,78;,
 4;64,66,79,77;,
 4;66,67,80,79;,
 4;67,68,81,80;,
 4;68,69,82,81;,
 4;69,70,83,82;,
 4;70,71,84,83;,
 4;71,72,85,84;,
 4;72,73,86,85;,
 4;73,74,87,86;,
 4;74,75,88,87;,
 4;75,76,89,88;,
 4;78,77,90,91;,
 4;77,79,92,90;,
 4;79,80,93,92;,
 4;80,81,94,93;,
 4;81,82,95,94;,
 4;82,83,96,95;,
 4;83,84,97,96;,
 4;84,85,98,97;,
 4;85,86,99,98;,
 4;86,87,100,99;,
 4;87,88,101,100;,
 4;88,89,102,101;,
 4;91,90,103,104;,
 4;90,92,105,103;,
 4;92,93,106,105;,
 4;93,94,107,106;,
 4;94,95,108,107;,
 4;95,96,109,108;,
 4;96,97,110,109;,
 4;97,98,111,110;,
 4;98,99,112,111;,
 4;99,100,113,112;,
 4;100,101,114,113;,
 4;101,102,115,114;,
 4;104,103,116,117;,
 4;103,105,118,116;,
 4;105,106,119,118;,
 4;106,107,120,119;,
 4;107,108,121,120;,
 4;108,109,122,121;,
 4;109,110,123,122;,
 4;110,111,124,123;,
 4;111,112,125,124;,
 4;112,113,126,125;,
 4;113,114,127,126;,
 4;114,115,128,127;,
 4;117,116,129,130;,
 4;116,118,131,129;,
 4;118,119,132,131;,
 4;119,120,133,132;,
 4;120,121,134,133;,
 4;121,122,135,134;,
 4;122,123,136,135;,
 4;123,124,137,136;,
 4;124,125,138,137;,
 4;125,126,139,138;,
 4;126,127,140,139;,
 4;127,128,141,140;,
 3;142,143,144;,
 3;143,145,146;,
 3;145,147,148;,
 3;147,149,150;,
 3;149,151,152;,
 3;151,153,154;,
 3;153,155,156;,
 3;155,157,158;,
 3;157,159,160;,
 3;159,161,162;,
 3;161,163,164;,
 3;163,165,166;;
 
 MeshMaterialList {
  1;
  132;
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
   1.000000;1.000000;1.000000;0.600000;;
   100.000000;
   1.000000;1.000000;1.000000;;
   1.000000;1.000000;1.000000;;
  }
 }
 MeshNormals {
  122;
  0.000000;1.000000;0.000000;,
  -0.000000;0.959603;-0.281358;,
  0.140679;0.959603;-0.243663;,
  0.243663;0.959603;-0.140679;,
  0.281358;0.959603;0.000000;,
  0.243663;0.959603;0.140679;,
  0.140679;0.959603;0.243663;,
  0.000000;0.959603;0.281358;,
  -0.140679;0.959603;0.243663;,
  -0.243663;0.959603;0.140679;,
  -0.281358;0.959603;0.000000;,
  -0.243663;0.959603;-0.140679;,
  -0.140679;0.959603;-0.243663;,
  -0.000000;0.841603;-0.540096;,
  0.270048;0.841603;-0.467737;,
  0.467737;0.841603;-0.270048;,
  0.540096;0.841603;0.000000;,
  0.467737;0.841603;0.270048;,
  0.270048;0.841603;0.467737;,
  0.000000;0.841603;0.540096;,
  -0.270048;0.841603;0.467737;,
  -0.467737;0.841603;0.270048;,
  -0.540096;0.841603;0.000000;,
  -0.467737;0.841603;-0.270048;,
  -0.270048;0.841603;-0.467737;,
  -0.000000;0.655383;-0.755297;,
  0.377648;0.655383;-0.654106;,
  0.654106;0.655383;-0.377648;,
  0.755297;0.655383;0.000000;,
  0.654106;0.655383;0.377648;,
  0.377648;0.655383;0.654106;,
  0.000000;0.655383;0.755297;,
  -0.377648;0.655383;0.654106;,
  -0.654106;0.655383;0.377649;,
  -0.755297;0.655383;0.000000;,
  -0.654106;0.655383;-0.377648;,
  -0.377649;0.655383;-0.654106;,
  -0.000000;0.415887;-0.909416;,
  0.454708;0.415887;-0.787578;,
  0.787578;0.415887;-0.454708;,
  0.909416;0.415887;0.000000;,
  0.787578;0.415887;0.454708;,
  0.454708;0.415887;0.787578;,
  0.000000;0.415887;0.909416;,
  -0.454708;0.415887;0.787578;,
  -0.787577;0.415887;0.454708;,
  -0.909416;0.415887;0.000000;,
  -0.787578;0.415887;-0.454708;,
  -0.454709;0.415887;-0.787577;,
  -0.000000;0.142504;-0.989794;,
  0.494897;0.142505;-0.857187;,
  0.857187;0.142504;-0.494897;,
  0.989794;0.142504;0.000000;,
  0.857187;0.142504;0.494897;,
  0.494897;0.142504;0.857187;,
  0.000000;0.142504;0.989794;,
  -0.494897;0.142504;0.857187;,
  -0.857187;0.142504;0.494897;,
  -0.989794;0.142504;0.000001;,
  -0.857187;0.142504;-0.494897;,
  -0.494898;0.142504;-0.857187;,
  -0.000000;-0.142505;-0.989794;,
  0.494897;-0.142505;-0.857187;,
  0.857187;-0.142505;-0.494897;,
  0.989794;-0.142505;0.000000;,
  0.857187;-0.142505;0.494897;,
  0.494897;-0.142505;0.857187;,
  0.000000;-0.142505;0.989794;,
  -0.494897;-0.142505;0.857187;,
  -0.857187;-0.142504;0.494897;,
  -0.989794;-0.142505;0.000000;,
  -0.857187;-0.142505;-0.494897;,
  -0.494897;-0.142505;-0.857187;,
  -0.000000;-0.415887;-0.909416;,
  0.454708;-0.415887;-0.787578;,
  0.787578;-0.415887;-0.454708;,
  0.909416;-0.415887;0.000000;,
  0.787578;-0.415887;0.454708;,
  0.454708;-0.415887;0.787577;,
  0.000000;-0.415887;0.909416;,
  -0.454708;-0.415887;0.787578;,
  -0.787577;-0.415887;0.454708;,
  -0.909416;-0.415887;0.000000;,
  -0.787578;-0.415887;-0.454708;,
  -0.454708;-0.415887;-0.787577;,
  -0.000000;-0.655383;-0.755296;,
  0.377648;-0.655383;-0.654106;,
  0.654106;-0.655383;-0.377648;,
  0.755297;-0.655383;0.000000;,
  0.654106;-0.655383;0.377648;,
  0.377648;-0.655383;0.654106;,
  0.000000;-0.655383;0.755297;,
  -0.377648;-0.655383;0.654106;,
  -0.654106;-0.655383;0.377648;,
  -0.755296;-0.655383;0.000000;,
  -0.654106;-0.655383;-0.377648;,
  -0.377649;-0.655383;-0.654106;,
  -0.000000;-0.841603;-0.540096;,
  0.270048;-0.841603;-0.467737;,
  0.467737;-0.841603;-0.270048;,
  0.540096;-0.841603;0.000000;,
  0.467737;-0.841603;0.270048;,
  0.270048;-0.841603;0.467737;,
  0.000000;-0.841603;0.540096;,
  -0.270048;-0.841603;0.467737;,
  -0.467737;-0.841603;0.270048;,
  -0.540096;-0.841603;0.000000;,
  -0.467737;-0.841603;-0.270048;,
  -0.270048;-0.841603;-0.467737;,
  -0.000000;-0.959603;-0.281358;,
  0.140679;-0.959603;-0.243663;,
  0.243663;-0.959603;-0.140679;,
  0.281358;-0.959603;-0.000000;,
  0.243663;-0.959603;0.140679;,
  0.140679;-0.959603;0.243663;,
  0.000000;-0.959603;0.281358;,
  -0.140679;-0.959603;0.243663;,
  -0.243663;-0.959603;0.140679;,
  -0.281358;-0.959603;0.000000;,
  -0.243663;-0.959603;-0.140679;,
  -0.140679;-0.959603;-0.243663;,
  -0.000000;-1.000000;0.000000;;
  132;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,9,8;,
  3;0,10,9;,
  3;0,11,10;,
  3;0,12,11;,
  3;0,1,12;,
  4;1,2,14,13;,
  4;2,3,15,14;,
  4;3,4,16,15;,
  4;4,5,17,16;,
  4;5,6,18,17;,
  4;6,7,19,18;,
  4;7,8,20,19;,
  4;8,9,21,20;,
  4;9,10,22,21;,
  4;10,11,23,22;,
  4;11,12,24,23;,
  4;12,1,13,24;,
  4;13,14,26,25;,
  4;14,15,27,26;,
  4;15,16,28,27;,
  4;16,17,29,28;,
  4;17,18,30,29;,
  4;18,19,31,30;,
  4;19,20,32,31;,
  4;20,21,33,32;,
  4;21,22,34,33;,
  4;22,23,35,34;,
  4;23,24,36,35;,
  4;24,13,25,36;,
  4;25,26,38,37;,
  4;26,27,39,38;,
  4;27,28,40,39;,
  4;28,29,41,40;,
  4;29,30,42,41;,
  4;30,31,43,42;,
  4;31,32,44,43;,
  4;32,33,45,44;,
  4;33,34,46,45;,
  4;34,35,47,46;,
  4;35,36,48,47;,
  4;36,25,37,48;,
  4;37,38,50,49;,
  4;38,39,51,50;,
  4;39,40,52,51;,
  4;40,41,53,52;,
  4;41,42,54,53;,
  4;42,43,55,54;,
  4;43,44,56,55;,
  4;44,45,57,56;,
  4;45,46,58,57;,
  4;46,47,59,58;,
  4;47,48,60,59;,
  4;48,37,49,60;,
  4;49,50,62,61;,
  4;50,51,63,62;,
  4;51,52,64,63;,
  4;52,53,65,64;,
  4;53,54,66,65;,
  4;54,55,67,66;,
  4;55,56,68,67;,
  4;56,57,69,68;,
  4;57,58,70,69;,
  4;58,59,71,70;,
  4;59,60,72,71;,
  4;60,49,61,72;,
  4;61,62,74,73;,
  4;62,63,75,74;,
  4;63,64,76,75;,
  4;64,65,77,76;,
  4;65,66,78,77;,
  4;66,67,79,78;,
  4;67,68,80,79;,
  4;68,69,81,80;,
  4;69,70,82,81;,
  4;70,71,83,82;,
  4;71,72,84,83;,
  4;72,61,73,84;,
  4;73,74,86,85;,
  4;74,75,87,86;,
  4;75,76,88,87;,
  4;76,77,89,88;,
  4;77,78,90,89;,
  4;78,79,91,90;,
  4;79,80,92,91;,
  4;80,81,93,92;,
  4;81,82,94,93;,
  4;82,83,95,94;,
  4;83,84,96,95;,
  4;84,73,85,96;,
  4;85,86,98,97;,
  4;86,87,99,98;,
  4;87,88,100,99;,
  4;88,89,101,100;,
  4;89,90,102,101;,
  4;90,91,103,102;,
  4;91,92,104,103;,
  4;92,93,105,104;,
  4;93,94,106,105;,
  4;94,95,107,106;,
  4;95,96,108,107;,
  4;96,85,97,108;,
  4;97,98,110,109;,
  4;98,99,111,110;,
  4;99,100,112,111;,
  4;100,101,113,112;,
  4;101,102,114,113;,
  4;102,103,115,114;,
  4;103,104,116,115;,
  4;104,105,117,116;,
  4;105,106,118,117;,
  4;106,107,119,118;,
  4;107,108,120,119;,
  4;108,97,109,120;,
  3;109,110,121;,
  3;110,111,121;,
  3;111,112,121;,
  3;112,113,121;,
  3;113,114,121;,
  3;114,115,121;,
  3;115,116,121;,
  3;116,117,121;,
  3;117,118,121;,
  3;118,119,121;,
  3;119,120,121;,
  3;120,109,121;;
 }
 MeshTextureCoords {
  167;
  0.041667;0.000000;,
  0.083333;0.090909;,
  0.000000;0.090909;,
  0.125000;0.000000;,
  0.166667;0.090909;,
  0.208333;0.000000;,
  0.250000;0.090909;,
  0.291667;0.000000;,
  0.333333;0.090909;,
  0.375000;0.000000;,
  0.416667;0.090909;,
  0.458333;0.000000;,
  0.500000;0.090909;,
  0.541667;0.000000;,
  0.583333;0.090909;,
  0.625000;0.000000;,
  0.666667;0.090909;,
  0.708333;0.000000;,
  0.750000;0.090909;,
  0.791667;0.000000;,
  0.833333;0.090909;,
  0.875000;0.000000;,
  0.916667;0.090909;,
  0.958333;0.000000;,
  1.000000;0.090909;,
  0.083333;0.181818;,
  0.000000;0.181818;,
  0.166667;0.181818;,
  0.250000;0.181818;,
  0.333333;0.181818;,
  0.416667;0.181818;,
  0.500000;0.181818;,
  0.583333;0.181818;,
  0.666667;0.181818;,
  0.750000;0.181818;,
  0.833333;0.181818;,
  0.916667;0.181818;,
  1.000000;0.181818;,
  0.083333;0.272727;,
  0.000000;0.272727;,
  0.166667;0.272727;,
  0.250000;0.272727;,
  0.333333;0.272727;,
  0.416667;0.272727;,
  0.500000;0.272727;,
  0.583333;0.272727;,
  0.666667;0.272727;,
  0.750000;0.272727;,
  0.833333;0.272727;,
  0.916667;0.272727;,
  1.000000;0.272727;,
  0.083333;0.363636;,
  0.000000;0.363636;,
  0.166667;0.363636;,
  0.250000;0.363636;,
  0.333333;0.363636;,
  0.416667;0.363636;,
  0.500000;0.363636;,
  0.583333;0.363636;,
  0.666667;0.363636;,
  0.750000;0.363636;,
  0.833333;0.363636;,
  0.916667;0.363636;,
  1.000000;0.363636;,
  0.083333;0.454545;,
  0.000000;0.454545;,
  0.166667;0.454545;,
  0.250000;0.454545;,
  0.333333;0.454545;,
  0.416667;0.454545;,
  0.500000;0.454545;,
  0.583333;0.454545;,
  0.666667;0.454545;,
  0.750000;0.454545;,
  0.833333;0.454545;,
  0.916667;0.454545;,
  1.000000;0.454545;,
  0.083333;0.545455;,
  0.000000;0.545455;,
  0.166667;0.545455;,
  0.250000;0.545455;,
  0.333333;0.545455;,
  0.416667;0.545455;,
  0.500000;0.545455;,
  0.583333;0.545455;,
  0.666667;0.545455;,
  0.750000;0.545455;,
  0.833333;0.545455;,
  0.916667;0.545455;,
  1.000000;0.545455;,
  0.083333;0.636364;,
  0.000000;0.636364;,
  0.166667;0.636364;,
  0.250000;0.636364;,
  0.333333;0.636364;,
  0.416667;0.636364;,
  0.500000;0.636364;,
  0.583333;0.636364;,
  0.666667;0.636364;,
  0.750000;0.636364;,
  0.833333;0.636364;,
  0.916667;0.636364;,
  1.000000;0.636364;,
  0.083333;0.727273;,
  0.000000;0.727273;,
  0.166667;0.727273;,
  0.250000;0.727273;,
  0.333333;0.727273;,
  0.416667;0.727273;,
  0.500000;0.727273;,
  0.583333;0.727273;,
  0.666667;0.727273;,
  0.750000;0.727273;,
  0.833333;0.727273;,
  0.916667;0.727273;,
  1.000000;0.727273;,
  0.083333;0.818182;,
  0.000000;0.818182;,
  0.166667;0.818182;,
  0.250000;0.818182;,
  0.333333;0.818182;,
  0.416667;0.818182;,
  0.500000;0.818182;,
  0.583333;0.818182;,
  0.666667;0.818182;,
  0.750000;0.818182;,
  0.833333;0.818182;,
  0.916667;0.818182;,
  1.000000;0.818182;,
  0.083333;0.909091;,
  0.000000;0.909091;,
  0.166667;0.909091;,
  0.250000;0.909091;,
  0.333333;0.909091;,
  0.416667;0.909091;,
  0.500000;0.909091;,
  0.583333;0.909091;,
  0.666667;0.909091;,
  0.750000;0.909091;,
  0.833333;0.909091;,
  0.916667;0.909091;,
  1.000000;0.909091;,
  0.000000;0.909091;,
  0.083333;0.909091;,
  0.041667;1.000000;,
  0.166667;0.909091;,
  0.125000;1.000000;,
  0.250000;0.909091;,
  0.208333;1.000000;,
  0.333333;0.909091;,
  0.291667;1.000000;,
  0.416667;0.909091;,
  0.375000;1.000000;,
  0.500000;0.909091;,
  0.458333;1.000000;,
  0.583333;0.909091;,
  0.541667;1.000000;,
  0.666667;0.909091;,
  0.625000;1.000000;,
  0.750000;0.909091;,
  0.708333;1.000000;,
  0.833333;0.909091;,
  0.791667;1.000000;,
  0.916667;0.909091;,
  0.875000;1.000000;,
  1.000000;0.909091;,
  0.958333;1.000000;;
 }
}