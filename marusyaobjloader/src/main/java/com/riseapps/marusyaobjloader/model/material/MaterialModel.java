package com.riseapps.marusyaobjloader.model.material;

import java.util.Arrays;
import java.util.HashMap;

public class MaterialModel {
    private String name;
    private float[] ambient;
    private float[] diffuse;
    private float[] specular;
    private float[] transmittance;
    private float[] emission;
    private float shininess;
    private float ior;
    private float dissolve;
    private int illum;
    private int dummy;
    private String ambientTexname;
    private String diffuseTexname;
    private String specularTexname;
    private String specularHighlightTexname;
    private String bumpTexname;
    private String displacementTexname;
    private String alphaTexname;
    private String reflectionTexname;
    private TextureOptionModel ambientTexopt;
    private TextureOptionModel diffuseTexopt;
    private TextureOptionModel specularTexopt;
    private TextureOptionModel specularHighlightTexopt;
    private TextureOptionModel bumpTexopt;
    private TextureOptionModel displacementTexopt;
    private TextureOptionModel alphaTexopt;
    private TextureOptionModel reflectionTexopt;
    private float roughness;
    private float metallic;
    private float sheen;
    private float clearcoatThickness;
    private float clearcoatRoughness;
    private float anisotropy;
    private float anisotropyRotation;
    private float pad0;
    private String roughnessTexname;
    private String metallicTexname;
    private String sheenTexname;
    private String emissiveTexname;
    private String normalTexname;
    private TextureOptionModel roughnessTexopt;
    private TextureOptionModel metallicTexopt;
    private TextureOptionModel sheenTexopt;
    private TextureOptionModel emissiveTexopt;
    private TextureOptionModel normalTexopt;
    private int pad2;
    private HashMap<String, String> unknownParameter;

    public MaterialModel() {
    }

    public String getName() {
        return name;
    }

    public float[] getAmbient() {
        return ambient;
    }

    public float[] getDiffuse() {
        return diffuse;
    }

    public float[] getSpecular() {
        return specular;
    }

    public float[] getTransmittance() {
        return transmittance;
    }

    public float[] getEmission() {
        return emission;
    }

    public float getShininess() {
        return shininess;
    }

    public float getIor() {
        return ior;
    }

    public float getDissolve() {
        return dissolve;
    }

    public int getIllum() {
        return illum;
    }

    public int getDummy() {
        return dummy;
    }

    public String getAmbientTexname() {
        return ambientTexname;
    }

    public String getDiffuseTexname() {
        return diffuseTexname;
    }

    public String getSpecularTexname() {
        return specularTexname;
    }

    public String getSpecularHighlightTexname() {
        return specularHighlightTexname;
    }

    public String getBumpTexname() {
        return bumpTexname;
    }

    public String getDisplacementTexname() {
        return displacementTexname;
    }

    public String getAlphaTexname() {
        return alphaTexname;
    }

    public String getReflectionTexname() {
        return reflectionTexname;
    }

    public TextureOptionModel getAmbientTexopt() {
        return ambientTexopt;
    }

    public TextureOptionModel getDiffuseTexopt() {
        return diffuseTexopt;
    }

    public TextureOptionModel getSpecularTexopt() {
        return specularTexopt;
    }

    public TextureOptionModel getSpecularHighlightTexopt() {
        return specularHighlightTexopt;
    }

    public TextureOptionModel getBumpTexopt() {
        return bumpTexopt;
    }

    public TextureOptionModel getDisplacementTexopt() {
        return displacementTexopt;
    }

    public TextureOptionModel getAlphaTexopt() {
        return alphaTexopt;
    }

    public TextureOptionModel getReflectionTexopt() {
        return reflectionTexopt;
    }

    public float getRoughness() {
        return roughness;
    }

    public float getMetallic() {
        return metallic;
    }

    public float getSheen() {
        return sheen;
    }

    public float getClearcoatThickness() {
        return clearcoatThickness;
    }

