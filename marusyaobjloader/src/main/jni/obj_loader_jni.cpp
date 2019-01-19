#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <jni.h>
#include <android/log.h>
#include <chrono>

#define LOG_TAG "MarusyaObjLoader"

const int VERTICES_NUM_COMPONENTS = 3;
const int NORMALS_NUM_COMPONENTS = 3;
const int COLORS_NUM_COMPONENTS = 3;
const int TEX_COORDS_NUM_COMPONENTS = 2;

bool print_log = true;

typedef struct {
    jclass j_result_model_class;
    jmethodID j_result_model_constructor;
    jfieldID j_shape_models_field_id;
    jfieldID j_material_models_field_id;
    jfieldID j_vertices_size_field_id;
    jfieldID j_normals_size_field_id;
    jfieldID j_texcoords_size_field_id;
    jfieldID j_colors_size_field_id;
    jfieldID j_warn_field_id;
    jfieldID j_error_field_id;
} RESULT_MODEL_JNI;

typedef struct {
    jclass j_shape_model_class;
    jmethodID j_shape_model_constructor;
    jfieldID j_name_field_id;
    jfieldID j_mesh_model_field_id;
} SHAPE_MODEL_JNI;

typedef struct {
    jclass j_mesh_model_class;
    jmethodID j_mesh_model_constructor;
    jfieldID j_vertices_field_id;
    jfieldID j_indices_field_id;
} MESH_MODEL_JNI;

typedef struct {
    jclass j_material_model_class;
    jmethodID j_material_model_constructor;
    jfieldID j_name_field_id;
    jfieldID j_ambient_field_id;
    jfieldID j_diffuse_field_id;
    jfieldID j_specular_field_id;
    jfieldID j_transmittance_field_id;
    jfieldID j_emission_field_id;
    jfieldID j_shininess_field_id;
    jfieldID j_ior_field_id;
    jfieldID j_dissolve_field_id;
    jfieldID j_illum_field_id;
    jfieldID j_dummy_field_id;
    jfieldID j_ambient_texname_field_id;
    jfieldID j_diffuse_texname_field_id;
    jfieldID j_specular_texname_field_id;
    jfieldID j_specular_highlight_texname_field_id;
    jfieldID j_bump_texname_field_id;
    jfieldID j_displacement_texname_field_id;
    jfieldID j_alpha_texname_field_id;
    jfieldID j_reflection_texname_field_id;
    jfieldID j_ambient_texopt_field_id;
    jfieldID j_diffuse_texopt_field_id;
    jfieldID j_specular_texopt_field_id;
    jfieldID j_specular_highlight_texopt_field_id;
    jfieldID j_bump_texopt_field_id;
    jfieldID j_displacement_texopt_field_id;
    jfieldID j_alpha_texopt_field_id;
    jfieldID j_reflection_texopt_field_id;
    jfieldID j_roughness_field_id;
    jfieldID j_metallic_field_id;
    jfieldID j_sheen_field_id;
    jfieldID j_clearcoat_thickness_field_id;
    jfieldID j_clearcoat_roughness_field_id;
    jfieldID j_anisotropy_field_id;
    jfieldID j_anisotropy_rotation_field_id;
    jfieldID j_pad0_field_id;
    jfieldID j_roughness_texname_field_id;
    jfieldID j_metallic_texname_field_id;
    jfieldID j_sheen_texname_field_id;
    jfieldID j_emissive_texname_field_id;
    jfieldID j_normal_texname_field_id;
    jfieldID j_roughness_texopt_field_id;
    jfieldID j_metallic_texopt_field_id;
    jfieldID j_sheen_texopt_field_id;
    jfieldID j_emissive_texopt_field_id;
    jfieldID j_normal_texopt_field_id;
    jfieldID j_pad2_field_id;
    jfieldID j_unknown_parameter_field_id;
} MATERIAL_MODEL_JNI;

typedef struct {
    jclass j_texture_option_model_class;
    jmethodID j_texture_option_model_constructor;
    jfieldID j_type_field_id;
    jfieldID j_sharpness_field_id;
    jfieldID j_brightness_field_id;
    jfieldID j_contrast_field_id;
    jfieldID j_origin_offset_field_id;
    jfieldID j_scale_field_id;
    jfieldID j_turbulence_field_id;
    jfieldID j_clamp_field_id;
    jfieldID j_imfchan_field_id;
    jfieldID j_blend_u_field_id;
    jfieldID j_blend_v_field_id;
    jfieldID j_bump_multiplier_field_id;
    jfieldID j_colorspace_field_id;
} TEXTURE_OPTION_MODEL_JNI;

typedef struct {
    jclass j_texture_type_class;
    jfieldID j_texture_type_none_field_id;
    jfieldID j_texture_type_sphere_field_id;
    jfieldID j_texture_type_cube_top_field_id;
    jfieldID j_texture_type_cube_bottom_field_id;
    jfieldID j_texture_type_cube_front_field_id;
    jfieldID j_texture_type_cube_back_field_id;
    jfieldID j_texture_type_cube_left_field_id;
    jfieldID j_texture_type_cube_right_field_id;
} TEXTURE_TYPE_JNI;

typedef struct {
    jclass j_hash_map_class;
    jmethodID j_hash_map_constructor;
    jmethodID j_hash_map_put;
} HASH_MAP_JNI;

RESULT_MODEL_JNI * result_model_jni = nullptr;
SHAPE_MODEL_JNI * shape_model_jni = nullptr;
MESH_MODEL_JNI * mesh_model_jni = nullptr;
MATERIAL_MODEL_JNI * material_model_jni = nullptr;
TEXTURE_OPTION_MODEL_JNI * texture_option_model_jni = nullptr;
TEXTURE_TYPE_JNI * texture_type_jni = nullptr;
HASH_MAP_JNI * hash_map_jni = nullptr;

void LoadResultModelJNIDetails(JNIEnv * env) {
    result_model_jni = new RESULT_MODEL_JNI;
    result_model_jni->j_result_model_class = env->FindClass("com/riseapps/marusyaobjloader/model/ResultModel");
    result_model_jni->j_result_model_constructor = env->GetMethodID(result_model_jni->j_result_model_class, "<init>", "()V");
    result_model_jni->j_shape_models_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "shapeModels", "[Lcom/riseapps/marusyaobjloader/model/mesh/ShapeModel;");
    result_model_jni->j_material_models_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "materialModels", "[Lcom/riseapps/marusyaobjloader/model/material/MaterialModel;");
    result_model_jni->j_vertices_size_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "verticesSize", "J");
    result_model_jni->j_normals_size_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "normalsSize", "J");
    result_model_jni->j_texcoords_size_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "texcoordsSize", "J");
    result_model_jni->j_colors_size_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "colorsSize", "J");
    result_model_jni->j_warn_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "warn", "Ljava/lang/String;");
    result_model_jni->j_error_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "error", "Ljava/lang/String;");
}

void LoadShapeModelJNIDetails(JNIEnv * env) {
    shape_model_jni = new SHAPE_MODEL_JNI;
    shape_model_jni->j_shape_model_class = env->FindClass("com/riseapps/marusyaobjloader/model/mesh/ShapeModel");
    shape_model_jni->j_shape_model_constructor = env->GetMethodID(shape_model_jni->j_shape_model_class, "<init>", "()V");
    shape_model_jni->j_name_field_id = env->GetFieldID(shape_model_jni->j_shape_model_class, "name", "Ljava/lang/String;");
    shape_model_jni->j_mesh_model_field_id = env->GetFieldID(shape_model_jni->j_shape_model_class, "meshModel", "Lcom/riseapps/marusyaobjloader/model/mesh/MeshModel;");
}

void LoadMeshModelJNIDetails(JNIEnv * env) {
    mesh_model_jni = new MESH_MODEL_JNI;
    mesh_model_jni->j_mesh_model_class = env->FindClass("com/riseapps/marusyaobjloader/model/mesh/MeshModel");
    mesh_model_jni->j_mesh_model_constructor = env->GetMethodID(mesh_model_jni->j_mesh_model_class, "<init>", "()V");
    mesh_model_jni->j_vertices_field_id = env->GetFieldID(mesh_model_jni->j_mesh_model_class, "vertices", "[F");
    mesh_model_jni->j_indices_field_id = env->GetFieldID(mesh_model_jni->j_mesh_model_class, "indices", "[I");
}

