#pragma once

#include "Graphics.h"
#include "Maths.h"
#include <string>
#include <vector>

class ShaderProgram;

#define GLU_SET_DEF(t, m) \
inline void Set(const t* data, int count) \
{ \
	m(GetHandle(), count, data); \
} \
inline void Set(const std::vector<t>& data) \
{ \
	m(GetHandle(), data.size(), data.data()); \
} \
inline void Set(const t& data) \
{ \
	m(GetHandle(), 1, &data); \
}

#define GLU_SET_DEF_ST(t, st, m) \
inline void Set(const t* data, int count) \
{ \
	m(GetHandle(), count, (st*)data); \
} \
inline void Set(const std::vector<t>& data) \
{ \
	m(GetHandle(), data.size(), (st*)data.data()); \
} \
inline void Set(const t& data) \
{ \
	m(GetHandle(), 1, (st*)&data); \
}

#define GLUSYNC_DEF(t) \
typedef GLuniformSync<t> GLu ## t; \
typedef GLuniformSyncL<t> GLu ## t ## L;

#define GLUSYNC_DEF2(t, n) \
typedef GLuniformSync<t> GLu ## n; \
typedef GLuniformSyncL<t> GLu ## n ## L;

class GLuniform
{
	std::string name;
	GLint GetHandle();

public:
	GLuniform(const std::string& name);
	GLuniform(const GLuniform&) = delete;
	GLuniform& operator= (const GLuniform&) = delete;

	GLU_SET_DEF_ST(Vec3, float, glUniform3fv)
		GLU_SET_DEF(float, glUniform1fv)
		GLU_SET_DEF(int, glUniform1iv)

		void Set(const Mat4& data);
	void Set(const Mat4* data, int count);

	static void BindInitalizationContext(const ShaderProgram& shader);
	static void UnbindInitalizationContext();
};

struct IGLuniformSync
{
	inline IGLuniformSync(std::vector<IGLuniformSync*>* syncGroup)
	{
		if (!syncGroup) return;
		syncGroup->push_back(this);
	}

	virtual void Send() = 0;
};

#define C_SYNC_DEF_ARGS std::vector<IGLuniformSync*>* syncGroup = nullptr
#define C_SYNC_PASS_ARGS syncGroup
#define C_SYNC_INIT IGLuniformSync(syncGroup)

template<typename T>
class GLuniformSync : public GLuniform, public IGLuniformSync
{
	T actual;

public:
	T value;

	GLuniformSync(const std::string& name, const T& value, C_SYNC_DEF_ARGS) : GLuniform(name), C_SYNC_INIT
	{
		this->value = value;
	}

		T& operator =(const T& other)
	{
		return value = other;
	}

	void Send()
	{
		actual = value;
		GLuniform::Set(value);
	}

	const T& Actual()
	{
		return actual;
	}
};

template<typename T>
class GLuniformSyncL : public GLuniformSync<std::vector<T>>
{
public:
	GLuniformSyncL(const std::string& name, C_SYNC_DEF_ARGS) : GLuniformSync<std::vector<T>>(name, {}, C_SYNC_PASS_ARGS) { }
	GLuniformSyncL(const std::string& name, const T& value, int preallocSize, C_SYNC_DEF_ARGS) : GLuniformSync<std::vector<T>>(name, {}, C_SYNC_PASS_ARGS)
	{
		for (int i = 0; i < preallocSize; i++)
		{
			GLuniformSync<std::vector<T>>::value.push_back(value);
		}
	}

	T& operator[](int i)
	{
		return GLuniformSync<std::vector<T>>::value[i];
	}

	T& operator[](const GLuniformSync<int>& i)
	{
		return GLuniformSync<std::vector<T>>::value[i.value];
	}
};

#define COMMA ,

GLUSYNC_DEF2(int, Int)
GLUSYNC_DEF2(float, Float)
GLUSYNC_DEF(Vec3)
GLUSYNC_DEF(Mat4)

#define GL_UNIFORM_SYNC_GROUP_DEF std::vector<IGLuniformSync*> internalGLUniformSync;

#define SYNC_GROUP(t, vName, pName) t vName = t (pName, internalGLUniformSync);
#define SYNC_GROUP_ARGS(t, vName, pName, args) t vName = t (pName, args, &internalGLUniformSync);

#define GL_UNIFORM_SYNC_ALL \
for (IGLuniformSync* sync : internalGLUniformSync) \
{ \
	sync->Send(); \
}