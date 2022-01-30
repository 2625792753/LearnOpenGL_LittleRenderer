#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//error report func of Glfw init
static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

class lazy {
public:
	static void glfwCoreEnv(int min_version, int max_version);
	static unsigned int ShaderBulider(unsigned vertexShader, const GLchar* vertexshadersource, unsigned int fragShader, const GLchar* fragshadersource);
	static void standardimput(GLFWwindow* window);
	static void settextureformula();
	static unsigned int TextureFromFile(const char *name, const std::string directory);
	//lazy guy :)
};

void lazy::glfwCoreEnv(int min_version, int max_version) {
	glfwSetErrorCallback(glfw_error_callback);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, max_version);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

unsigned int lazy::ShaderBulider(unsigned vertexShader, const GLchar* vertexshadersource, unsigned int fragShader, const GLchar* fragshadersource) {
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexshadersource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragshadersource, NULL);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int program;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	return program;
}

void lazy::settextureformula() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

unsigned int lazy::TextureFromFile(const char *name, const std::string directory) {
    std::string fileName = std::string(name);
    fileName = directory + '/' + fileName;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width;
    int height;
    int colorChannels;
    unsigned char *textureData = stbi_load(fileName.c_str(), &width, &height, &colorChannels, 0);

    if(textureData) {
        GLenum format;
        if(colorChannels == 1)
            format = GL_RED;
        else if(colorChannels == 3)
            format = GL_RGB;
        else if(colorChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);

		settextureformula();

		stbi_image_free(textureData);
    }
    else {
        std::cout << "Texture Failed to Load at Path:" << fileName << std::endl;
        stbi_image_free(textureData);
    }

    return textureID;
}