void LoadMaterialModelJNIDetails(JNIEnv * env) {
    material_model_jni = new MATERIAL_MODEL_JNI;
    material_model_jni->j_material_model_class = env->FindClass("com/riseapps/marusyaobjloader/model/material/MaterialModel");
    material_model_jni->j_material_model_constructor = env->GetMethodID(material_model_jni->j_material_model_class, "<init>", "()V");
    material_model_jni->j_name_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "name", "Ljava/lang/String;");
    material_model_jni->j_ambient_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "ambient", "[F");
    material_model_jni->j_diffuse_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "diffuse", "[F");
    material_model_jni->j_specular_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "specular", "[F");
    material_model_jni->j_transmittance_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "transmittance", "[F");
    material_model_jni->j_emission_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "emission", "[F");
    material_model_jni->j_shininess_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "shininess", "F");
    material_model_jni->j_ior_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "ior", "F");
    material_model_jni->j_dissolve_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "dissolve", "F");
    material_model_jni->j_illum_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "illum", "I");
    material_model_jni->j_dummy_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "dummy", "I");
    material_model_jni->j_ambient_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "ambientTexname", "Ljava/lang/String;");
    material_model_jni->j_diffuse_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "diffuseTexname", "Ljava/lang/String;");
    material_model_jni->j_specular_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "specularTexname", "Ljava/lang/String;");
    material_model_jni->j_specular_highlight_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "specularHighlightTexname", "Ljava/lang/String;");
    material_model_jni->j_bump_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "bumpTexname", "Ljava/lang/String;");
    material_model_jni->j_displacement_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "displacementTexname", "Ljava/lang/String;");
    material_model_jni->j_alpha_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "alphaTexname", "Ljava/lang/String;");
    material_model_jni->j_reflection_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "reflectionTexname", "Ljava/lang/String;");
    material_model_jni->j_ambient_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "ambientTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_diffuse_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "diffuseTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_specular_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "specularTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_specular_highlight_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "specularHighlightTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_bump_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "bumpTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_displacement_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "displacementTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_alpha_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "alphaTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_reflection_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "reflectionTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_roughness_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "roughness", "F");
    material_model_jni->j_metallic_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "metallic", "F");
    material_model_jni->j_sheen_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "sheen", "F");
    material_model_jni->j_clearcoat_thickness_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "clearcoatThickness", "F");
    material_model_jni->j_clearcoat_roughness_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "clearcoatRoughness", "F");
    material_model_jni->j_anisotropy_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "anisotropy", "F");
    material_model_jni->j_anisotropy_rotation_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "anisotropyRotation", "F");
    material_model_jni->j_pad0_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "pad0", "F");
    material_model_jni->j_roughness_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "roughnessTexname", "Ljava/lang/String;");
    material_model_jni->j_metallic_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "metallicTexname", "Ljava/lang/String;");
    material_model_jni->j_sheen_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "sheenTexname", "Ljava/lang/String;");
    material_model_jni->j_emissive_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "emissiveTexname", "Ljava/lang/String;");
    material_model_jni->j_normal_texname_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "normalTexname", "Ljava/lang/String;");
    material_model_jni->j_roughness_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "roughnessTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_metallic_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "metallicTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_sheen_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "sheenTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_emissive_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "emissiveTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_normal_texopt_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "normalTexopt", "Lcom/riseapps/marusyaobjloader/model/material/TextureOptionModel;");
    material_model_jni->j_pad2_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "pad2", "I");
    material_model_jni->j_unknown_parameter_field_id = env->GetFieldID(material_model_jni->j_material_model_class, "unknownParameter", "Ljava/util/HashMap;");
}

void LoadTextureOptionModelJNIDetails(JNIEnv * env) {
    texture_option_model_jni = new TEXTURE_OPTION_MODEL_JNI;
    texture_option_model_jni->j_texture_option_model_class = env->FindClass("com/riseapps/marusyaobjloader/model/material/TextureOptionModel");
    texture_option_model_jni->j_texture_option_model_constructor = env->GetMethodID(texture_option_model_jni->j_texture_option_model_class, "<init>", "()V");
    texture_option_model_jni->j_type_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "type", "Lcom/riseapps/marusyaobjloader/model/material/TextureType;");
    texture_option_model_jni->j_sharpness_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "sharpness", "F");
    texture_option_model_jni->j_brightness_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "brightness", "F");
    texture_option_model_jni->j_contrast_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "contrast", "F");
    texture_option_model_jni->j_origin_offset_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "originOffset", "[F");
    texture_option_model_jni->j_scale_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "scale", "[F");
    texture_option_model_jni->j_turbulence_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "turbulence", "[F");
    texture_option_model_jni->j_clamp_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "clamp", "Z");
    texture_option_model_jni->j_imfchan_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "imfchan", "C");
    texture_option_model_jni->j_blend_u_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "blendU", "Z");
    texture_option_model_jni->j_blend_v_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "blendV", "Z");
    texture_option_model_jni->j_bump_multiplier_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "bumpMultiplier", "F");
    texture_option_model_jni->j_colorspace_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "colorspace", "Ljava/lang/String;");
}

void LoadTextureTypeJNIDetails(JNIEnv * env) {
    texture_type_jni = new TEXTURE_TYPE_JNI;
    texture_type_jni->j_texture_type_class = env->FindClass("com/riseapps/marusyaobjloader/model/material/TextureType");
    texture_type_jni->j_texture_type_none_field_id = env->GetStaticFieldID(texture_type_jni->j_texture_type_class, "TEXTURE_TYPE_NONE", "Lcom/riseapps/marusyaobjloader/model/material/TextureType;");
    texture_type_jni->j_texture_type_sphere_field_id = env->GetStaticFieldID(texture_type_jni->j_texture_type_class, "TEXTURE_TYPE_SPHERE", "Lcom/riseapps/marusyaobjloader/model/material/TextureType;");
    texture_type_jni->j_texture_type_cube_top_field_id = env->GetStaticFieldID(texture_type_jni->j_texture_type_class, "TEXTURE_TYPE_CUBE_TOP", "Lcom/riseapps/marusyaobjloader/model/material/TextureType;");
    texture_type_jni->j_texture_type_cube_bottom_field_id = env->GetStaticFieldID(texture_type_jni->j_texture_type_class, "TEXTURE_TYPE_CUBE_BOTTOM", "Lcom/riseapps/marusyaobjloader/model/material/TextureType;");
    texture_type_jni->j_texture_type_cube_front_field_id = env->GetStaticFieldID(texture_type_jni->j_texture_type_class, "TEXTURE_TYPE_CUBE_FRONT", "Lcom/riseapps/marusyaobjloader/model/material/TextureType;");
    texture_type_jni->j_texture_type_cube_back_field_id = env->GetStaticFieldID(texture_type_jni->j_texture_type_class, "TEXTURE_TYPE_CUBE_BACK", "Lcom/riseapps/marusyaobjloader/model/material/TextureType;");
    texture_type_jni->j_texture_type_cube_left_field_id = env->GetStaticFieldID(texture_type_jni->j_texture_type_class, "TEXTURE_TYPE_CUBE_LEFT", "Lcom/riseapps/marusyaobjloader/model/material/TextureType;");
    texture_type_jni->j_texture_type_cube_right_field_id = env->GetStaticFieldID(texture_type_jni->j_texture_type_class, "TEXTURE_TYPE_CUBE_RIGHT", "Lcom/riseapps/marusyaobjloader/model/material/TextureType;");
}

