#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class object
{
	std::vector<int> vertices;

	void setVertices(std::vector<int> data)
	{
		for (int i = 0; i < vertices.size(); i++)
		{
			if(i < data.size())
			{
				vertices[i] = data[i];
			}
		}

		for (int i = vertices.size(); i < data.size(); i++)
		{
			vertices.push_back(data[i]);
		}

		return;
	}
};