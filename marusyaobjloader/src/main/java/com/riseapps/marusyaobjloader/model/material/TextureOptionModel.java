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
    private String imfchan;
    private boolean blendU;
    private boolean blendV;
    private float bumpMultiplier;

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

    public String getImfchan() {
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
                ", imfchan=" + imfchan +
                ", blendU=" + blendU +
                ", blendV=" + blendV +
                ", bumpMultiplier=" + bumpMultiplier +
                '}';
    }
}