void LoadHashMapJNIDetails(JNIEnv *env) {
    hash_map_jni = new HASH_MAP_JNI;
    hash_map_jni->j_hash_map_class = env->FindClass("java/util/HashMap");
    hash_map_jni->j_hash_map_constructor = env->GetMethodID(hash_map_jni->j_hash_map_class, "<init>", "(I)V");
    hash_map_jni->j_hash_map_put = env->GetMethodID(hash_map_jni->j_hash_map_class, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
}

void ReleaseJNIDetails() {
    if (result_model_jni != nullptr) {
        delete result_model_jni;
        result_model_jni = nullptr;
    }
    if (shape_model_jni != nullptr) {
        delete shape_model_jni;
        shape_model_jni = nullptr;
    }
    if (mesh_model_jni != nullptr) {
        delete mesh_model_jni;
        mesh_model_jni = nullptr;
    }
    if (texture_option_model_jni != nullptr) {
        delete texture_option_model_jni;
        texture_option_model_jni = nullptr;
    }
    if (texture_type_jni != nullptr) {
        delete texture_type_jni;
        texture_type_jni = nullptr;
    }
    if (hash_map_jni != nullptr) {
        delete hash_map_jni;
        hash_map_jni = nullptr;
    }
}

void LoadJNIDetails(JNIEnv * env) {
    LoadResultModelJNIDetails(env);
    LoadShapeModelJNIDetails(env);
    LoadMeshModelJNIDetails(env);
    LoadMaterialModelJNIDetails(env);
    LoadTextureOptionModelJNIDetails(env);
    LoadTextureTypeJNIDetails(env);
    LoadHashMapJNIDetails(env);
}

std::string GetBaseDir(const std::string &filepath) {
    if (filepath.find_last_of("/\\") != std::string::npos) {
        return filepath.substr(0, filepath.find_last_of("/\\")) + "/";
    } else {
        return "";
    }
}

jobject GenerateResultModelDueToError(JNIEnv * env, const std::string &err) {
    jobject j_result_model = env->NewObject(result_model_jni->j_result_model_class, result_model_jni->j_result_model_constructor);
    jstring j_error = env->NewStringUTF(err.c_str());
    env->SetObjectField(j_result_model, result_model_jni->j_error_field_id, j_error);
    return j_result_model;
}

void GenerateShapeModels(JNIEnv * env,
                         jobject &j_result_model,
                         const tinyobj::attrib_t &attrib,
                         const std::vector<tinyobj::shape_t> &shapes,
                         const jboolean flip_texcoords) {
    jobjectArray shape_models = env->NewObjectArray((jsize) shapes.size(), shape_model_jni->j_shape_model_class, nullptr);
    for (size_t s = 0; s < shapes.size(); s++) {
        jobject j_shape_model = env->NewObject(shape_model_jni->j_shape_model_class, shape_model_jni->j_shape_model_constructor);
        jobject j_mesh_model = env->NewObject(mesh_model_jni->j_mesh_model_class, mesh_model_jni->j_mesh_model_constructor);

        // vertices
        tinyobj::shape_t shape = shapes[s];
        unsigned long vertices_size =
                (!attrib.vertices.empty() ? shape.mesh.indices.size() * VERTICES_NUM_COMPONENTS : 0)
                + (!attrib.normals.empty() ? shape.mesh.indices.size() * NORMALS_NUM_COMPONENTS : 0)
                + (!attrib.texcoords.empty() ? shape.mesh.indices.size() * TEX_COORDS_NUM_COMPONENTS : 0);
        jfloatArray vertices_array = env->NewFloatArray((jsize) vertices_size);
        jboolean is_copy_vertices;
        jfloat *body_vertices = env->GetFloatArrayElements(vertices_array, &is_copy_vertices);

        // indices
        jintArray indices_array = env->NewIntArray((jsize) shape.mesh.indices.size());
        jboolean is_copy_indices;
        jint *body_indices = env->GetIntArrayElements(indices_array, &is_copy_indices);

        size_t vertices_count = 0;
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                for (size_t vn = 0; vn < VERTICES_NUM_COMPONENTS; vn++) {
                    tinyobj::real_t vertex = attrib.vertices[VERTICES_NUM_COMPONENTS * idx.vertex_index + vn];
                    body_vertices[vertices_count] = vertex;
                    vertices_count++;
                }
                if (!attrib.normals.empty()) {
                    for (size_t nn = 0; nn < NORMALS_NUM_COMPONENTS; nn++) {
                        tinyobj::real_t normal = attrib.normals[NORMALS_NUM_COMPONENTS * idx.normal_index + nn];
                        body_vertices[vertices_count] = normal;
                        vertices_count++;
                    }
                }
                if (!attrib.texcoords.empty()) {
                    for (size_t tcn = 0; tcn < TEX_COORDS_NUM_COMPONENTS; tcn++) {
                        tinyobj::real_t tex_coord = attrib.texcoords[TEX_COORDS_NUM_COMPONENTS * idx.texcoord_index + tcn];
                        body_vertices[vertices_count] = flip_texcoords ? 1 - tex_coord : tex_coord;
                        vertices_count++;
                    }
                }
            }
            index_offset += fv;
        }
        for (size_t i = 0; i < shape.mesh.indices.size(); i++) {
            body_indices[i] = (jshort) i;
        }

        // mesh model
        env->SetFloatArrayRegion(vertices_array, 0, env->GetArrayLength(vertices_array), body_vertices);
        env->SetIntArrayRegion(indices_array, 0, env->GetArrayLength(indices_array), body_indices);
        // vertices field
        env->SetObjectField(j_mesh_model, mesh_model_jni->j_vertices_field_id, vertices_array);
        // indices field
        env->SetObjectField(j_mesh_model, mesh_model_jni->j_indices_field_id, indices_array);
        // shape model
        // name field
        env->SetObjectField(j_shape_model, shape_model_jni->j_name_field_id, env->NewStringUTF(shape.name.c_str()));
        // mesh model field
        env->SetObjectField(j_shape_model, shape_model_jni->j_mesh_model_field_id, j_mesh_model);
        // shape model field
        env->SetObjectArrayElement(shape_models, (jsize) s, j_shape_model);
    }
    // shape models field
    env->SetObjectField(j_result_model, result_model_jni->j_shape_models_field_id, shape_models);
}

void SetMaterialAmbientField(JNIEnv * env,
                             jobject &j_material_model,
                             const tinyobj::material_t &material) {
    jfloatArray j_ambient_array = env->NewFloatArray((jsize) sizeof(material.ambient) / sizeof(material.ambient[0]));
    jboolean is_copy_ambient;
    jfloat *body_ambient = env->GetFloatArrayElements(j_ambient_array, &is_copy_ambient);
    for (size_t i = 0; i < sizeof(material.ambient) / sizeof(material.ambient[0]); i++) {
        body_ambient[i] = material.ambient[i];
    }
    env->SetFloatArrayRegion(j_ambient_array, 0, env->GetArrayLength(j_ambient_array), body_ambient);
    env->SetObjectField(j_material_model, material_model_jni->j_ambient_field_id, j_ambient_array);
}

void SetMaterialDiffuseField(JNIEnv * env,
                             jobject &j_material_model,
                             const tinyobj::material_t &material) {
    jfloatArray j_diffuse_array = env->NewFloatArray((jsize) sizeof(material.diffuse) / sizeof(material.diffuse[0]));
    jboolean is_copy_diffuse;
    jfloat *body_diffuse = env->GetFloatArrayElements(j_diffuse_array, &is_copy_diffuse);
    for (size_t i = 0; i < sizeof(material.diffuse) / sizeof(material.diffuse[0]); i++) {
        body_diffuse[i] = material.diffuse[i];
    }
    env->SetFloatArrayRegion(j_diffuse_array, 0, env->GetArrayLength(j_diffuse_array), body_diffuse);
    env->SetObjectField(j_material_model, material_model_jni->j_diffuse_field_id, j_diffuse_array);
}

void SetMaterialSpecularField(JNIEnv * env,
                              jobject &j_material_model,
                              const tinyobj::material_t &material) {
    jfloatArray j_specular_array = env->NewFloatArray((jsize) sizeof(material.specular) / sizeof(material.specular[0]));
    jboolean is_copy_specular;
    jfloat *body_specular = env->GetFloatArrayElements(j_specular_array, &is_copy_specular);
    for (size_t i = 0; i < sizeof(material.specular) / sizeof(material.specular[0]); i++) {
        body_specular[i] = material.specular[i];
    }
    env->SetFloatArrayRegion(j_specular_array, 0, env->GetArrayLength(j_specular_array), body_specular);
    env->SetObjectField(j_material_model, material_model_jni->j_specular_field_id, j_specular_array);
}

