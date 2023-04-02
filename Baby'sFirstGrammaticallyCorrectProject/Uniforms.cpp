#include "Uniforms.h"

GLuniform<int>
Uniforms::DLightCount = { "dLightCount" };

GLuniform<float>
Uniforms::Time = { "_Time" };

GLuniform<Vec3>
Uniforms::CamPos = { "_CamPos" },
Uniforms::AmbientLight = { "ambientLight" };

GLuniform<Mat4>
Uniforms::ModelMat = { "_Model" },
Uniforms::ViewMat = { "_View" },
Uniforms::ProjMat = { "_Projection" },
Uniforms::ViewProjMat = { "_VP" },
Uniforms::ModelViewProjMat = { "_MVP" };

GLuniformList<Vec3>
Uniforms::DLightDirections = { "dLightDirections", MaxDLights },
Uniforms::DLightColors = { "dLightColors", MaxDLights };