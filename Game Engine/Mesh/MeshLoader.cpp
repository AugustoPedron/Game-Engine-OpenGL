#include "MeshLoader.h"
//creazione della mesh per un modello esportato in formato obj
MeshLoader::MeshLoader(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<structTexture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->SetupMesh();
}
//creazione della mesh tramite vertici (scritti a mano) e texture
MeshLoader::MeshLoader(std::vector<Vertex> vertices, const GLchar* texture_diffuse, const GLchar* texture_specular) {
	this->diffuseMap = TextureLoader::LoadTexture(texture_diffuse);
	this->specularMap = TextureLoader::LoadTexture(texture_specular);
	this->vertices = vertices;
	//this->shadow = nullptr;
	this->SetupMeshNoIndices();
}

void MeshLoader::Draw(ShaderLoader shader) {
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < this->textures.size(); i++) {
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
		shader.Set1i((name + number).c_str(), i);
		//si collega all'indice specificato prima la texture da stampare
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	shader.Set1f("material.shininess", 16.0f);
	glBindVertexArray(this->VAO);
	//disegna i vertici del VAO per triangoli usando la rappresentazione indicizzata
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//disattiva le texture attivate in precedenza
	for (GLuint i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
//fa la stessa cosa di prima ma senza usare gl indici per i vertici.
void MeshLoader::DrawNoIndices(ShaderLoader shader) {
	/*if (this->shadow == nullptr) {
		this->shadow = std::make_shared<ShadowSetter::Shadow>(this->diffuseMap, this->VAO);
	}

	this->shadow->DrawShadows(model);*/

	shader.Set1i("material.diffuse", 0);
	shader.Set1i("material.specular", 1);
	//shader.Set1i("material.shadowMap", 2);
	shader.Set1f("material.shininess", 32.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->diffuseMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->specularMap);

	/*glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->shadow->getDepthMap());*/

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
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

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
/*
void MeshLoader::DrawShadows(ShaderLoader shader, glm::vec3 lightPos, glm::mat4 lightSpaceMatrix) {

	shader.use();

	shader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, ShadowSetter::SHADOW_WIDTH, ShadowSetter::SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, this->shadow->getDepthMapFBO());
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(50.0f));
	model = glm::translate(model, glm::vec3(0.0f, -0.53f, 0.0f));
	shader.SetMat4("model", model);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}*/

void MeshLoader::DeleteMesh() {
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}