void SetMaterialTransmittanceField(JNIEnv * env,
                                   jobject &j_material_model,
                                   const tinyobj::material_t &material) {
    jfloatArray j_transmittance_array = env->NewFloatArray((jsize) sizeof(material.transmittance) / sizeof(material.transmittance[0]));
    jboolean is_copy_transmittance;
    jfloat *body_transmittance = env->GetFloatArrayElements(j_transmittance_array, &is_copy_transmittance);
    for (size_t i = 0; i < sizeof(material.transmittance) / sizeof(material.transmittance[0]); i++) {
        body_transmittance[i] = material.transmittance[i];
    }
    env->SetFloatArrayRegion(j_transmittance_array, 0, env->GetArrayLength(j_transmittance_array), body_transmittance);
    env->SetObjectField(j_material_model, material_model_jni->j_transmittance_field_id, j_transmittance_array);
}

void SetMaterialEmissionField(JNIEnv * env,
                              jobject &j_material_model,
                              const tinyobj::material_t &material) {
    jfloatArray j_emission_array = env->NewFloatArray((jsize) sizeof(material.emission) / sizeof(material.emission[0]));
    jboolean is_copy_emission;
    jfloat *body_emission = env->GetFloatArrayElements(j_emission_array, &is_copy_emission);
    for (size_t i = 0; i < sizeof(material.emission) / sizeof(material.emission[0]); i++) {
        body_emission[i] = material.emission[i];
    }
    env->SetFloatArrayRegion(j_emission_array, 0, env->GetArrayLength(j_emission_array), body_emission);
    env->SetObjectField(j_material_model, material_model_jni->j_emission_field_id, j_emission_array);
}

void SetMaterialShininessField(JNIEnv * env,
                               jobject &j_material_model,
                               const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_shininess_field_id, material.shininess);
}

void SetMaterialIorField(JNIEnv * env,
                         jobject &j_material_model,
                         const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_ior_field_id, material.ior);
}

void SetMaterialDissolveField(JNIEnv * env,
                              jobject &j_material_model,
                              const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_dissolve_field_id, material.dissolve);
}

void SetMaterialIllumField(JNIEnv * env,
                           jobject &j_material_model,
                           const tinyobj::material_t &material) {
    env->SetIntField(j_material_model, material_model_jni->j_illum_field_id, material.illum);
}

void SetMaterialDummyField(JNIEnv * env,
                           jobject &j_material_model,
                           const tinyobj::material_t &material) {
    env->SetIntField(j_material_model, material_model_jni->j_dummy_field_id, material.dummy);
}

void SetMaterialAmbientTexnameField(JNIEnv * env,
                                    jobject &j_material_model,
                                    const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_ambient_texname_field_id, env->NewStringUTF(material.ambient_texname.c_str()));
}

void SetMaterialDiffuseTexnameField(JNIEnv * env,
                                    jobject &j_material_model,
                                    const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_diffuse_texname_field_id, env->NewStringUTF(material.diffuse_texname.c_str()));
}

void SetMaterialSpecularTexnameField(JNIEnv * env,
                                     jobject &j_material_model,
                                     const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_specular_texname_field_id, env->NewStringUTF(material.specular_texname.c_str()));
}

void SetMaterialSpecularHighlightTexnameField(JNIEnv * env,
                                              jobject &j_material_model,
                                              const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_specular_highlight_texname_field_id, env->NewStringUTF(material.specular_highlight_texname.c_str()));
}

void SetMaterialBumpTexnameField(JNIEnv * env,
                                 jobject &j_material_model,
                                 const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_bump_texname_field_id, env->NewStringUTF(material.bump_texname.c_str()));
}

void SetMaterialDisplacementTexnameField(JNIEnv * env,
                                         jobject &j_material_model,
                                         const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_displacement_texname_field_id, env->NewStringUTF(material.displacement_texname.c_str()));
}

void SetMaterialAlphaTexnameField(JNIEnv * env,
                                  jobject &j_material_model,
                                  const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_alpha_texname_field_id, env->NewStringUTF(material.alpha_texname.c_str()));
}

void SetMaterialReflectionTexnameField(JNIEnv * env,
                                       jobject &j_material_model,
                                       const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_reflection_texname_field_id, env->NewStringUTF(material.reflection_texname.c_str()));
}

void SetMaterialAmbientTexoptField(JNIEnv * env,
                                   jobject &j_material_model,
                                   const tinyobj::material_t &material) {
    jobject j_ambient_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_ambient_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_ambient_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_ambient_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_ambient_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_ambient_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_ambient_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_ambient_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_ambient_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_ambient_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_ambient_texopt_model, texture_option_model_jni->j_type_field_id, j_ambient_texture_type);
    // sharpness field
    env->SetFloatField(j_ambient_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.ambient_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_ambient_texopt_model, texture_option_model_jni->j_brightness_field_id, material.ambient_texopt.brightness);
    // contrast field
    env->SetFloatField(j_ambient_texopt_model, texture_option_model_jni->j_contrast_field_id, material.ambient_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.ambient_texopt.origin_offset) / sizeof(material.ambient_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.ambient_texopt.origin_offset) / sizeof(material.ambient_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.ambient_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_ambient_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.ambient_texopt.scale) / sizeof(material.ambient_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.ambient_texopt.scale) / sizeof(material.ambient_texopt.scale[0]); i++) {
        body_scale[i] = material.ambient_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_ambient_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.ambient_texopt.turbulence) / sizeof(material.ambient_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.ambient_texopt.turbulence) / sizeof(material.ambient_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.ambient_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_ambient_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_ambient_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.ambient_texopt.clamp);
    // imfchan field
    env->SetCharField(j_ambient_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.ambient_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_ambient_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.ambient_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_ambient_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.ambient_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_ambient_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.ambient_texopt.colorspace.c_str()));
    // ambient texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_ambient_texopt_field_id, j_ambient_texopt_model);
}

void SetMaterialDiffuseTexoptField(JNIEnv * env,
                                   jobject &j_material_model,
                                   const tinyobj::material_t &material) {
    jobject j_diffuse_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_diffuse_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_diffuse_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_diffuse_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_diffuse_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_diffuse_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_diffuse_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_diffuse_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_diffuse_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_diffuse_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_diffuse_texopt_model, texture_option_model_jni->j_type_field_id, j_diffuse_texture_type);
    // sharpness field
    env->SetFloatField(j_diffuse_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.diffuse_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_diffuse_texopt_model, texture_option_model_jni->j_brightness_field_id, material.diffuse_texopt.brightness);
    // contrast field
    env->SetFloatField(j_diffuse_texopt_model, texture_option_model_jni->j_contrast_field_id, material.diffuse_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.diffuse_texopt.origin_offset) / sizeof(material.diffuse_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.diffuse_texopt.origin_offset) / sizeof(material.diffuse_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.diffuse_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_diffuse_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.diffuse_texopt.scale) / sizeof(material.diffuse_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.diffuse_texopt.scale) / sizeof(material.diffuse_texopt.scale[0]); i++) {
        body_scale[i] = material.diffuse_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_diffuse_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.diffuse_texopt.turbulence) / sizeof(material.diffuse_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.diffuse_texopt.turbulence) / sizeof(material.diffuse_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.diffuse_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_diffuse_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_diffuse_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.diffuse_texopt.clamp);
    // imfchan field
    env->SetCharField(j_diffuse_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.diffuse_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_diffuse_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.diffuse_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_diffuse_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.diffuse_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_diffuse_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.diffuse_texopt.colorspace.c_str()));
    // diffuse texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_diffuse_texopt_field_id, j_diffuse_texopt_model);
}

