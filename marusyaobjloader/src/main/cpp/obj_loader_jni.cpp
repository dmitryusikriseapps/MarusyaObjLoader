#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "MarusyaObjLoader"
#define  ALOG(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#define CLOCKS_PER_MS 1000

const int VERTICES_NUM_COMPONENTS = 3;
const int NORMALS_NUM_COMPONENTS = 3;
const int TEX_COORDS_NUM_COMPONENTS = 2;

bool print_log = true;

typedef struct {
    jclass j_result_model_class;
    jmethodID j_result_model_constructor;
    jfieldID j_shape_models_field_id;
    jfieldID j_warn_field_id;
    jfieldID j_error_field_id;
} RESULT_MODEL_JNI;

typedef struct {
    jclass j_shape_model_class;
    jmethodID j_shape_model_constructor;
    jfieldID j_mesh_model_field_id;
    jfieldID j_material_model_field_id;
} SHAPE_MODEL_JNI;

typedef struct {
    jclass j_mesh_model_class;
    jmethodID j_mesh_model_constructor;
    jfieldID j_vertices_field_id;
    jfieldID j_indices_field_id;
    jfieldID j_vertices_size_field_id;
    jfieldID j_normals_size_field_id;
    jfieldID j_tex_coords_size_field_id;
    jfieldID j_colors_size_field_id;

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
    jfieldID j_bump_multiplier;
} TEXTURE_OPTION_MODEL_JNI;

RESULT_MODEL_JNI * result_model_jni = NULL;
SHAPE_MODEL_JNI * shape_model_jni = NULL;
MESH_MODEL_JNI * mesh_model_jni = NULL;
MATERIAL_MODEL_JNI * material_model_jni = NULL;
TEXTURE_OPTION_MODEL_JNI * texture_option_model_jni = NULL;

void LoadResultModelJNIDetails(JNIEnv * env) {
    result_model_jni = new RESULT_MODEL_JNI;
    result_model_jni->j_result_model_class = env->FindClass("com/riseapps/marusyaobjloader/model/ResultModel");
    result_model_jni->j_result_model_constructor = env->GetMethodID(result_model_jni->j_result_model_class, "<init>", "()V");
    result_model_jni->j_shape_models_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "shapeModels", "[Lcom/riseapps/marusyaobjloader/model/ShapeModel;");
    result_model_jni->j_warn_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "warn", "Ljava/lang/String;");
    result_model_jni->j_error_field_id = env->GetFieldID(result_model_jni->j_result_model_class, "error", "Ljava/lang/String;");
}

void LoadShapeModelJNIDetails(JNIEnv * env) {
    shape_model_jni = new SHAPE_MODEL_JNI;
    shape_model_jni->j_shape_model_class = env->FindClass("com/riseapps/marusyaobjloader/model/ShapeModel");
    shape_model_jni->j_shape_model_constructor = env->GetMethodID(shape_model_jni->j_shape_model_class, "<init>", "()V");
    shape_model_jni->j_mesh_model_field_id = env->GetFieldID(shape_model_jni->j_shape_model_class, "meshModel", "Lcom/riseapps/marusyaobjloader/model/mesh/MeshModel;");
    shape_model_jni->j_material_model_field_id = env->GetFieldID(shape_model_jni->j_shape_model_class, "materialModel", "Lcom/riseapps/marusyaobjloader/model/material/MaterialModel;");
}

void LoadMeshModelJNIDetails(JNIEnv * env) {
    mesh_model_jni = new MESH_MODEL_JNI;
    mesh_model_jni->j_mesh_model_class = env->FindClass("com/riseapps/marusyaobjloader/model/mesh/MeshModel");
    mesh_model_jni->j_mesh_model_constructor = env->GetMethodID(mesh_model_jni->j_mesh_model_class, "<init>", "()V");
    mesh_model_jni->j_vertices_field_id = env->GetFieldID(mesh_model_jni->j_mesh_model_class, "vertices", "[F");
    mesh_model_jni->j_indices_field_id = env->GetFieldID(mesh_model_jni->j_mesh_model_class, "indices", "[I");
    mesh_model_jni->j_vertices_size_field_id = env->GetFieldID(mesh_model_jni->j_mesh_model_class, "verticesSize", "I");
    mesh_model_jni->j_normals_size_field_id = env->GetFieldID(mesh_model_jni->j_mesh_model_class, "normalsSize", "I");
    mesh_model_jni->j_tex_coords_size_field_id = env->GetFieldID(mesh_model_jni->j_mesh_model_class, "texCoordsSize", "I");
    mesh_model_jni->j_colors_size_field_id = env->GetFieldID(mesh_model_jni->j_mesh_model_class, "colorsSize", "I");
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
    texture_option_model_jni->j_imfchan_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "imfchan", "Ljava/lang/String;");
    texture_option_model_jni->j_blend_u_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "blendU", "Z");
    texture_option_model_jni->j_blend_v_field_id = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "blendV", "Z");
    texture_option_model_jni->j_bump_multiplier = env->GetFieldID(texture_option_model_jni->j_texture_option_model_class, "bumpMultiplier", "F");
}

void NullJNIDetails() {
    if (result_model_jni != NULL) {
        delete result_model_jni;
        result_model_jni = NULL;
    }
    if (shape_model_jni != NULL) {
        delete shape_model_jni;
        shape_model_jni = NULL;
    }
    if (mesh_model_jni != NULL) {
        delete mesh_model_jni;
        mesh_model_jni = NULL;
    }
    if (texture_option_model_jni != NULL) {
        delete texture_option_model_jni;
        texture_option_model_jni = NULL;
    }
}

