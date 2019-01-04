package com.riseapps.marusyaobjloader.model.material;

import java.util.Arrays;

public class TextureOptionModel {
    private TextureType type;
    private float sharpness;
    private float brightness;
    private float contrast;
    private float[] originOffset;
    private float[] scale;
    private float[] turbulence;
    private boolean clamp;
    private char imfchan;
    private boolean blendU;
    private boolean blendV;
    private float bumpMultiplier;
    private String colorspace;

    public TextureOptionModel() {
    }

    public TextureType getType() {
        return type;
    }

    public float getSharpness() {
        return sharpness;
    }

    public float getBrightness() {
        return brightness;
    }

    public float getContrast() {
        return contrast;
    }

    public float[] getOriginOffset() {
        return originOffset;
    }

    public float[] getScale() {
        return scale;
    }

    public float[] getTurbulence() {
        return turbulence;
    }

    public boolean isClamp() {
        return clamp;
    }

    public char getImfchan() {
        return imfchan;
    }

    public boolean isBlendU() {
        return blendU;
    }

    public boolean isBlendV() {
        return blendV;
    }

    public float getBumpMultiplier() {
        return bumpMultiplier;
    }

    public String getColorspace() {
        return colorspace;
    }

    @Override
    public String toString() {
        return "TextureOptionModel{" +
                "type=" + type +
                ", sharpness=" + sharpness +
                ", brightness=" + brightness +
                ", contrast=" + contrast +
                ", originOffset=" + Arrays.toString(originOffset) +
                ", scale=" + Arrays.toString(scale) +
                ", turbulence=" + Arrays.toString(turbulence) +
                ", clamp=" + clamp +
                ", imfchan='" + imfchan + '\'' +
                ", blendU=" + blendU +
                ", blendV=" + blendV +
                ", bumpMultiplier=" + bumpMultiplier +
                ", colorspace='" + colorspace + '\'' +
                '}';
    }
}