void SetMaterialSpecularTexoptField(JNIEnv * env,
                                    jobject &j_material_model,
                                    const tinyobj::material_t &material) {
    jobject j_specular_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_specular_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_specular_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_specular_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_specular_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_specular_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_specular_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_specular_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_specular_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_specular_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_specular_texopt_model, texture_option_model_jni->j_type_field_id, j_specular_texture_type);
    // sharpness field
    env->SetFloatField(j_specular_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.specular_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_specular_texopt_model, texture_option_model_jni->j_brightness_field_id, material.specular_texopt.brightness);
    // contrast field
    env->SetFloatField(j_specular_texopt_model, texture_option_model_jni->j_contrast_field_id, material.specular_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.specular_texopt.origin_offset) / sizeof(material.specular_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.specular_texopt.origin_offset) / sizeof(material.specular_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.specular_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_specular_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.specular_texopt.scale) / sizeof(material.specular_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.specular_texopt.scale) / sizeof(material.specular_texopt.scale[0]); i++) {
        body_scale[i] = material.specular_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_specular_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.specular_texopt.turbulence) / sizeof(material.specular_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.specular_texopt.turbulence) / sizeof(material.specular_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.specular_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_specular_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_specular_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.specular_texopt.clamp);
    // imfchan field
    env->SetCharField(j_specular_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.specular_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_specular_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.specular_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_specular_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.specular_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_specular_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.specular_texopt.colorspace.c_str()));
    // specular texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_specular_texopt_field_id, j_specular_texopt_model);
}

void SetMaterialSpecularHighlightTexoptField(JNIEnv * env,
                                             jobject &j_material_model,
                                             const tinyobj::material_t &material) {
    jobject j_specular_highlight_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_specular_highlight_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_specular_highlight_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_specular_highlight_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_specular_highlight_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_specular_highlight_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_specular_highlight_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_specular_highlight_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_specular_highlight_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_specular_highlight_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_specular_highlight_texopt_model, texture_option_model_jni->j_type_field_id, j_specular_highlight_texture_type);
    // sharpness field
    env->SetFloatField(j_specular_highlight_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.specular_highlight_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_specular_highlight_texopt_model, texture_option_model_jni->j_brightness_field_id, material.specular_highlight_texopt.brightness);
    // contrast field
    env->SetFloatField(j_specular_highlight_texopt_model, texture_option_model_jni->j_contrast_field_id, material.specular_highlight_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.specular_highlight_texopt.origin_offset) / sizeof(material.specular_highlight_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.specular_highlight_texopt.origin_offset) / sizeof(material.specular_highlight_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.specular_highlight_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_specular_highlight_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.specular_highlight_texopt.scale) / sizeof(material.specular_highlight_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.specular_highlight_texopt.scale) / sizeof(material.specular_highlight_texopt.scale[0]); i++) {
        body_scale[i] = material.specular_highlight_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_specular_highlight_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.specular_highlight_texopt.turbulence) / sizeof(material.specular_highlight_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.specular_highlight_texopt.turbulence) / sizeof(material.specular_highlight_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.specular_highlight_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_specular_highlight_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_specular_highlight_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.specular_highlight_texopt.clamp);
    // imfchan field
    env->SetCharField(j_specular_highlight_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.specular_highlight_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_specular_highlight_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.specular_highlight_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_specular_highlight_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.specular_highlight_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_specular_highlight_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.specular_highlight_texopt.colorspace.c_str()));
    // specular highlight texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_specular_highlight_texopt_field_id, j_specular_highlight_texopt_model);
}

void SetMaterialBumpTexoptField(JNIEnv * env,
                                jobject &j_material_model,
                                const tinyobj::material_t &material) {
    jobject j_bump_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_bump_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_bump_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_bump_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_bump_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_bump_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_bump_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_bump_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_bump_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_bump_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_bump_texopt_model, texture_option_model_jni->j_type_field_id, j_bump_texture_type);
    // sharpness field
    env->SetFloatField(j_bump_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.bump_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_bump_texopt_model, texture_option_model_jni->j_brightness_field_id, material.bump_texopt.brightness);
    // contrast field
    env->SetFloatField(j_bump_texopt_model, texture_option_model_jni->j_contrast_field_id, material.bump_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.bump_texopt.origin_offset) / sizeof(material.bump_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.bump_texopt.origin_offset) / sizeof(material.bump_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.bump_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_bump_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.bump_texopt.scale) / sizeof(material.bump_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.bump_texopt.scale) / sizeof(material.bump_texopt.scale[0]); i++) {
        body_scale[i] = material.bump_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_bump_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.bump_texopt.turbulence) / sizeof(material.bump_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.bump_texopt.turbulence) / sizeof(material.bump_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.bump_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_bump_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_bump_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.bump_texopt.clamp);
    // imfchan field
    env->SetCharField(j_bump_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.bump_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_bump_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.bump_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_bump_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.bump_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_bump_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.bump_texopt.colorspace.c_str()));
    // bump texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_bump_texopt_field_id, j_bump_texopt_model);
}

void SetMaterialDisplacementTexoptField(JNIEnv * env,
                                        jobject &j_material_model,
                                        const tinyobj::material_t &material) {
    jobject j_displacement_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_displacement_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_displacement_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_displacement_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_displacement_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_displacement_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_displacement_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_displacement_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_displacement_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_displacement_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_displacement_texopt_model, texture_option_model_jni->j_type_field_id, j_displacement_texture_type);
    // sharpness field
    env->SetFloatField(j_displacement_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.displacement_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_displacement_texopt_model, texture_option_model_jni->j_brightness_field_id, material.displacement_texopt.brightness);
    // contrast field
    env->SetFloatField(j_displacement_texopt_model, texture_option_model_jni->j_contrast_field_id, material.displacement_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.displacement_texopt.origin_offset) / sizeof(material.displacement_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.displacement_texopt.origin_offset) / sizeof(material.displacement_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.displacement_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_displacement_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.displacement_texopt.scale) / sizeof(material.displacement_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.displacement_texopt.scale) / sizeof(material.displacement_texopt.scale[0]); i++) {
        body_scale[i] = material.displacement_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_displacement_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.displacement_texopt.turbulence) / sizeof(material.displacement_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.displacement_texopt.turbulence) / sizeof(material.displacement_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.displacement_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_displacement_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_displacement_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.displacement_texopt.clamp);
    // imfchan field
    env->SetCharField(j_displacement_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.displacement_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_displacement_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.displacement_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_displacement_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.displacement_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_displacement_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.displacement_texopt.colorspace.c_str()));
    // displacement texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_displacement_texopt_field_id, j_displacement_texopt_model);
}

void SetMaterialAlphaTexoptField(JNIEnv * env,
                                 jobject &j_material_model,
                                 const tinyobj::material_t &material) {
    jobject j_alpha_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_alpha_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_alpha_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_alpha_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_alpha_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_alpha_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_alpha_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_alpha_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_alpha_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_alpha_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_alpha_texopt_model, texture_option_model_jni->j_type_field_id, j_alpha_texture_type);
    // sharpness field
    env->SetFloatField(j_alpha_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.alpha_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_alpha_texopt_model, texture_option_model_jni->j_brightness_field_id, material.alpha_texopt.brightness);
    // contrast field
    env->SetFloatField(j_alpha_texopt_model, texture_option_model_jni->j_contrast_field_id, material.alpha_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.alpha_texopt.origin_offset) / sizeof(material.alpha_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.alpha_texopt.origin_offset) / sizeof(material.alpha_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.alpha_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_alpha_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.alpha_texopt.scale) / sizeof(material.alpha_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.alpha_texopt.scale) / sizeof(material.alpha_texopt.scale[0]); i++) {
        body_scale[i] = material.alpha_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_alpha_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.alpha_texopt.turbulence) / sizeof(material.alpha_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.alpha_texopt.turbulence) / sizeof(material.alpha_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.alpha_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_alpha_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_alpha_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.alpha_texopt.clamp);
    // imfchan field
    env->SetCharField(j_alpha_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.alpha_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_alpha_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.alpha_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_alpha_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.alpha_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_alpha_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.alpha_texopt.colorspace.c_str()));
    // alpha texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_alpha_texopt_field_id, j_alpha_texopt_model);
}