void LoadJNIDetails(JNIEnv * env) {
    LoadResultModelJNIDetails(env);
    LoadShapeModelJNIDetails(env);
    LoadMeshModelJNIDetails(env);
    LoadMaterialModelJNIDetails(env);
    LoadTextureOptionModelJNIDetails(env);
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
                         const jboolean &flip_texcoords) {
    for (size_t s = 0; s < shapes.size(); s++) {
        jobject j_mesh_model = env->NewObject(mesh_model_jni->j_mesh_model_class, mesh_model_jni->j_mesh_model_constructor);

        // vertices
        tinyobj::shape_t shape = shapes[s];
        unsigned long vertices_size =
                (attrib.vertices.size() != 0 ? shape.mesh.indices.size() * VERTICES_NUM_COMPONENTS : 0)
                + (attrib.normals.size() != 0 ? shape.mesh.indices.size() * NORMALS_NUM_COMPONENTS : 0)
                + (attrib.texcoords.size() != 0 ? shape.mesh.indices.size() * TEX_COORDS_NUM_COMPONENTS : 0);
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
                if (attrib.normals.size() != 0) {
                    for (size_t nn = 0; nn < NORMALS_NUM_COMPONENTS; nn++) {
                        tinyobj::real_t normal = attrib.normals[NORMALS_NUM_COMPONENTS * idx.normal_index + nn];
                        body_vertices[vertices_count] = normal;
                        vertices_count++;
                    }
                }
                if (attrib.texcoords.size() != 0) {
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

        // data
        env->SetFloatArrayRegion(vertices_array, 0, env->GetArrayLength(vertices_array), body_vertices);
        env->SetIntArrayRegion(indices_array, 0, env->GetArrayLength(indices_array), body_indices);
        env->SetObjectField(j_mesh_model, mesh_model_jni->j_vertices_field_id, vertices_array);
        env->SetObjectField(j_mesh_model, mesh_model_jni->j_indices_field_id, indices_array);
        // data size
        env->SetIntField(j_mesh_model, mesh_model_jni->j_vertices_size_field_id, (jint) (attrib.vertices.size()));
        env->SetIntField(j_mesh_model, mesh_model_jni->j_normals_size_field_id, (jint) (attrib.normals.size()));
        env->SetIntField(j_mesh_model, mesh_model_jni->j_tex_coords_size_field_id, (jint) (attrib.texcoords.size()));
        env->SetIntField(j_mesh_model, mesh_model_jni->j_colors_size_field_id, (jint) (attrib.colors.size()));
        // releasing arrays
        env->ReleaseFloatArrayElements(vertices_array, body_vertices, JNI_ABORT);
        env->ReleaseIntArrayElements(indices_array, body_indices, JNI_ABORT);
    }
}

void GenerateMaterialModels(JNIEnv * env,
                            jobject &j_result_model,
                            const std::vector<tinyobj::material_t> &materials) {
}

void GenerateMessages(JNIEnv * env,
                      jobject &j_result_model,
                      const std::string &warn,
                      const std::string &err) {
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_riseapps_marusyaobjloader_MarusyaObjLoaderImpl_load(JNIEnv *env,
                                                             jobject instance,
                                                             jstring obj_path,
                                                             jboolean flip_texcoord) {
    clock_t startTime = clock();
    NullJNIDetails();
    LoadJNIDetails(env);

    std::string input_file = env->GetStringUTFChars(obj_path, (jboolean *) false);
    std::string base_dir = GetBaseDir(input_file);
    if (print_log) {
        ALOG("***********************************************************************************");
        ALOG("Start parsing -> %s", input_file.c_str());
    }
    // parsing process
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, input_file.c_str(), base_dir.c_str());

    // when error
    if (!err.empty() && !ret) {
        if (print_log) {
            ALOG("Error while parsing -> %s", err.c_str());
        }
        return GenerateResultModelDueToError(env, err);
    }

    if (print_log) {
        ALOG("End parsing -> %s", input_file.c_str());
    }

    // print parse data
    if (print_log) {
        ALOG("shapes size -> %lu", shapes.size());
        ALOG("material size -> %lu", materials.size());
        ALOG("vertices size -> %lu", attrib.vertices.size());
        ALOG("normals size -> %lu", attrib.normals.size());
        ALOG("texcoords size -> %lu", attrib.texcoords.size());
        ALOG("colors size -> %lu", attrib.colors.size());
        unsigned long indicesSize = 0;
        for (size_t i = 0; i < shapes.size(); i++) {
            indicesSize += shapes[i].mesh.indices.size();
        }
        ALOG("indices size -> %lu", indicesSize);
    }

    jobject j_result_model = env->NewObject(result_model_jni->j_result_model_class, result_model_jni->j_result_model_constructor);
    GenerateShapeModels(env, j_result_model, attrib, shapes, flip_texcoord);
    GenerateMaterialModels(env, j_result_model, materials);
    GenerateMessages(env, j_result_model, warn, err);

    clock_t endTime = clock();
    if (print_log) {
        ALOG("Time to parsing -> %ld ms", (endTime - startTime) / CLOCKS_PER_MS);
        ALOG("****************************************************************");
    }
    NullJNIDetails();

    return j_result_model;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_riseapps_marusyaobjloader_MarusyaObjLoaderImpl_enableLogJNI(JNIEnv *env, jobject instance) {
    print_log = true;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_riseapps_marusyaobjloader_MarusyaObjLoaderImpl_disableLogJNI(JNIEnv *env, jobject instance) {
    print_log = false;
}