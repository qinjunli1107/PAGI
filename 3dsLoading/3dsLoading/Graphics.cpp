#include "Graphics.h"

Graphics::Graphics()
{
	this->m_window = 0;
	this->m_camera = 0;
	this->m_input = 0;
	this->m_vertexArrayID = 0;
	this->m_currentSelected = -1;
	this->m_selectionKeyPressed = false;
	this->m_mousePressed = false;
	this->m_selectedMode = false;
	this->timer = 0.0f;
}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(string appName)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);	//4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//Setting OpenGL 3.3 version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //older OpenGL version not included

	this->m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, appName.c_str(), NULL, NULL);

	if (this->m_window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(this->m_window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	this->m_input = new Input();
	if (!this->m_input->Initialize(this->m_window))
	{
		fprintf(stderr, "Failed to initialize input. But why?\n");
		return false;
	}

	glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &this->m_vertexArrayID);
	glBindVertexArray(this->m_vertexArrayID);

	this->m_camera = new Camera(glm::vec3(0.0f, 0.0f, 200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 3000.0f);

	Model* model = new Model();
	std::vector<Model*> children;
	std::vector<Model*> firstChildChildren;
	Model* firstChild = new Model();
	GLuint shaderID = this->InitializeShaders("VertexShader.glsl", "FragmentShader.glsl");

	if (!firstChild->Initialize(shaderID, this->m_camera, 0))
	{
		return false;
	}

	firstChild->Translate(glm::vec3(-30.0f, 0.0f, 0.0f)); 

	if (!model->Initialize(shaderID, this->m_camera, 1))
	{
		fprintf(stderr, "Failed to load model (say what?!)\n");
		return false;
	}

	Model* secondChild = new Model();
	if (!secondChild->Initialize(shaderID, this->m_camera, 2))
	{
		return false;
	}

	secondChild->Translate(glm::vec3(-30.0f, 30.0f, 0.0f));

	Model* thirdChild = new Model();
	if (!thirdChild->Initialize(shaderID, this->m_camera, 3))
	{
		return false;
	}

	thirdChild->Translate(glm::vec3(-30.0f, 0.0f, -30.0f));

	firstChild->AddChild(thirdChild);
	model->AddChild(firstChild);
	model->AddChild(secondChild);

	this->m_models.push_back(model);
	this->m_models.push_back(firstChild);
	this->m_models.push_back(secondChild);
	this->m_models.push_back(thirdChild);

	this->m_light = new Light();
	if (!this->m_light->Initialize(glm::vec3(0.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)))
	{
		return false;
	}

	return true;
}

void Graphics::Frame(float deltaTime)
{
	this->m_camera->Frame(this->m_input, deltaTime);

#pragma region Selecting model part

	if (this->m_selectedMode)
	{
		timer += deltaTime;
		if (timer > 0.003f)
		{
			timer = 0.0f;
			this->m_selectedMode = false;
			this->SelectPixel();
		}
		if (!this->m_mousePressed)
		{
			this->m_selectedMode = false;
			this->SelectPixel();
		}
	}

	if (this->m_input->IsMouseButtonDown(MouseButtons::Left) && !this->m_mousePressed)
	{
		this->m_mousePressed = true;
		this->m_selectedMode = true;
	}

	if (this->m_mousePressed)
	{
		if (this->m_input->IsMouseButtonUp(MouseButtons::Left))
		{
			this->m_mousePressed = false;

			glFlush();
			glFinish();
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
	}
	
	if (this->m_currentSelected >= 0)
	{
		if (this->m_input->IsKeyDown(KeyCodes::I))
		{
			this->m_models[this->m_currentSelected]->Translate(glm::vec3(0.0f, 0.0f, -15.0f) * deltaTime);
		}
		if (this->m_input->IsKeyDown(KeyCodes::K))
		{
			this->m_models[this->m_currentSelected]->Translate(glm::vec3(0.0f, 0.0f, 15.0f) * deltaTime);
		}
		if (this->m_input->IsKeyDown(KeyCodes::J))
		{
			this->m_models[this->m_currentSelected]->Translate(glm::vec3(-15.0f, 0.0f, 0.0f) * deltaTime);
		}
		if (this->m_input->IsKeyDown(KeyCodes::L))
		{
			this->m_models[this->m_currentSelected]->Translate(glm::vec3(15.0f, 0.0f, 0.0f) * deltaTime);
		}
		if (this->m_input->IsKeyDown(KeyCodes::U))
		{
			this->m_models[this->m_currentSelected]->Rotate(-10.0f * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (this->m_input->IsKeyDown(KeyCodes::O))
		{
			this->m_models[this->m_currentSelected]->Rotate(10.0f * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

#pragma endregion

	for each (Model* m in this->m_models)
	{
		if (m->GetParent() == nullptr)
		{
			m->Frame(this->m_camera);
		}
	}
	Render();
}

void Graphics::Shutdown()
{
	if (this->m_camera != nullptr)
	{
		delete this->m_camera;
		this->m_camera = 0;
	}

	for (int i = 0; i < this->m_models.size(); ++i)
	{
		if (this->m_models[i] != NULL)
		{
			this->m_models[i]->Shutdown();
			delete this->m_models[i];
			this->m_models[i] = 0;
		}
	}

	if (this->m_window != nullptr)
	{
		glfwDestroyWindow(this->m_window);
		this->m_window = 0;
	}

	glfwTerminate();
	glDeleteVertexArrays(1, &this->m_vertexArrayID);
}

GLFWwindow* Graphics::GetWindow()
{
	return this->m_window;
}

void Graphics::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!this->m_selectedMode)
	{
		for each (Model* m in this->m_models)
		{
			if (m->GetParent() == nullptr)
			{
				m->Render(this->m_camera, this->m_light);
			}
		}
	}
	else
	{
		for (int i = 0; i < this->m_models.size(); ++i)
		{
			if (this->m_models[i]->GetParent() == nullptr)
			{
				this->m_models[i]->RenderSelectionMode(this->m_camera);
			}
		}
	}
	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

GLuint Graphics::InitializeShaders(const char const* vsPath, const char const* fsPath)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	string vsCode = this->LoadShaderCode(vsPath);
	string fsCode = this->LoadShaderCode(fsPath);

	//Compiling shaders
	printf("Compiling vertex shader (%s)\n", vsPath);
	this->CompileShader(vsCode, vertexShaderID);
	printf("Compiling fragment shader (%s)\n", fsPath);
	this->CompileShader(fsCode, fragmentShaderID);

	//Linking program
	printf("Linking program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	//Checking program
	GLint result;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		vector<char> ProgramErrorMessage(glm::max(infoLogLength, int(1)));
		glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stderr, "%s\n", &ProgramErrorMessage[0]);
	}
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

string Graphics::LoadShaderCode(const char const* shaderPath)
{
	string shaderCode = "";
	ifstream shaderStream(shaderPath, ios::in);
	if (shaderStream.is_open())
	{
		string line = "";
		while (getline(shaderStream, line))
		{
			shaderCode += "\n" + line;
		}
		shaderStream.close();
	}

	return shaderCode;
}

void Graphics::CompileShader(string shaderCode, GLuint shaderCodeID)
{
	char const * shaderPointer = shaderCode.c_str();
	glShaderSource(shaderCodeID, 1, &shaderPointer, NULL);
	glCompileShader(shaderCodeID);

	//Checking shader
	GLint result = 0;
	glGetShaderiv(shaderCodeID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(shaderCodeID, GL_INFO_LOG_LENGTH, &infoLogLength);
		vector<char> shaderErrorMsg(infoLogLength);
		glGetShaderInfoLog(shaderCodeID, infoLogLength, NULL, &shaderErrorMsg[0]);
		fprintf(stderr, "%s\n", &shaderErrorMsg[0]);
	}
}

Input* Graphics::GetInput()
{
	return this->m_input;
}

void Graphics::SelectPixel()
{
	GLchar color[4];
	glm::vec2 mousePosition = this->m_input->GetMousePosition();

	glReadPixels(mousePosition.x, SCREEN_HEIGHT - mousePosition.y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);

	GLchar* pickColor = NULL;
	bool hit = false;
	for (int i = 0; i < this->m_models.size(); ++i)
	{
		pickColor = this->m_models[i]->GetPickColor();
		if (pickColor[0] == color[0] && pickColor[1] == color[1] && pickColor[2] == color[2])
		{
			hit = true;
			this->m_currentSelected = i;
			this->m_models[i]->Select();
		}
	}

	if (!hit)
	{
		this->m_currentSelected = -1;
	}

	for (int i = 0; i < this->m_models.size(); ++i)
	{
		if (i != this->m_currentSelected)
		{
			this->m_models[i]->Unselect();
		}
	}
}