void SetMaterialReflectionTexoptField(JNIEnv * env,
                                      jobject &j_material_model,
                                      const tinyobj::material_t &material) {
    jobject j_reflection_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_reflection_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_reflection_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_reflection_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_reflection_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_reflection_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_reflection_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_reflection_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_reflection_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_reflection_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_reflection_texopt_model, texture_option_model_jni->j_type_field_id, j_reflection_texture_type);
    // sharpness field
    env->SetFloatField(j_reflection_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.reflection_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_reflection_texopt_model, texture_option_model_jni->j_brightness_field_id, material.reflection_texopt.brightness);
    // contrast field
    env->SetFloatField(j_reflection_texopt_model, texture_option_model_jni->j_contrast_field_id, material.reflection_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.reflection_texopt.origin_offset) / sizeof(material.reflection_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.reflection_texopt.origin_offset) / sizeof(material.reflection_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.reflection_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_reflection_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.reflection_texopt.scale) / sizeof(material.reflection_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.reflection_texopt.scale) / sizeof(material.reflection_texopt.scale[0]); i++) {
        body_scale[i] = material.reflection_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_reflection_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.reflection_texopt.turbulence) / sizeof(material.reflection_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.reflection_texopt.turbulence) / sizeof(material.reflection_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.reflection_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_reflection_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_reflection_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.reflection_texopt.clamp);
    // imfchan field
    env->SetCharField(j_reflection_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.reflection_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_reflection_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.reflection_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_reflection_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.reflection_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_reflection_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.reflection_texopt.colorspace.c_str()));
    // reflection texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_reflection_texopt_field_id, j_reflection_texopt_model);
}

void SetMaterialRoughnessField(JNIEnv * env,
                               jobject &j_material_model,
                               const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_roughness_field_id, material.roughness);
}

void SetMaterialMetallicField(JNIEnv * env,
                              jobject &j_material_model,
                              const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_metallic_field_id, material.metallic);
}

void SetMaterialSheenField(JNIEnv * env,
                           jobject &j_material_model,
                           const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_sheen_field_id, material.sheen);
}

void SetMaterialClearcoatThicknessField(JNIEnv * env,
                                        jobject &j_material_model,
                                        const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_clearcoat_thickness_field_id, material.clearcoat_thickness);
}

void SetMaterialClearcoatRoughnessField(JNIEnv * env,
                                        jobject &j_material_model,
                                        const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_clearcoat_roughness_field_id, material.clearcoat_roughness);
}

void SetMaterialAnisotrophyField(JNIEnv * env,
                                 jobject &j_material_model,
                                 const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_anisotropy_field_id, material.anisotropy);
}

void SetMaterialAnisotrophyRotationField(JNIEnv * env,
                                         jobject &j_material_model,
                                         const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_anisotropy_rotation_field_id, material.anisotropy_rotation);
}

void SetMaterialPad0Field(JNIEnv * env,
                          jobject &j_material_model,
                          const tinyobj::material_t &material) {
    env->SetFloatField(j_material_model, material_model_jni->j_pad0_field_id, material.pad0);
}

void SetMaterialRoughnessTexnameField(JNIEnv * env,
                                     jobject &j_material_model,
                                     const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_roughness_texname_field_id, env->NewStringUTF(material.roughness_texname.c_str()));
}

void SetMaterialMetallicTexnameField(JNIEnv * env,
                                     jobject &j_material_model,
                                     const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_metallic_texname_field_id, env->NewStringUTF(material.metallic_texname.c_str()));
}

void SetMaterialSheenTexnameField(JNIEnv * env,
                                  jobject &j_material_model,
                                  const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_sheen_texname_field_id, env->NewStringUTF(material.sheen_texname.c_str()));
}

void SetMaterialEmissiveTexnameField(JNIEnv * env,
                                     jobject &j_material_model,
                                     const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_emissive_texname_field_id, env->NewStringUTF(material.emissive_texname.c_str()));
}

void SetMaterialNormalTexnameField(JNIEnv * env,
                                   jobject &j_material_model,
                                   const tinyobj::material_t &material) {
    env->SetObjectField(j_material_model, material_model_jni->j_normal_texname_field_id, env->NewStringUTF(material.normal_texname.c_str()));
}

void SetMaterialRoughnessTexoptField(JNIEnv * env,
                                     jobject &j_material_model,
                                     const tinyobj::material_t &material) {
    jobject j_roughness_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_roughness_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_roughness_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_roughness_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_roughness_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_roughness_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_roughness_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_roughness_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_roughness_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_roughness_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_roughness_texopt_model, texture_option_model_jni->j_type_field_id, j_roughness_texture_type);
    // sharpness field
    env->SetFloatField(j_roughness_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.roughness_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_roughness_texopt_model, texture_option_model_jni->j_brightness_field_id, material.roughness_texopt.brightness);
    // contrast field
    env->SetFloatField(j_roughness_texopt_model, texture_option_model_jni->j_contrast_field_id, material.roughness_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.roughness_texopt.origin_offset) / sizeof(material.roughness_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.roughness_texopt.origin_offset) / sizeof(material.roughness_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.roughness_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_roughness_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.roughness_texopt.scale) / sizeof(material.roughness_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.roughness_texopt.scale) / sizeof(material.roughness_texopt.scale[0]); i++) {
        body_scale[i] = material.roughness_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_roughness_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.roughness_texopt.turbulence) / sizeof(material.roughness_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.roughness_texopt.turbulence) / sizeof(material.roughness_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.roughness_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_roughness_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_roughness_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.roughness_texopt.clamp);
    // imfchan field
    env->SetCharField(j_roughness_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.roughness_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_roughness_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.roughness_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_roughness_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.roughness_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_roughness_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.roughness_texopt.colorspace.c_str()));
    // roughness texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_roughness_texopt_field_id, j_roughness_texopt_model);
}

void SetMaterialMetallicTexoptField(JNIEnv * env,
                                    jobject &j_material_model,
                                    const tinyobj::material_t &material) {
    jobject j_metallic_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_metallic_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_metallic_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_metallic_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_metallic_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_metallic_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_metallic_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_metallic_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_metallic_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_metallic_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_metallic_texopt_model, texture_option_model_jni->j_type_field_id, j_metallic_texture_type);
    // sharpness field
    env->SetFloatField(j_metallic_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.metallic_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_metallic_texopt_model, texture_option_model_jni->j_brightness_field_id, material.metallic_texopt.brightness);
    // contrast field
    env->SetFloatField(j_metallic_texopt_model, texture_option_model_jni->j_contrast_field_id, material.metallic_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.metallic_texopt.origin_offset) / sizeof(material.metallic_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.metallic_texopt.origin_offset) / sizeof(material.metallic_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.metallic_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_metallic_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.metallic_texopt.scale) / sizeof(material.metallic_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.metallic_texopt.scale) / sizeof(material.metallic_texopt.scale[0]); i++) {
        body_scale[i] = material.metallic_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_metallic_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.metallic_texopt.turbulence) / sizeof(material.metallic_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.metallic_texopt.turbulence) / sizeof(material.metallic_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.metallic_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_metallic_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_metallic_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.metallic_texopt.clamp);
    // imfchan field
    env->SetCharField(j_metallic_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.metallic_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_metallic_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.metallic_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_metallic_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.metallic_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_metallic_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.metallic_texopt.colorspace.c_str()));
    // metallic texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_metallic_texopt_field_id, j_metallic_texopt_model);
}

void SetMaterialSheenTexoptField(JNIEnv * env,
                                 jobject &j_material_model,
                                 const tinyobj::material_t &material) {
    jobject j_sheen_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_sheen_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_sheen_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_sheen_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_sheen_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_sheen_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_sheen_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_sheen_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_sheen_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_sheen_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_sheen_texopt_model, texture_option_model_jni->j_type_field_id, j_sheen_texture_type);
    // sharpness field
    env->SetFloatField(j_sheen_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.sheen_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_sheen_texopt_model, texture_option_model_jni->j_brightness_field_id, material.sheen_texopt.brightness);
    // contrast field
    env->SetFloatField(j_sheen_texopt_model, texture_option_model_jni->j_contrast_field_id, material.sheen_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.sheen_texopt.origin_offset) / sizeof(material.sheen_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.sheen_texopt.origin_offset) / sizeof(material.sheen_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.sheen_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_sheen_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.sheen_texopt.scale) / sizeof(material.sheen_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.sheen_texopt.scale) / sizeof(material.sheen_texopt.scale[0]); i++) {
        body_scale[i] = material.sheen_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_sheen_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.sheen_texopt.turbulence) / sizeof(material.sheen_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.sheen_texopt.turbulence) / sizeof(material.sheen_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.sheen_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_sheen_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_sheen_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.sheen_texopt.clamp);
    // imfchan field
    env->SetCharField(j_sheen_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.sheen_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_sheen_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.sheen_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_sheen_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.sheen_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_sheen_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.sheen_texopt.colorspace.c_str()));
    // sheen texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_sheen_texopt_field_id, j_sheen_texopt_model);
}

