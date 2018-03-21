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
#define  ALOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#define CLOCKS_PER_MS 1000

const int VERTICES_NUM_COMPONENTS = 3;
const int NORMALS_NUM_COMPONENTS = 3;
const int TEX_COORDS_NUM_COMPONENTS = 2;

const float NORM_COEFFICIENT = 100.0f;

extern "C"
JNIEXPORT jobject JNICALL
Java_com_riseapps_marusyaobjloader_MarusyaObjLoaderImpl_loadMesh(JNIEnv *env, jobject instance,
                                                                 jstring obj_path,
                                                                 jboolean normalize_vertices,
                                                                 jboolean flip_texcoord) {
    clock_t startTime = clock();
    std::string inputfile = env->GetStringUTFChars(obj_path, false);
    ALOG("****************************************************************");
    ALOG("Loading the mesh...");
    ALOG("Start parsing -> %s", inputfile.c_str());

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
    if (!err.empty() && !ret) {
        ALOG("Error parsing -> %s", err.c_str());
        jclass j_mesh_model_class = env->FindClass("com/riseapps/marusyaobjloader/model/MeshModel");
        jmethodID j_mesh_model_constructor = env->GetMethodID(j_mesh_model_class, "<init>", "()V");
        return env->NewObject(j_mesh_model_class, j_mesh_model_constructor);
    }
    ALOG("End parsing -> %s", inputfile.c_str());

    // print data
    ALOG("shapes size -> %lu", shapes.size());
    ALOG("material size -> %lu", materials.size());
    ALOG("vertices size -> %lu", attrib.vertices.size());
    ALOG("normals size -> %lu", attrib.normals.size());
    ALOG("texcoords size -> %lu", attrib.texcoords.size());
    ALOG("colors size -> %lu", attrib.colors.size());
    ALOG("mesh name -> %s", shapes[0].name.c_str());
    ALOG("indices size -> %lu", shapes[0].mesh.indices.size());

    jclass j_mesh_model_class = env->FindClass("com/riseapps/marusyaobjloader/model/MeshModel");
    jmethodID j_mesh_model_constructor = env->GetMethodID(j_mesh_model_class, "<init>", "()V");
    jobject j_mesh_model = env->NewObject(j_mesh_model_class, j_mesh_model_constructor);

    jfieldID j_vertices_field_id = env->GetFieldID(j_mesh_model_class, "vertices", "[F");
    jfieldID j_indices_field_id = env->GetFieldID(j_mesh_model_class, "indices", "[I");
    jfieldID j_vertices_size_field_id = env->GetFieldID(j_mesh_model_class, "verticesSize", "I");
    jfieldID j_normals_size_field_id = env->GetFieldID(j_mesh_model_class, "normalsSize", "I");
    jfieldID j_texture_coordinates_size_field_id = env->GetFieldID(j_mesh_model_class,
                                                                   "textureCoordinatesSize", "I");
    jfieldID j_colors_size_field_id = env->GetFieldID(j_mesh_model_class, "colorsSize", "I");

    // vertices
    unsigned long vertices_size =
            (attrib.vertices.size() != 0 ? shapes[0].mesh.indices.size() * VERTICES_NUM_COMPONENTS
                                         : 0)
            + (attrib.normals.size() != 0 ? shapes[0].mesh.indices.size() * NORMALS_NUM_COMPONENTS
                                          : 0)
            + (attrib.texcoords.size() != 0 ? shapes[0].mesh.indices.size() *
                                              TEX_COORDS_NUM_COMPONENTS : 0);
    jfloatArray vertices_array = env->NewFloatArray((jsize) vertices_size);
    jboolean is_copy_vertices;
    jfloat *body_vertices = env->GetFloatArrayElements(vertices_array, &is_copy_vertices);

    // indices
    jintArray indices_array = env->NewIntArray((jsize) shapes[0].mesh.indices.size());
    jboolean is_copy_indices;
    jint *body_indices = env->GetIntArrayElements(indices_array, &is_copy_indices);

    size_t vertices_count = 0;
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) {
        int fv = shapes[0].mesh.num_face_vertices[f];
        for (size_t v = 0; v < fv; v++) {
            tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];

            for (size_t vn = 0; vn < VERTICES_NUM_COMPONENTS; vn++) {
                tinyobj::real_t vertex = attrib.vertices[
                        VERTICES_NUM_COMPONENTS * idx.vertex_index + vn];
                body_vertices[vertices_count] = normalize_vertices ? vertex / NORM_COEFFICIENT
                                                                   : vertex;
                vertices_count++;
            }
            if (attrib.normals.size() != 0) {
                for (size_t nn = 0; nn < NORMALS_NUM_COMPONENTS; nn++) {
                    tinyobj::real_t normal = attrib.normals[
                            NORMALS_NUM_COMPONENTS * idx.normal_index + nn];
                    body_vertices[vertices_count] = normal;
                    vertices_count++;
                }
            }
            if (attrib.texcoords.size() != 0) {
                for (size_t tcn = 0; tcn < TEX_COORDS_NUM_COMPONENTS; tcn++) {
                    tinyobj::real_t tex_coord = attrib.texcoords[
                            TEX_COORDS_NUM_COMPONENTS * idx.texcoord_index + tcn];
                    body_vertices[vertices_count] = flip_texcoord ? 1 - tex_coord : tex_coord;
                    vertices_count++;
                }
            }
        }
        index_offset += fv;
    }
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++) {
        body_indices[i] = (jshort) i;
    }
    ALOG("Final data:\nvertices size -> %1zu\nindices size -> %2zu", vertices_count,
         shapes[0].mesh.indices.size());

    // data
    env->SetFloatArrayRegion(vertices_array, 0, env->GetArrayLength(vertices_array), body_vertices);
    env->SetIntArrayRegion(indices_array, 0, env->GetArrayLength(indices_array), body_indices);
    env->SetObjectField(j_mesh_model, j_vertices_field_id, vertices_array);
    env->SetObjectField(j_mesh_model, j_indices_field_id, indices_array);
    // data size
    env->SetIntField(j_mesh_model, j_vertices_size_field_id,
                     (jint) (attrib.vertices.size()));
    env->SetIntField(j_mesh_model, j_normals_size_field_id,
                     (jint) (attrib.normals.size()));
    env->SetIntField(j_mesh_model, j_texture_coordinates_size_field_id,
                     (jint) (attrib.texcoords.size()));
    env->SetIntField(j_mesh_model, j_colors_size_field_id,
                     (jint) (attrib.colors.size()));
    // releasing arrays
    env->ReleaseFloatArrayElements(vertices_array, body_vertices, JNI_ABORT);
    env->ReleaseIntArrayElements(indices_array, body_indices, JNI_ABORT);
    clock_t endTime = clock();
    ALOG("Time to parsing -> %ld ms", (endTime - startTime) / CLOCKS_PER_MS);
    ALOG("****************************************************************");

    return j_mesh_model;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_riseapps_marusyaobjloader_MarusyaObjLoaderImpl_loadGeometryData(JNIEnv *env,
                                                                         jobject instance,
                                                                         jstring obj_path,
                                                                         jboolean normalize_vertices,
                                                                         jboolean flip_texcoord) {
    clock_t startTime = clock();
    ALOG("****************************************************************");
    ALOG("Loading the geometry data...");
    std::string inputfile = env->GetStringUTFChars(obj_path, false);
    ALOG("Start parsing -> %s", inputfile.c_str());

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
    if (!err.empty() && !ret) {
        ALOG("Error parsing -> %s", err.c_str());
        jclass j_geometry_data_model_class = env->FindClass(
                "com/riseapps/marusyaobjloader/model/GeometryDataModel");
        jmethodID j_geometry_data_model_constructor = env->GetMethodID(j_geometry_data_model_class,
                                                                       "<init>", "()V");
        return env->NewObject(j_geometry_data_model_class, j_geometry_data_model_constructor);
    }
    ALOG("End parsing -> %s", inputfile.c_str());

    // print data
    ALOG("shapes size -> %lu", shapes.size());
    ALOG("material size -> %lu", materials.size());
    ALOG("vertices size -> %lu", attrib.vertices.size());
    ALOG("normals size -> %lu", attrib.normals.size());
    ALOG("texcoords size -> %lu", attrib.texcoords.size());
    ALOG("colors size -> %lu", attrib.colors.size());
    ALOG("mesh name -> %s", shapes[0].name.c_str());
    ALOG("indices size -> %lu", shapes[0].mesh.indices.size());

    jclass j_geometry_data_model_class = env->FindClass(
            "com/riseapps/marusyaobjloader/model/GeometryDataModel");
    jmethodID j_geometry_data_model_constructor = env->GetMethodID(j_geometry_data_model_class,
                                                                   "<init>", "()V");
    jobject j_geometry_data_model = env->NewObject(j_geometry_data_model_class,
                                                   j_geometry_data_model_constructor);

    jfieldID j_vertices_field_id = env->GetFieldID(j_geometry_data_model_class, "vertices", "[F");
    jfieldID j_normals_field_id = env->GetFieldID(j_geometry_data_model_class, "normals", "[F");
    jfieldID j_texture_coordinates_field_id = env->GetFieldID(j_geometry_data_model_class,
                                                              "textureCoordinates", "[F");
    jfieldID j_colors_field_id = env->GetFieldID(j_geometry_data_model_class, "colors", "[F");
    jfieldID j_vertices_indices_field_id = env->GetFieldID(j_geometry_data_model_class,
                                                           "verticesIndices", "[I");
    jfieldID j_normals_indices_field_id = env->GetFieldID(j_geometry_data_model_class,
                                                          "normalsIndices", "[I");
    jfieldID j_texture_coordinates_indices_field_id = env->GetFieldID(j_geometry_data_model_class,
                                                                      "textureCoordinatesIndices",
                                                                      "[I");

    // vertices
    jfloatArray vertices_array = env->NewFloatArray((jsize) attrib.vertices.size());
    jboolean is_copy_vertices;
    jfloat *body_vertices = env->GetFloatArrayElements(vertices_array, &is_copy_vertices);
    for (size_t i = 0; i < attrib.vertices.size(); ++i) {
        body_vertices[i] = normalize_vertices ? attrib.vertices[i] / NORM_COEFFICIENT
                                              : attrib.vertices[i];
    }
    env->SetFloatArrayRegion(vertices_array, 0, env->GetArrayLength(vertices_array), body_vertices);
    env->SetObjectField(j_geometry_data_model, j_vertices_field_id, vertices_array);
    env->ReleaseFloatArrayElements(vertices_array, body_vertices, JNI_ABORT);

    // normals
    jfloatArray normals_array = env->NewFloatArray((jsize) attrib.normals.size());
    jboolean is_copy_normals;
    jfloat *body_normals = env->GetFloatArrayElements(normals_array, &is_copy_normals);
    for (size_t i = 0; i < attrib.normals.size(); ++i) {
        body_normals[i] = attrib.normals[i];
    }
    env->SetFloatArrayRegion(normals_array, 0, env->GetArrayLength(normals_array), body_normals);
    env->SetObjectField(j_geometry_data_model, j_normals_field_id, normals_array);
    env->ReleaseFloatArrayElements(normals_array, body_normals, JNI_ABORT);

    // texture coordinates
    jfloatArray texture_coordinates_array = env->NewFloatArray((jsize) attrib.texcoords.size());
    jboolean is_copy_texture_coordinates;
    jfloat *body_texture_coordinates = env->GetFloatArrayElements(texture_coordinates_array,
                                                                  &is_copy_texture_coordinates);
    for (size_t i = 0; i < attrib.texcoords.size(); ++i) {
        body_texture_coordinates[i] = flip_texcoord ? 1 - attrib.texcoords[i] : attrib.texcoords[i];
    }
    env->SetFloatArrayRegion(texture_coordinates_array, 0,
                             env->GetArrayLength(texture_coordinates_array),
                             body_texture_coordinates);
    env->SetObjectField(j_geometry_data_model, j_texture_coordinates_field_id,
                        texture_coordinates_array);
    env->ReleaseFloatArrayElements(texture_coordinates_array, body_texture_coordinates, JNI_ABORT);

    // colors
    jfloatArray colors_array = env->NewFloatArray((jsize) attrib.colors.size());
    jboolean is_copy_colors;
    jfloat *body_colors = env->GetFloatArrayElements(colors_array, &is_copy_colors);
    for (size_t i = 0; i < attrib.colors.size(); ++i) {
        body_colors[i] = attrib.colors[i];
    }
    env->SetFloatArrayRegion(colors_array, 0, env->GetArrayLength(colors_array), body_colors);
    env->SetObjectField(j_geometry_data_model, j_colors_field_id, colors_array);
    env->ReleaseFloatArrayElements(colors_array, body_colors, JNI_ABORT);

    // vertices indices
    jintArray vertices_indices_array = env->NewIntArray((jsize) shapes[0].mesh.indices.size());
    jboolean is_copy_vertices_indices;
    jint *body_vertices_indices = env->GetIntArrayElements(vertices_indices_array,
                                                           &is_copy_vertices_indices);
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); ++i) {
        body_vertices_indices[i] = shapes[0].mesh.indices[i].vertex_index;
    }
    env->SetIntArrayRegion(vertices_indices_array, 0, env->GetArrayLength(vertices_indices_array),
                           body_vertices_indices);
    env->SetObjectField(j_geometry_data_model, j_vertices_indices_field_id, vertices_indices_array);
    env->ReleaseIntArrayElements(vertices_indices_array, body_vertices_indices, JNI_ABORT);

    // normals indices
    jintArray normals_indices_array = env->NewIntArray((jsize) shapes[0].mesh.indices.size());
    jboolean is_copy_normals_indices;
    jint *body_normals_indices = env->GetIntArrayElements(normals_indices_array,
                                                          &is_copy_normals_indices);
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); ++i) {
        body_normals_indices[i] = shapes[0].mesh.indices[i].normal_index;
    }
    env->SetIntArrayRegion(normals_indices_array, 0, env->GetArrayLength(normals_indices_array),
                           body_normals_indices);
    env->SetObjectField(j_geometry_data_model, j_normals_indices_field_id, normals_indices_array);
    env->ReleaseIntArrayElements(normals_indices_array, body_normals_indices, JNI_ABORT);

    // texture coordinates indices
    jintArray texture_coordinates_indices_array = env->NewIntArray(
            (jsize) shapes[0].mesh.indices.size());
    jboolean is_copy_texture_coordinates_indices;
    jint *body_texture_coordinates_indices = env->GetIntArrayElements(
            texture_coordinates_indices_array, &is_copy_texture_coordinates_indices);
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); ++i) {
        body_texture_coordinates_indices[i] = shapes[0].mesh.indices[i].texcoord_index;
    }
    env->SetIntArrayRegion(texture_coordinates_indices_array, 0,
                           env->GetArrayLength(texture_coordinates_indices_array),
                           body_texture_coordinates_indices);
    env->SetObjectField(j_geometry_data_model, j_texture_coordinates_indices_field_id,
                        texture_coordinates_indices_array);
    env->ReleaseIntArrayElements(texture_coordinates_indices_array,
                                 body_texture_coordinates_indices, JNI_ABORT);

    clock_t endTime = clock();
    ALOG("Time to parsing -> %ld ms", (endTime - startTime) / CLOCKS_PER_MS);
    ALOG("****************************************************************");

    return j_geometry_data_model;
}