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
 28;
 -7.38880;-1.70650;-0.11909;,
 -6.41825;-2.67967;-0.11909;,
 -6.50257;-2.76374;0.00000;,
 -7.47311;-1.79058;0.00000;,
 -7.30448;-1.62243;0.00000;,
 -6.33394;-2.59560;0.00000;,
 -7.38880;-1.70650;0.11909;,
 -6.41825;-2.67967;0.11909;,
 -7.64245;-2.93785;-0.11909;,
 -5.66968;-0.97555;-0.11909;,
 -5.58584;-1.06010;0.00000;,
 -7.55838;-3.02216;0.00000;,
 -7.72629;-2.85329;0.00000;,
 -5.75375;-0.89124;0.00000;,
 -7.64245;-2.93785;0.11909;,
 -5.66968;-0.97555;0.11909;,
 -6.74264;-0.97484;-0.11909;,
 -5.66968;-0.97555;-0.11909;,
 -5.78090;-1.09392;0.00000;,
 -6.74264;-1.09392;0.00000;,
 -6.74264;-0.85575;0.00000;,
 -5.54273;-0.85575;0.00000;,
 -6.74264;-0.97484;0.11909;,
 -5.66968;-0.97555;0.11909;,
 -5.66182;-2.05566;-0.11909;,
 -5.78090;-2.05566;0.00000;,
 -5.54273;-2.05566;0.00000;,
 -5.66182;-2.05566;0.11909;;
 
 22;
 4;0,1,2,3;,
 4;4,5,1,0;,
 4;6,7,5,4;,
 4;3,2,7,6;,
 4;0,3,6,4;,
 4;7,2,1,5;,
 4;8,9,10,11;,
 4;12,13,9,8;,
 4;14,15,13,12;,
 4;11,10,15,14;,
 4;8,11,14,12;,
 4;15,10,9,13;,
 4;16,17,18,19;,
 4;20,21,17,16;,
 4;22,23,21,20;,
 4;19,18,23,22;,
 4;17,24,25,18;,
 4;21,26,24,17;,
 4;23,27,26,21;,
 4;18,25,27,23;,
 4;16,19,22,20;,
 4;27,25,24,26;;
 
 MeshMaterialList {
  2;
  22;
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
  28;
  -0.500716;-0.499368;-0.707047;,
  0.500724;0.499374;-0.707037;,
  0.500724;0.499374;0.707037;,
  0.498716;-0.501378;-0.707037;,
  -0.498708;0.501372;-0.707048;,
  -0.498708;0.501372;0.707048;,
  0.438233;-0.442887;-0.782178;,
  -0.447423;0.443211;0.776773;,
  -0.000235;-0.708083;-0.706129;,
  0.000234;0.706055;-0.708157;,
  0.000234;0.706055;0.708157;,
  0.400392;0.407682;-0.820659;,
  -0.717753;-0.002650;-0.696293;,
  0.695724;0.002573;-0.718305;,
  0.695724;0.002573;0.718305;,
  -0.500716;-0.499368;0.707047;,
  -0.706098;0.708114;0.000000;,
  0.706077;-0.708135;-0.000000;,
  -0.447423;0.443211;-0.776773;,
  0.498716;-0.501378;0.707037;,
  0.438233;-0.442887;0.782178;,
  -0.709119;-0.705088;0.000000;,
  0.709099;0.705109;0.000000;,
  0.400392;0.407682;0.820659;,
  -0.000235;-0.708083;0.706129;,
  -0.717753;-0.002650;0.696293;,
  -1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  22;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;3,6,3,3;,
  4;4,4,18,4;,
  4;5,7,5,5;,
  4;19,19,20,19;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;8,6,8,8;,
  4;9,11,18,9;,
  4;10,7,23,10;,
  4;24,24,20,24;,
  4;18,12,12,12;,
  4;11,13,13,6;,
  4;20,14,14,23;,
  4;25,25,25,7;,
  4;26,26,26,26;,
  4;27,27,27,27;;
 }
 MeshTextureCoords {
  28;
  -2.037610;-1.043120;,
  2.037610;3.043120;,
  1.683580;3.396190;,
  -2.391640;-0.690040;,
  -1.683580;-1.396200;,
  2.391640;2.690040;,
  -2.037610;-1.043120;,
  2.037610;3.043120;,
  -4.141310;5.119280;,
  4.141310;-3.119280;,
  4.493920;-2.764790;,
  -3.788700;5.473780;,
  -4.493920;4.764790;,
  3.788700;-3.473780;,
  -4.141310;5.119280;,
  4.141310;-3.119280;,
  -3.025210;-0.512610;,
  1.512610;-0.512610;,
  1.012610;-0.012610;,
  -3.025210;-0.012610;,
  -3.025210;-1.012610;,
  2.012610;-1.012610;,
  -3.025210;-0.512610;,
  1.512610;-0.512610;,
  1.512610;4.025210;,
  1.012610;4.025210;,
  2.012610;4.025210;,
  1.512610;4.025210;;
 }
}