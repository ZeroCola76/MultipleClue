#include "Converter.h"
#include "FileData.h"
#include "GameObject.h"
#include <iostream> // 디버깅을 위한 거라 나중에 지울 것

NestGraphics::MeshData GameObject::MakeSquare(const float scale)
{
	vector<Vector3> positions;
	vector<Vector3> colors;
	vector<Vector3> normals;
	vector<Vector2> texcoords; // 텍스춰 좌표

	// 앞면
	positions.push_back(Vector3(-1.0f, 1.0f, 0.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, 0.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, 0.0f) * scale);
	positions.push_back(Vector3(-1.0f, -1.0f, 0.0f) * scale);
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	NestGraphics::MeshData meshData;

	for (size_t i = 0; i < positions.size(); i++)
	{
		NestGraphics::Vertex v;
		v.position = positions[i];
		v.normal = normals[i];
		v.texcoord = texcoords[i];
		// v.color = colors[i];
		meshData.vertices.push_back(v);
	}
	meshData.indices =
	{
		0, 1, 2, 0, 2, 3, // 앞면
	};
	return meshData;
}

NestGraphics::MeshData GameObject::MakeBox(const float scale)
{
	vector<Vector3> positions;
	vector<Vector3> colors;
	vector<Vector3> normals;
	vector<Vector2> texcoords;

	// 윗면
	positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
	positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// 아랫면
	positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
	positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// 앞면
	positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
	positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// 뒷면
	positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
	positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// 왼쪽
	positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
	positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
	positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
	positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	// 오른쪽
	positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
	positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
	positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
	colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
	colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
	texcoords.push_back(Vector2(0.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 0.0f));
	texcoords.push_back(Vector2(1.0f, 1.0f));
	texcoords.push_back(Vector2(0.0f, 1.0f));

	NestGraphics::MeshData meshData;
	for (size_t i = 0; i < positions.size(); i++)
	{
		NestGraphics::Vertex v;
		v.position = positions[i];
		v.normal = normals[i];
		v.texcoord = texcoords[i];
		// v.color = colors[i];
		meshData.vertices.push_back(v);
	}

	meshData.indices =
	{
		0,  1,  2,  0,  2,  3,  // 윗면
		4,  5,  7,  7,  5,  6,  // 아랫면
		8,  9,  10, 8,  10, 11, // 앞면
		12, 13, 14, 12, 14, 15, // 뒷면
		16, 17, 18, 16, 18, 19, // 왼면
		20, 21, 22, 20, 22, 23, // 오른면
	};

	return meshData;
}

NestGraphics::MeshData GameObject::MakeGrid(const float width, const float height, const int numSlices,
	const int numStacks, const float curve)
{
	const float dx = width / numSlices;
	const float dy = height / numStacks;

	NestGraphics::MeshData meshData;

	vector<NestGraphics::Vertex>& vertices = meshData.vertices;
	vector<uint16_t>& indices = meshData.indices;

	Vector3 leftBottomStartPoint = Vector3(-0.5f * width, -0.5f * height, 0.0f);
	for (int j = 0; j < numStacks + 1; j++)
	{
		// x-y 평면에서 시작점을 y 방향으로 이동
		Vector3 stackStartPoint = Vector3::Transform(
			leftBottomStartPoint, Matrix::CreateTranslation(Vector3(0.0f, dy * float(j), 0.0f)));

		for (int i = 0; i < numSlices + 1; i++)
		{
			NestGraphics::Vertex v;
			// x-y 평면에서 시작점을 x 방향으로 이동
			v.position += Vector3::Transform(
				stackStartPoint, Matrix::CreateTranslation(Vector3(dx * float(i), 0.0f, 0.0f)));
			v.position.z = sin(i * dx * 10.0f) * curve;
			v.normal = Vector3(0.0f, 0.0f, -1.0f);
			v.texcoord = Vector2(float(j) / numSlices, 1.0f - float(i) / numStacks);

			vertices.push_back(v);
		}
	}

	// 인덱스를 넣어보자.
	for (int idxY = 0; idxY < numStacks; idxY++)
	{
		const int offset = (numSlices + 1) * idxY;
		for (int idx = 0; idx < numSlices; idx++)
		{
			indices.push_back(offset + idx);
			indices.push_back(offset + idx + numSlices + 1);
			indices.push_back(offset + idx + numSlices + 2);

			indices.push_back(offset + idx);
			indices.push_back(offset + idx + numSlices + 2);
			indices.push_back(offset + idx + 1);
		}
	}

	///디버깅용
	//std::cout << vertices.size() << std::endl;
	//for (int i = 0; i < vertices.size(); i++)
	//{
	//	if ((i + 1) % 6 != 0)
	//	{
	//		std::cout << "[" << vertices[i].position.x << "," << vertices[i].position.y << ","
	//			<< vertices[i].position.z << "] ";
	//	}
	//	else
	//	{
	//		std::cout << "[" << vertices[i].position.x << "," << vertices[i].position.y << ","
	//			<< vertices[i].position.z << "] " << std::endl;
	//	}

	//}

	//std::cout << indices.size() << std::endl;
	//for (int i = 0; i < indices.size(); i++)
	//{
	//	if ((i + 1) % 3 != 0) std::cout << indices[i] << " ";
	//	else std::cout << indices[i] << std::endl;
	//}

	return meshData;
}

