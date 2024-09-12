/// »ç¿ëÇÏÁö ¾Ê´Â ÀÓ½Ã ÆÄÀÏ
//#include "DXBox.h"
//
//
//std::tuple<vector<Vertex>, vector<uint32_t>> MakeSquare() 
//{
//    vector<Vector3> positions;
//    vector<Vector3> colors;
//    vector<Vector3> normals;
//    vector<Vector2> texcoords; // ÅØ½ºÃç ÁÂÇ¥
//
//    const float scale = 1.0f;
//
//    // ¾Õ¸é
//    positions.push_back(Vector3(-1.0f, 1.0f, 0.0f) * scale);
//    positions.push_back(Vector3(1.0f, 1.0f, 0.0f) * scale);
//    positions.push_back(Vector3(1.0f, -1.0f, 0.0f) * scale);
//    positions.push_back(Vector3(-1.0f, -1.0f, 0.0f) * scale);
//    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
//    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
//    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
//    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
//    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
//    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
//    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
//    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
//
//    texcoords.push_back(Vector2(0.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 1.0f));
//    texcoords.push_back(Vector2(0.0f, 1.0f));
//
//    vector<Vertex> boxTexVertices;
//    for (size_t i = 0; i < positions.size(); i++) {
//        Vertex v;
//        v.position = positions[i];
//        v.normal = normals[i];
//        //v.color = colors[i];
//        v.texcoord = texcoords[i];
//        boxTexVertices.push_back(v);
//    }
//    vector<uint32_t> boxTexIndices = {
//        0, 1, 2, 0, 2, 3, // ¾Õ¸é
//    };
//
//    return std::tuple{boxTexVertices, boxTexIndices};
//}
//
//std::tuple<vector<Vertex>, vector<uint32_t>> MakeBox()
//{
//	vector<Vector3> positions;
//	vector<Vector3> colors;
//	vector<Vector3> normals;
//    vector<Vector2> texcoords;
//
//	const float scale = 1.0f;
//
//	// À­¸é
//    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
//    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
//    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
//    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
//	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
//	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
//	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
//	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
//	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
//	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
//    texcoords.push_back(Vector2(0.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 1.0f));
//    texcoords.push_back(Vector2(0.0f, 1.0f));
//
//	// ¾Æ·§¸é
//    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
//    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
//    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
//    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
//	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
//	colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
//	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
//	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
//	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
//	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
//	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
//    texcoords.push_back(Vector2(0.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 1.0f));
//    texcoords.push_back(Vector2(0.0f, 1.0f));
//
//	// ¾Õ¸é
//    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
//    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
//    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
//    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
//	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
//	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
//	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
//	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
//	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
//	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
//	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
//    texcoords.push_back(Vector2(0.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 1.0f));
//    texcoords.push_back(Vector2(0.0f, 1.0f));
//
//	// µÞ¸é
//    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
//    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
//    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
//    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
//	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
//	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
//	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
//    texcoords.push_back(Vector2(0.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 1.0f));
//    texcoords.push_back(Vector2(0.0f, 1.0f));
//
//	// ¿ÞÂÊ
//    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
//    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
//    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
//    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
//	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
//	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
//	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
//	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
//	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
//	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
//	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
//    texcoords.push_back(Vector2(0.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 1.0f));
//    texcoords.push_back(Vector2(0.0f, 1.0f));
//
//	// ¿À¸¥ÂÊ
//    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
//    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
//    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
//    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
//	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
//	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
//	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
//	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
//	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
//	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
//	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
//    texcoords.push_back(Vector2(0.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 0.0f));
//    texcoords.push_back(Vector2(1.0f, 1.0f));
//    texcoords.push_back(Vector2(0.0f, 1.0f));
//
//	std::vector<Vertex> boxVertices;
//	for (size_t i = 0; i < positions.size(); i++) {
//		Vertex v;
//		v.position = positions[i];
//        v.normal = normals[i];
//		//v.color = colors[i];
//        v.texcoord = texcoords[i];
//		boxVertices.push_back(v);
//	}
//
//	std::vector<uint32_t> boxIndices = {
//		0,  1,  2,  0,  2,  3,  // À­¸é
//		4,  5,  7,  7,  5,  6,  // ¾Æ·§¸é
//		8,  9,  10, 8,  10, 11, // ¾Õ¸é
//		12, 13, 14, 12, 14, 15, // µÞ¸é
//		16, 17, 18, 16, 18, 19, // ¿Þ¸é
//		20, 21, 22, 20, 22, 23, // ¿À¸¥¸é
//	};
//
//	return std::tuple/*<std::vector<Vertex>, std::vector<uint32_t>>*/{boxVertices,boxIndices};
//}
