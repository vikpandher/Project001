#include "Stores.h"

#include "Logger.h"

#include <fstream>



namespace Project001
{		
	// public ------------------------------------------------------------------

	Stores::Stores()
	{

	}

	Stores::~Stores()
	{
		for (std::map<std::string, Mesh*>::iterator iterator = meshMap_.begin(); iterator != meshMap_.end(); ++iterator)
		{
			delete iterator->second;
		}
		meshMap_.clear();
	}

	Mesh* Stores::GetMesh(std::string name)
	{
		return meshMap_[name];
	}

	bool Stores::LoadOBJFile(std::string name, std::string path)
	{
		// file path must have .obk extension
		if (path.substr(path.size() - 4, 4) != ".obj")
		{
			return false;
		}

		std::ifstream file(path);

		if (!file.is_open())
		{
			return false;
		}

		Mesh* newMeshPtr = new Mesh();

		std::string currentLine;
		while (std::getline(file, currentLine))
		{
			if (currentLine.empty())
			{
				continue;
			}

			std::string tag;
			std::string values;

			size_t tagStart = currentLine.find_first_not_of(" \t");
			size_t tagEnd = currentLine.find_first_of(" \t", tagStart);

			size_t valuesStart = currentLine.find_first_not_of(" \t", tagEnd);

			if (tagStart != std::string::npos && tagEnd != std::string::npos)
			{
				tag = currentLine.substr(tagStart, tagEnd - tagStart);
			}
			else if (tagStart != std::string::npos)
			{
				continue;
			}
			else
			{
				continue;
			}

			if (valuesStart != std::string::npos)
			{
				values = currentLine.substr(valuesStart);
			}
			else
			{
				continue;
			}

			std::vector<std::string> splitValues;

			size_t previousValueEnd = 0;
			while (previousValueEnd != std::string::npos)
			{
				size_t valueStart = values.find_first_not_of(" \t", previousValueEnd);
				size_t valueEnd = values.find_first_of(" \t", valueStart);

				if (valueStart != std::string::npos && valueEnd != std::string::npos)
				{
					splitValues.push_back(values.substr(valueStart, valueEnd - valueStart));
				}
				else if (tagStart != std::string::npos)
				{
					splitValues.push_back(values.substr(valueStart));
				}
				else
				{
					continue;
				}

				previousValueEnd = valueEnd;
			}

			if (tag == "v")
			{
				if (splitValues.size() >= 3)
				{
					glm::vec3 newPosition;
					newPosition.x = std::stof(splitValues[0]);
					newPosition.y = std::stof(splitValues[1]);
					newPosition.z = std::stof(splitValues[2]);

					newMeshPtr->positions.push_back(newPosition);
				}
			}
			else if (tag == "vt")
			{
				if (splitValues.size() >= 2)
				{
					glm::vec2 newTextureCoordinate;
					newTextureCoordinate.x = std::stof(splitValues[0]);
					newTextureCoordinate.y = std::stof(splitValues[1]);

					newMeshPtr->textureCoordinates.push_back(newTextureCoordinate);
				}
			}
			else if (tag == "vn")
			{
				if (splitValues.size() >= 3)
				{
					glm::vec3 newNormal;
					newNormal.x = std::stof(splitValues[0]);
					newNormal.y = std::stof(splitValues[1]);
					newNormal.z = std::stof(splitValues[2]);

					newMeshPtr->normals.push_back(newNormal);
				}
			}
			else if (tag == "f")
			{				
				if (splitValues.size() >= 3)
				{
					std::vector<IndexGroup> newFace;

					for (int i = 0; i < splitValues.size(); i++)
					{
						std::string indexGroup = splitValues[i];

						std::vector<std::string> indexGroupValues;

						size_t previousindexGroupValueEnd = 0;
						while (previousindexGroupValueEnd != std::string::npos)
						{
							size_t indexGroupValueStart = indexGroup.find_first_not_of("/", previousindexGroupValueEnd);
							size_t indexGroupValueEnd = indexGroup.find_first_of("/", indexGroupValueStart);

							if (indexGroupValueStart != std::string::npos && indexGroupValueEnd != std::string::npos)
							{
								indexGroupValues.push_back(indexGroup.substr(indexGroupValueStart, indexGroupValueEnd - indexGroupValueStart));
							}
							else if (tagStart != std::string::npos)
							{
								indexGroupValues.push_back(indexGroup.substr(indexGroupValueStart));
							}
							else
							{
								continue;
							}

							previousindexGroupValueEnd = indexGroupValueEnd;
						}

						IndexGroup newindexGroup;
						newindexGroup.positonIndex = std::stoi(indexGroupValues[0]);
						newindexGroup.textureCoordinateIndex = std::stoi(indexGroupValues[1]);
						newindexGroup.normalIndex = std::stoi(indexGroupValues[2]);

						newFace.push_back(newindexGroup);
					}

					newMeshPtr->faces.push_back(newFace);
				}
			}
		}

		meshMap_.insert(std::make_pair(name, newMeshPtr));

		return true;
	}

	// protected: --------------------------------------------------------------

	std::string Stores::GetFirstToken(const std::string& input) const
	{
		if (input.empty())
		{
			return "";
		}
		
		// find the index of the first character that isn't a space or tab
		size_t tokenStart = input.find_first_not_of(" \t");
		// find the index of the first space, tab, or new line after
		size_t tokenEnd = input.find_first_of(" \t\n", tokenStart);

		// if start and and to a token has been found:
		if (tokenStart != std::string::npos && tokenEnd != std::string::npos)
		{
			return input.substr(tokenStart, tokenEnd - tokenStart);
		}
		// if only the start of a token has been found:
		else if (tokenStart != std::string::npos)
		{
			return input.substr(tokenStart);
		}
		// if no token was found
		else
		{
			return "";
		}
	}
}