void SetMaterialEmissiveTexoptField(JNIEnv * env,
                                    jobject &j_material_model,
                                    const tinyobj::material_t &material) {
    jobject j_emissive_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_emissive_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_emissive_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_emissive_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_emissive_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_emissive_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_emissive_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_emissive_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_emissive_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_emissive_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_emissive_texopt_model, texture_option_model_jni->j_type_field_id, j_emissive_texture_type);
    // sharpness field
    env->SetFloatField(j_emissive_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.emissive_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_emissive_texopt_model, texture_option_model_jni->j_brightness_field_id, material.emissive_texopt.brightness);
    // contrast field
    env->SetFloatField(j_emissive_texopt_model, texture_option_model_jni->j_contrast_field_id, material.emissive_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.emissive_texopt.origin_offset) / sizeof(material.emissive_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.emissive_texopt.origin_offset) / sizeof(material.emissive_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.emissive_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_emissive_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.emissive_texopt.scale) / sizeof(material.emissive_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.emissive_texopt.scale) / sizeof(material.emissive_texopt.scale[0]); i++) {
        body_scale[i] = material.emissive_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_emissive_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.emissive_texopt.turbulence) / sizeof(material.emissive_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.emissive_texopt.turbulence) / sizeof(material.emissive_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.emissive_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_emissive_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_emissive_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.emissive_texopt.clamp);
    // imfchan field
    env->SetCharField(j_emissive_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.emissive_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_emissive_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.emissive_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_emissive_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.emissive_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_emissive_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.emissive_texopt.colorspace.c_str()));
    // emissive texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_emissive_texopt_field_id, j_emissive_texopt_model);
}

void SetMaterialNormalTexoptField(JNIEnv * env,
                                  jobject &j_material_model,
                                  const tinyobj::material_t &material) {
    jobject j_normal_texopt_model = env->NewObject(texture_option_model_jni->j_texture_option_model_class, texture_option_model_jni->j_texture_option_model_constructor);
    jobject j_normal_texture_type = nullptr;
    switch (material.ambient_texopt.type) {
        case tinyobj::texture_type_t::TEXTURE_TYPE_NONE:
            j_normal_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_none_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_SPHERE:
            j_normal_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_sphere_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_TOP:
            j_normal_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_top_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BOTTOM:
            j_normal_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_bottom_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_FRONT:
            j_normal_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_front_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_BACK:
            j_normal_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_back_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_LEFT:
            j_normal_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_left_field_id);
            break;
        case tinyobj::texture_type_t::TEXTURE_TYPE_CUBE_RIGHT:
            j_normal_texture_type = env->GetStaticObjectField(texture_type_jni->j_texture_type_class, texture_type_jni->j_texture_type_cube_right_field_id);
            break;
    }
    // type field
    env->SetObjectField(j_normal_texopt_model, texture_option_model_jni->j_type_field_id, j_normal_texture_type);
    // sharpness field
    env->SetFloatField(j_normal_texopt_model, texture_option_model_jni->j_sharpness_field_id, material.normal_texopt.sharpness);
    // brightness field
    env->SetFloatField(j_normal_texopt_model, texture_option_model_jni->j_brightness_field_id, material.normal_texopt.brightness);
    // contrast field
    env->SetFloatField(j_normal_texopt_model, texture_option_model_jni->j_contrast_field_id, material.normal_texopt.contrast);
    // origin offset field
    jfloatArray j_origin_offset_array = env->NewFloatArray((jsize) sizeof(material.normal_texopt.origin_offset) / sizeof(material.normal_texopt.origin_offset[0]));
    jboolean is_copy_origin_offset;
    jfloat *body_origin_offset = env->GetFloatArrayElements(j_origin_offset_array, &is_copy_origin_offset);
    for (size_t i = 0; i < sizeof(material.normal_texopt.origin_offset) / sizeof(material.normal_texopt.origin_offset[0]); i++) {
        body_origin_offset[i] = material.normal_texopt.origin_offset[i];
    }
    env->SetFloatArrayRegion(j_origin_offset_array, 0, env->GetArrayLength(j_origin_offset_array), body_origin_offset);
    env->SetObjectField(j_normal_texopt_model, texture_option_model_jni->j_origin_offset_field_id, j_origin_offset_array);
    // scale field
    jfloatArray j_scale_array = env->NewFloatArray((jsize) sizeof(material.normal_texopt.scale) / sizeof(material.normal_texopt.scale[0]));
    jboolean is_copy_scale;
    jfloat *body_scale = env->GetFloatArrayElements(j_scale_array, &is_copy_scale);
    for (size_t i = 0; i < sizeof(material.normal_texopt.scale) / sizeof(material.normal_texopt.scale[0]); i++) {
        body_scale[i] = material.normal_texopt.scale[i];
    }
    env->SetFloatArrayRegion(j_scale_array, 0, env->GetArrayLength(j_scale_array), body_scale);
    env->SetObjectField(j_normal_texopt_model, texture_option_model_jni->j_scale_field_id, j_scale_array);
    // turbulence field
    jfloatArray j_turbulence_array = env->NewFloatArray((jsize) sizeof(material.normal_texopt.turbulence) / sizeof(material.normal_texopt.turbulence[0]));
    jboolean is_copy_turbulence;
    jfloat *body_turbulence = env->GetFloatArrayElements(j_turbulence_array, &is_copy_turbulence);
    for (size_t i = 0; i < sizeof(material.normal_texopt.turbulence) / sizeof(material.normal_texopt.turbulence[0]); i++) {
        body_turbulence[i] = material.normal_texopt.turbulence[i];
    }
    env->SetFloatArrayRegion(j_turbulence_array, 0, env->GetArrayLength(j_turbulence_array), body_turbulence);
    env->SetObjectField(j_normal_texopt_model, texture_option_model_jni->j_turbulence_field_id, j_turbulence_array);
    // clamp field
    env->SetBooleanField(j_normal_texopt_model, texture_option_model_jni->j_clamp_field_id, (jboolean) material.normal_texopt.clamp);
    // imfchan field
    env->SetCharField(j_normal_texopt_model, texture_option_model_jni->j_imfchan_field_id, (jchar) material.normal_texopt.imfchan);
    // blendu field
    env->SetBooleanField(j_normal_texopt_model, texture_option_model_jni->j_blend_u_field_id, (jboolean) material.normal_texopt.blendu);
    // blendv field
    env->SetBooleanField(j_normal_texopt_model, texture_option_model_jni->j_blend_v_field_id, (jboolean) material.normal_texopt.blendv);
    // colorspace field
    env->SetObjectField(j_normal_texopt_model, texture_option_model_jni->j_colorspace_field_id, env->NewStringUTF(material.normal_texopt.colorspace.c_str()));
    // normal texopt field
    env->SetObjectField(j_material_model, material_model_jni->j_normal_texopt_field_id, j_normal_texopt_model);
}

void SetMaterialPad2Field(JNIEnv * env,
                          jobject &j_material_model,
                          const tinyobj::material_t &material) {
    env->SetIntField(j_material_model, material_model_jni->j_pad2_field_id, material.pad2);
}

void SetMaterialUnknownParameterField(JNIEnv * env,
                                      jobject &j_material_model,
                                      const tinyobj::material_t &material) {
    jobject j_unknown_parameter = env->NewObject(hash_map_jni->j_hash_map_class, hash_map_jni->j_hash_map_constructor, material.unknown_parameter.size());
    for (const auto &it : material.unknown_parameter) {
        env->CallObjectMethod(j_unknown_parameter, hash_map_jni->j_hash_map_put, env->NewStringUTF(it.first.c_str()), env->NewStringUTF(it.second.c_str()));
    }
    env->SetObjectField(j_material_model, material_model_jni->j_unknown_parameter_field_id, j_unknown_parameter);
}

