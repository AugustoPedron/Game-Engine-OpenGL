#include "MeshLoader.h"
//creazione della mesh per un modello esportato in formato obj
MeshLoader::MeshLoader(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<structTexture>& textures) {
	this->vertices =  std::shared_ptr<std::vector<Vertex>>(new std::vector<Vertex>(vertices));
	this->indices = indices;
	this->textures = textures;
	this->SetupMesh();
}
//creazione della mesh tramite vertici (scritti a mano) e texture
MeshLoader::MeshLoader(MeshDefinition& Mesh) {
	this->position = Mesh.position;
	this->diffuseMap = TextureLoader::LoadTexture(Mesh.mesh.texture_diffuse.c_str());
	this->specularMap = TextureLoader::LoadTexture(Mesh.mesh.texture_specular.c_str());
	this->vertices = std::shared_ptr<std::vector<Vertex>>(new std::vector<Vertex>(Mesh.mesh.Vertices));
	this->SetupMeshNoIndices();
}

void MeshLoader::Draw(ShaderLoader* shader, unsigned int shadowMap) {
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint i;
	for (i = 0; i < this->textures.size(); i++) {
		//viene attivata una texture e si specifica quale indice ha
		glActiveTexture(GL_TEXTURE0 + i);

		std::stringstream ss;
		std::string number;
		std::string name = this->textures[i].type;

		if ("texture_diffuse" == name) {
			ss << diffuseNr++;
		}
		else if ("texture_specular" == name) {
			ss << specularNr++;
		}

		number = ss.str();
		shader->Set1i((name + number).c_str(), i);
		//si collega all'indice specificato prima la texture da stampare
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	shader->Set1f("material.shininess", 16.0f);

	shader->Set1i("shadowMap", i);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glBindVertexArray(this->VAO);
	//disegna i vertici del VAO per triangoli usando la rappresentazione indicizzata
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//disattiva le texture attivate in precedenza
	for (GLuint i = 0; i < this->textures.size()+1; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
//fa la stessa cosa di prima ma senza usare gl indici per i vertici.
void MeshLoader::DrawNoIndices(ShaderLoader* shader, unsigned int shadowMap) {
	shader->SetMat4("model", this->position);
	shader->Set1i("material.diffuse", 0);
	shader->Set1i("material.specular", 1);
	shader->Set1i("material.shadowMap", 2);
	shader->Set1f("material.shininess", 32.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->diffuseMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->specularMap);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void MeshLoader::SetupMesh() {
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	glBindVertexArray(this->VAO);
	//si riempie il VBO con le posizioni dei vertici
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices->size() * sizeof(Vertex), &this->vertices->at(0), GL_STATIC_DRAW);
	//si riempie il EBO con gli indici dei vertici che verranno usati.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	//vertex positions
	//manda al parametro x del vertex shader gli attributi
	//indica quanti valori prendere dall'array dei vertici per ogni riga:
	//i primi 3 numeri indicano la posizione dei vertici
	//i secondi 3 numeri indicano la direzione della normale alla superficie
	//gli ultimi due indcano le coordinate per le texture
	//l'ultimo campo serve per indicare l'offset all'interno della riga per prendere i valori
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

	//vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));

	//vertex texture coordniates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
//stessa cosa di prima ma senza indici
void MeshLoader::SetupMeshNoIndices() {
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices->size() * sizeof(Vertex), &this->vertices->at(0), GL_STATIC_DRAW);

	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

	//vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));

	//vertex texture coordniates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

MeshLoader::~MeshLoader() {
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
	this->vertices.reset();
	std::vector<GLuint>().swap(this->indices);
	std::vector<structTexture>().swap(this->textures);
}