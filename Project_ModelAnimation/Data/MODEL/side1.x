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
 34;
 0.00000;4.75000;-9.50000;,
 3.63549;4.75000;-8.77686;,
 2.72662;0.00000;-6.58264;,
 0.00000;0.00000;-7.12500;,
 6.71751;4.75000;-6.71751;,
 5.03814;0.00000;-5.03814;,
 8.77686;4.75000;-3.63549;,
 6.58264;0.00000;-2.72662;,
 9.50000;4.75000;0.00000;,
 7.12500;0.00000;0.00000;,
 8.77685;4.75000;3.63549;,
 6.58264;0.00000;2.72662;,
 6.71751;4.75000;6.71751;,
 5.03814;0.00000;5.03814;,
 3.63549;4.75000;8.77685;,
 2.72662;0.00000;6.58264;,
 0.00000;4.75000;9.50000;,
 0.00000;0.00000;7.12500;,
 -3.63549;4.75000;8.77686;,
 -2.72662;0.00000;6.58264;,
 -6.71751;4.75000;6.71752;,
 -5.03813;0.00000;5.03814;,
 -8.77685;4.75000;3.63549;,
 -6.58264;0.00000;2.72662;,
 -9.50000;4.75000;-0.00000;,
 -7.12500;0.00000;-0.00000;,
 -8.77685;4.75000;-3.63549;,
 -6.58264;0.00000;-2.72662;,
 -6.71751;4.75000;-6.71752;,
 -5.03813;0.00000;-5.03814;,
 -3.63549;4.75000;-8.77686;,
 -2.72662;0.00000;-6.58264;,
 0.00000;4.75000;-9.50000;,
 0.00000;0.00000;-7.12500;;
 
 16;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 4;20,22,23,21;,
 4;22,24,25,23;,
 4;24,26,27,25;,
 4;26,28,29,27;,
 4;28,30,31,29;,
 4;30,32,33,31;;
 
 MeshMaterialList {
  2;
  16;
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.047059;0.360784;0.901961;1.000000;;
   100.000000;
   1.000000;1.000000;1.000000;;
   0.047059;0.360784;0.901961;;
   TextureFilename {
    "Data\\TEXTURE\\side.png";
   }
  }
  Material {
   0.047059;0.360784;0.901961;1.000000;;
   100.000000;
   1.000000;1.000000;1.000000;;
   0.047059;0.360784;0.901961;;
   TextureFilename {
    "Data\\TEXTURE\\floor.png";
   }
  }
 }
 MeshNormals {
  16;
  0.000000;-0.447214;-0.894427;,
  0.342283;-0.447214;-0.826343;,
  0.632456;-0.447214;-0.632456;,
  0.826343;-0.447214;-0.342282;,
  0.894427;-0.447214;0.000000;,
  0.826343;-0.447214;0.342283;,
  0.632456;-0.447214;0.632456;,
  0.342283;-0.447214;0.826343;,
  0.000000;-0.447214;0.894427;,
  -0.342282;-0.447214;0.826343;,
  -0.632455;-0.447214;0.632456;,
  -0.826343;-0.447214;0.342283;,
  -0.894427;-0.447214;0.000000;,
  -0.826343;-0.447214;-0.342283;,
  -0.632455;-0.447214;-0.632456;,
  -0.342282;-0.447214;-0.826343;;
  16;
  4;0,1,1,0;,
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,9,9,8;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,0,0,15;;
 }
 MeshTextureCoords {
  34;
  0.000000;0.157958;,
  0.062500;0.157958;,
  0.062500;0.880046;,
  0.000000;0.880046;,
  0.125000;0.157958;,
  0.125000;0.880046;,
  0.187500;0.157958;,
  0.187500;0.880046;,
  0.250000;0.157958;,
  0.250000;0.880046;,
  0.312500;0.157958;,
  0.312500;0.880046;,
  0.375000;0.157958;,
  0.375000;0.880046;,
  0.437500;0.157958;,
  0.437500;0.880046;,
  0.500000;0.157958;,
  0.500000;0.880046;,
  0.562500;0.157958;,
  0.562500;0.880046;,
  0.625000;0.157958;,
  0.625000;0.880046;,
  0.687500;0.157958;,
  0.687500;0.880046;,
  0.750000;0.157958;,
  0.750000;0.880046;,
  0.812500;0.157958;,
  0.812500;0.880046;,
  0.875000;0.157958;,
  0.875000;0.880046;,
  0.937500;0.157958;,
  0.937500;0.880046;,
  1.000000;0.157958;,
  1.000000;0.880046;;
 }
}