void GenerateMaterialModels(JNIEnv * env,
                            jobject &j_result_model,
                            const std::vector<tinyobj::material_t> &materials) {
    jobjectArray material_models = env->NewObjectArray((jsize) materials.size(), material_model_jni->j_material_model_class, nullptr);
    for (size_t m = 0; m < materials.size(); m++) {
        jobject j_material_model = env->NewObject(material_model_jni->j_material_model_class, material_model_jni->j_material_model_constructor);
        tinyobj::material_t material = materials[m];

        // material model
        env->SetObjectField(j_material_model, material_model_jni->j_name_field_id, env->NewStringUTF(material.name.c_str()));
        env->SetObjectArrayElement(material_models, (jsize) m, j_material_model);

        SetMaterialAmbientField(env, j_material_model, material);
        SetMaterialDiffuseField(env, j_material_model, material);
        SetMaterialSpecularField(env, j_material_model, material);
        SetMaterialTransmittanceField(env, j_material_model, material);
        SetMaterialEmissionField(env, j_material_model, material);
        SetMaterialShininessField(env, j_material_model, material);
        SetMaterialIorField(env, j_material_model, material);
        SetMaterialDissolveField(env, j_material_model, material);
        SetMaterialIllumField(env, j_material_model, material);
        SetMaterialDummyField(env, j_material_model, material);
        SetMaterialAmbientTexnameField(env, j_material_model, material);
        SetMaterialDiffuseTexnameField(env, j_material_model, material);
        SetMaterialSpecularTexnameField(env, j_material_model, material);
        SetMaterialSpecularHighlightTexnameField(env, j_material_model, material);
        SetMaterialBumpTexnameField(env, j_material_model, material);
        SetMaterialDisplacementTexnameField(env, j_material_model, material);
        SetMaterialAlphaTexnameField(env, j_material_model, material);
        SetMaterialReflectionTexnameField(env, j_material_model, material);
        SetMaterialAmbientTexoptField(env, j_material_model, material);
        SetMaterialDiffuseTexoptField(env, j_material_model, material);
        SetMaterialSpecularTexoptField(env, j_material_model, material);
        SetMaterialSpecularHighlightTexoptField(env, j_material_model, material);
        SetMaterialBumpTexoptField(env, j_material_model, material);
        SetMaterialDisplacementTexoptField(env, j_material_model, material);
        SetMaterialAlphaTexoptField(env, j_material_model, material);
        SetMaterialReflectionTexoptField(env, j_material_model, material);
        SetMaterialRoughnessField(env, j_material_model, material);
        SetMaterialMetallicField(env, j_material_model, material);
        SetMaterialSheenField(env, j_material_model, material);
        SetMaterialClearcoatThicknessField(env, j_material_model, material);
        SetMaterialClearcoatRoughnessField(env, j_material_model, material);
        SetMaterialAnisotrophyField(env, j_material_model, material);
        SetMaterialAnisotrophyRotationField(env, j_material_model, material);
        SetMaterialPad0Field(env, j_material_model, material);
        SetMaterialRoughnessTexnameField(env, j_material_model, material);
        SetMaterialMetallicTexnameField(env, j_material_model, material);
        SetMaterialSheenTexnameField(env, j_material_model, material);
        SetMaterialEmissiveTexnameField(env, j_material_model, material);
        SetMaterialNormalTexnameField(env, j_material_model, material);
        SetMaterialRoughnessTexoptField(env, j_material_model, material);
        SetMaterialMetallicTexoptField(env, j_material_model, material);
        SetMaterialSheenTexoptField(env, j_material_model, material);
        SetMaterialEmissiveTexoptField(env, j_material_model, material);
        SetMaterialNormalTexoptField(env, j_material_model, material);
        SetMaterialPad2Field(env, j_material_model, material);
        SetMaterialUnknownParameterField(env, j_material_model, material);
    }
    env->SetObjectField(j_result_model, result_model_jni->j_material_models_field_id, material_models);
}

void GenerateAttributes(JNIEnv * env,
                        jobject &j_result_model,
                        const tinyobj::attrib_t &attrib) {
    // vertices size field
    env->SetLongField(j_result_model, result_model_jni->j_vertices_size_field_id, attrib.vertices.size() / VERTICES_NUM_COMPONENTS);
    // normals size field
    env->SetLongField(j_result_model, result_model_jni->j_normals_size_field_id, attrib.normals.size() / NORMALS_NUM_COMPONENTS);
    // texcoords size field
    env->SetLongField(j_result_model, result_model_jni->j_texcoords_size_field_id, attrib.texcoords.size() / TEX_COORDS_NUM_COMPONENTS);
    // colors size field
    env->SetLongField(j_result_model, result_model_jni->j_colors_size_field_id, attrib.colors.size() / COLORS_NUM_COMPONENTS);
}

void GenerateMessages(JNIEnv * env,
                      jobject &j_result_model,
                      const std::string &warn,
                      const std::string &err) {
    // warn field
    env->SetObjectField(j_result_model, result_model_jni->j_warn_field_id, env->NewStringUTF(warn.c_str()));
    // error field
    env->SetObjectField(j_result_model, result_model_jni->j_error_field_id, env->NewStringUTF(err.c_str()));
}

template<typename... ArgTypes>
void log(const char* pattern, ArgTypes... args) {
    if (print_log) {
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, pattern, args...);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_riseapps_marusyaobjloader_MarusyaObjLoaderImpl_nativeLoad(JNIEnv *env,
                                                             jobject instance,
                                                             jstring obj_path,
                                                             jstring mtl_path,
                                                             jboolean flip_texcoord) {
    log("***********************************************************************************", NULL);
    std::chrono::time_point<std::chrono::high_resolution_clock> t_start, t_end;
    t_start = std::chrono::high_resolution_clock::now();

    ReleaseJNIDetails();
    LoadJNIDetails(env);
    // input files
    std::string obj_file_path = env->GetStringUTFChars(obj_path, (jboolean *) false);
    std::string mtl_file_path = env->GetStringUTFChars(mtl_path, (jboolean *) false);
    std::string mtl_base_dir = GetBaseDir(mtl_file_path);

    log("Start parsing -> obj: %s, mtl: %s", obj_file_path.c_str(), mtl_file_path.c_str());
    // parsing
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, obj_file_path.c_str(), mtl_base_dir.c_str());

    // when error
    if (!err.empty() && !ret) {
        log("Error while parsing -> %s", err.c_str());
        return GenerateResultModelDueToError(env, err);
    }

    log("End parsing -> %s", obj_file_path.c_str());

    // print parse data
    log("shapes size -> %lu", shapes.size());
    log("materials size -> %lu", materials.size());
    log("vertices size -> %lu", attrib.vertices.size());
    log("normals size -> %lu", attrib.normals.size());
    log("texcoords size -> %lu", attrib.texcoords.size());
    log("colors size -> %lu", attrib.colors.size());
    unsigned long indicesSize = 0;
    for (auto &shape : shapes) {
        indicesSize += shape.mesh.indices.size();
    }
    log("indices size -> %lu", indicesSize);

    // generate result model
    jobject j_result_model = env->NewObject(result_model_jni->j_result_model_class, result_model_jni->j_result_model_constructor);
    GenerateShapeModels(env, j_result_model, attrib, shapes, flip_texcoord);
    GenerateMaterialModels(env, j_result_model, materials);
    GenerateAttributes(env, j_result_model, attrib);
    GenerateMessages(env, j_result_model, warn, err);
    ReleaseJNIDetails();

    t_end = std::chrono::high_resolution_clock::now();
    log("Time to parse -> %ld ms", std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count());
    log("***********************************************************************************", NULL);

    return j_result_model;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_riseapps_marusyaobjloader_MarusyaObjLoaderImpl_nativeEnableLog(JNIEnv *env, jobject instance) {
    print_log = true;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_riseapps_marusyaobjloader_MarusyaObjLoaderImpl_nativeDisableLog(JNIEnv *env, jobject instance) {
    print_log = false;
}