NestGraphics::MeshData GameObject::MakeCylinder(const float bottomRadius, const float topRadius, const float height,
	const int numSlices, const int numStacks)
{
	const float dTheta = DirectX::XM_2PI / numSlices;
	//const float dy = height / numStacks;

	NestGraphics::MeshData meshData;
	vector<NestGraphics::Vertex>& vertices = meshData.vertices;

	// 옆 면의 바닥 버텍스
	Vector3 leftBottomStartPoint = Vector3(bottomRadius, -0.5f * height, 0.0f);
	// 옆 면의 위 버텍스틀
	Vector3 TopBottomStartPoint = Vector3(topRadius, 0.5f * height, 0.0f);
	// 이동 위치를 위한 임시 값
	Vector3 dy = (TopBottomStartPoint - leftBottomStartPoint) / numStacks;

	for (int j = 0; j < numStacks + 1; j++)
	{
		// 바닥 버텍스에서 시작점을 위 버텍스 방향으로 이동
		Vector3 stackStartPoint = Vector3::Transform(
			leftBottomStartPoint, Matrix::CreateTranslation(dy * float(j)));
		for (int i = 0; i < numSlices + 1; i++)
		{
			NestGraphics::Vertex v;
			v.position = Vector3::Transform(Vector3(stackStartPoint), Matrix::CreateRotationY(dTheta * float(i)));
			//v.position.z = sin(i * dx * 10.0f) * curve;
			v.normal = v.position - Vector3(0.0f, -0.5f * height, 0.0f);
			v.normal.Normalize();
			v.texcoord = Vector2(float(i) / numSlices, 1.0f - float(j) / numStacks);
			//v.texcoord = Vector2(float(j), 1.0f - float(i));
			vertices.push_back(v);
			std::cout << vertices[i].position.y << " ";
		}
	}

	// 인덱스를 넣어보자.
	vector<uint16_t>& indices = meshData.indices;
	for (int idxY = 0; idxY < numStacks; idxY++)
	{
		const int offset = (numSlices + 1) * idxY;
		for (int idx = 0; idx < numSlices; idx++)
		{
			indices.push_back(offset + idx);
			indices.push_back(offset + idx + numSlices + 1);
			indices.push_back(offset + idx + numSlices + 2);

			indices.push_back(offset + idx);
			indices.push_back(offset + idx + numSlices + 2);
			indices.push_back(offset + idx + 1);
		}
	}
	return meshData;
}

NestGraphics::MeshData GameObject::MakeSphere(const float radius, const int numSlices, const int numStacks)
{
	const float dTheta = -DirectX::XM_2PI / float(numSlices);
	const float dPhi = -DirectX::XM_PI / float(numStacks);

	NestGraphics::MeshData meshData;
	vector<NestGraphics::Vertex>& vertices = meshData.vertices;

	for (int j = 0; j <= numStacks; j++)
	{
		// 스택에 쌓일 수록 시작점을 x-y 평면에서 회전 시켜서 위로 올리는 구조
		Vector3 stackStartPoint = Vector3::Transform(
			Vector3(0.0f, -radius, 0.0f), Matrix::CreateRotationZ(dPhi * j));
		for (int i = 0; i <= numSlices; i++)
		{
			NestGraphics::Vertex v;
			// 시작점을 x-z 평면에서 회전시키면서 원을 만드는 구조
			v.position = Vector3::Transform(
				stackStartPoint, Matrix::CreateRotationY(dTheta * float(i)));
			v.normal = v.position; // 원점이 구의 중심
			v.normal.Normalize();
			v.texcoord =
				Vector2(float(i) / numSlices, 1.0f - float(j) / numStacks);

			vertices.push_back(v);
		}
	}

	// 인덱스
	vector<uint16_t>& indices = meshData.indices;
	for (int j = 0; j < numStacks; j++)
	{
		const int offset = (numSlices + 1) * j;
		for (int i = 0; i < numSlices; i++)
		{
			indices.push_back(offset + i);
			indices.push_back(offset + i + numSlices + 1);
			indices.push_back(offset + i + 1 + numSlices + 1);

			indices.push_back(offset + i);
			indices.push_back(offset + i + 1 + numSlices + 1);
			indices.push_back(offset + i + 1);
		}
	}
	return meshData;
}

vector<NestGraphics::MeshData> GameObject::ReadFromFile(std::wstring filename)
{
	Converter* m_pConverter = new Converter();
	m_pConverter->ReadFile(filename);
	NestGraphics::ModelData* model = m_pConverter->GetModelData();
	vector<NestGraphics::MeshData> meshes = model->meshes;

	Vector3 vmin(1000, 1000, 1000);
	Vector3 vmax(-1000, -1000, -1000);
	for (auto& mesh : meshes)
	{
		for (auto& v : mesh.vertices)
		{
			vmin.x = DirectX::XMMin(vmin.x, v.position.x);
			vmin.y = DirectX::XMMin(vmin.y, v.position.y);
			vmin.z = DirectX::XMMin(vmin.z, v.position.z);
			vmax.x = DirectX::XMMax(vmax.x, v.position.x);
			vmax.y = DirectX::XMMax(vmax.y, v.position.y);
			vmax.z = DirectX::XMMax(vmax.z, v.position.z);
		}
	}

	float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
	float dl = DirectX::XMMax(DirectX::XMMax(dx, dy), dz);
	float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f,
		cz = (vmax.z + vmin.z) * 0.5f;

	for (auto& mesh : meshes)
	{
		for (auto& v : mesh.vertices)
		{
			v.position.x = (v.position.x - cx) / dl;
			v.position.y = (v.position.y - cy) / dl;
			v.position.z = (v.position.z - cz) / dl;
		}
	}
	return meshes;
}
