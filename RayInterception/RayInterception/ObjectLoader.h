#pragma once

#include <memory>
#include <string>

class ObjectData;
typedef std::shared_ptr<ObjectData> ObjectDataPtr;

class ObjectLoader
{
public:
	enum LineElements
	{
		v = 0,
		vn,
		vt,
		f,
		element_count
	};

	static ObjectDataPtr Load(const std::string& path);
};