    public float getClearcoatRoughness() {
        return clearcoatRoughness;
    }

    public float getAnisotropy() {
        return anisotropy;
    }

    public float getAnisotropyRotation() {
        return anisotropyRotation;
    }

    public float getPad0() {
        return pad0;
    }

    public String getRoughnessTexname() {
        return roughnessTexname;
    }

    public String getMetallicTexname() {
        return metallicTexname;
    }

    public String getSheenTexname() {
        return sheenTexname;
    }

    public String getEmissiveTexname() {
        return emissiveTexname;
    }

    public String getNormalTexname() {
        return normalTexname;
    }

    public TextureOptionModel getRoughnessTexopt() {
        return roughnessTexopt;
    }

    public TextureOptionModel getMetallicTexopt() {
        return metallicTexopt;
    }

    public TextureOptionModel getSheenTexopt() {
        return sheenTexopt;
    }

    public TextureOptionModel getEmissiveTexopt() {
        return emissiveTexopt;
    }

    public TextureOptionModel getNormalTexopt() {
        return normalTexopt;
    }

    public int getPad2() {
        return pad2;
    }

    public HashMap<String, String> getUnknownParameter() {
        return unknownParameter;
    }

    @Override
    public String toString() {
        return "MaterialModel{" +
                "name='" + name + '\'' +
                ", ambient=" + Arrays.toString(ambient) +
                ", diffuse=" + Arrays.toString(diffuse) +
                ", specular=" + Arrays.toString(specular) +
                ", transmittance=" + Arrays.toString(transmittance) +
                ", emission=" + Arrays.toString(emission) +
                ", shininess=" + shininess +
                ", ior=" + ior +
                ", dissolve=" + dissolve +
                ", illum=" + illum +
                ", dummy=" + dummy +
                ", ambientTexname='" + ambientTexname + '\'' +
                ", diffuseTexname='" + diffuseTexname + '\'' +
                ", specularTexname='" + specularTexname + '\'' +
                ", specularHighlightTexname='" + specularHighlightTexname + '\'' +
                ", bumpTexname='" + bumpTexname + '\'' +
                ", displacementTexname='" + displacementTexname + '\'' +
                ", alphaTexname='" + alphaTexname + '\'' +
                ", reflectionTexname='" + reflectionTexname + '\'' +
                ", ambientTexopt=" + ambientTexopt.toString() +
                ", diffuseTexopt=" + diffuseTexopt.toString() +
                ", specularTexopt=" + specularTexopt.toString() +
                ", specularHighlightTexopt=" + specularHighlightTexopt.toString() +
                ", bumpTexopt=" + bumpTexopt.toString() +
                ", displacementTexopt=" + displacementTexopt.toString() +
                ", alphaTexopt=" + alphaTexopt.toString() +
                ", reflectionTexopt=" + reflectionTexopt.toString() +
                ", roughness=" + roughness +
                ", metallic=" + metallic +
                ", sheen=" + sheen +
                ", clearcoatThickness=" + clearcoatThickness +
                ", clearcoatRoughness=" + clearcoatRoughness +
                ", anisotropy=" + anisotropy +
                ", anisotropyRotation=" + anisotropyRotation +
                ", pad0=" + pad0 +
                ", roughnessTexname='" + roughnessTexname + '\'' +
                ", metallicTexname='" + metallicTexname + '\'' +
                ", sheenTexname='" + sheenTexname + '\'' +
                ", emissiveTexname='" + emissiveTexname + '\'' +
                ", normalTexname='" + normalTexname + '\'' +
                ", roughnessTexopt=" + roughnessTexopt.toString() +
                ", metallicTexopt=" + metallicTexopt.toString() +
                ", sheenTexopt=" + sheenTexopt.toString() +
                ", emissiveTexopt=" + emissiveTexopt.toString() +
                ", normalTexopt=" + normalTexopt.toString() +
                ", pad2=" + pad2 +
                ", unknownParameter=" + unknownParameter.toString() +
                '}';
    }
}