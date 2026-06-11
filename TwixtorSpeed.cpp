#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "AE_EffectUI.h"

#define PLUGIN_VERSION 1

enum {
    TWIX_INPUT = 0,
    TWIX_SPEED_PERCENT,
    TWIX_SPEED_MODE,
    TWIX_INTERPOLATION,
    TWIX_QUALITY,
    TWIX_NUM_PARAMS
};

// ============================================
// GLOBAL SETUP
// ============================================

PF_Err GlobalSetup(
    PF_InData *in_data,
    PF_OutData *out_data,
    PF_ParamDef *params[],
    PF_LayerDef *output)
{
    out_data->my_version = PF_VERSION(PLUGIN_VERSION, 0, 0, 0);
    out_data->out_flags = PF_OutFlag_DEEP_COLOR_AWARE;
    out_data->out_flags2 = PF_OutFlag2_SUPPORTS_SMART_RENDER;
    
    return PF_Err_NONE;
}

// ============================================
// GLOBAL SETDOWN
// ============================================

PF_Err GlobalSetdown(
    PF_InData *in_data)
{
    return PF_Err_NONE;
}

// ============================================
// PARAM SETUP
// ============================================

PF_Err ParamSetup(
    PF_InData *in_data,
    PF_OutData *out_data,
    PF_ParamDef *params[],
    PF_LayerDef *output)
{
    PF_ParamDef def;
    
    // ========== SPEED SLIDER ==========
    AEFX_CLR_STRUCT(def);
    def.param_type = PF_Param_SLIDER_STEREO;
    PF_STRCPY(def.name, "Speed %");
    def.u.sd.value = 100;
    def.u.sd.min_value = 25;
    def.u.sd.max_value = 400;
    def.u.sd.dontDisplayUnitText = TRUE;
    PF_ADD_PARAM_TO_STREAM(in_data, out_data, &def);
    
    // ========== SPEED MODE PRESETS ==========
    AEFX_CLR_STRUCT(def);
    def.param_type = PF_Param_POPUP;
    PF_STRCPY(def.name, "Speed Preset");
    def.u.pd.value = 3;
    def.u.pd.num_options = 7;
    
    PF_STRCPY(def.u.pd.u.namesptr[0], "0.25x (4x Slow)");
    PF_STRCPY(def.u.pd.u.namesptr[1], "0.5x (2x Slow)");
    PF_STRCPY(def.u.pd.u.namesptr[2], "0.75x");
    PF_STRCPY(def.u.pd.u.namesptr[3], "1.0x (Normal)");
    PF_STRCPY(def.u.pd.u.namesptr[4], "1.5x");
    PF_STRCPY(def.u.pd.u.namesptr[5], "2.0x (2x Fast)");
    PF_STRCPY(def.u.pd.u.namesptr[6], "4.0x (4x Fast)");
    
    PF_ADD_PARAM_TO_STREAM(in_data, out_data, &def);
    
    // ========== FRAME INTERPOLATION ==========
    AEFX_CLR_STRUCT(def);
    def.param_type = PF_Param_CHECKBOX;
    PF_STRCPY(def.name, "Frame Interpolation");
    def.u.bd.value = TRUE;
    PF_ADD_PARAM_TO_STREAM(in_data, out_data, &def);
    
    // ========== QUALITY ==========
    AEFX_CLR_STRUCT(def);
    def.param_type = PF_Param_POPUP;
    PF_STRCPY(def.name, "Quality");
    def.u.pd.value = 2;
    def.u.pd.num_options = 3;
    
    PF_STRCPY(def.u.pd.u.namesptr[0], "Low (Fast)");
    PF_STRCPY(def.u.pd.u.namesptr[1], "Medium (Balanced)");
    PF_STRCPY(def.u.pd.u.namesptr[2], "High (Best)");
    
    PF_ADD_PARAM_TO_STREAM(in_data, out_data, &def);
    
    out_data->num_params = TWIX_NUM_PARAMS;
    
    return PF_Err_NONE;
}

// ============================================
// SEQUENCE SETUP
// ============================================

PF_Err SequenceSetup(
    PF_InData *in_data,
    PF_OutData *out_data,
    PF_ParamDef *params[],
    PF_LayerDef *output)
{
    return PF_Err_NONE;
}

// ============================================
// SEQUENCE SETDOWN
// ============================================

PF_Err SequenceSetdown(
    PF_InData *in_data)
{
    return PF_Err_NONE;
}

// ============================================
// MAIN RENDER FUNCTION
// ============================================

PF_Err Render(
    PF_InData *in_data,
    PF_OutData *out_data,
    PF_ParamDef *params[],
    PF_LayerDef *output)
{
    PF_Err err = PF_Err_NONE;
    
    float speed_percent = params[TWIX_SPEED_PERCENT]->u.sd.value;
    A_long speed_mode = params[TWIX_SPEED_MODE]->u.pd.value;
    A_Boolean use_interpolation = params[TWIX_INTERPOLATION]->u.bd.value;
    A_long quality = params[TWIX_QUALITY]->u.pd.value;
    
    if(speed_mode != 3) {
        float preset_speeds[] = {25, 50, 75, 100, 150, 200, 400};
        speed_percent = preset_speeds[speed_mode];
    }
    
    float speed_multiplier = speed_percent / 100.0f;
    A_long current_frame = in_data->current_time;
    float source_frame_float = (float)current_frame / speed_multiplier;
    
    A_long frame_idx = (A_long)floor(source_frame_float);
    float blend_factor = source_frame_float - frame_idx;
    
    if(!use_interpolation) {
        blend_factor = 0.0f;
    }
    
    PF_EffectWorld *src = &output->input_world_index;
    PF_EffectWorld *dst = &output->dest;
    
    A_long width = src->width;
    A_long height = src->height;
    A_long src_row_bytes = src->row_bytes;
    A_long dst_row_bytes = dst->row_bytes;
    
    PF_Pixel *src_data = (PF_Pixel *)src->data;
    PF_Pixel *dst_data = (PF_Pixel *)dst->data;
    
    if(quality == 0) {
        for(A_long y = 0; y < height; y++) {
            for(A_long x = 0; x < width; x++) {
                A_long idx_src = y * (src_row_bytes / 4) + x;
                A_long idx_dst = y * (dst_row_bytes / 4) + x;
                dst_data[idx_dst] = src_data[idx_src];
            }
        }
    } 
    else if(quality == 1) {
        for(A_long y = 0; y < height; y++) {
            for(A_long x = 0; x < width; x++) {
                A_long idx_src = y * (src_row_bytes / 4) + x;
                A_long idx_dst = y * (dst_row_bytes / 4) + x;
                
                PF_Pixel pixel = src_data[idx_src];
                
                if(use_interpolation && blend_factor > 0.0f) {
                    pixel.alpha = (A_u_char)(pixel.alpha * (1.0f - blend_factor * 0.1f));
                }
                
                dst_data[idx_dst] = pixel;
            }
        }
    }
    else {
        for(A_long y = 0; y < height; y++) {
            for(A_long x = 0; x < width; x++) {
                A_long idx_src = y * (src_row_bytes / 4) + x;
                A_long idx_dst = y * (dst_row_bytes / 4) + x;
                
                PF_Pixel pixel = src_data[idx_src];
                
                if(use_interpolation && blend_factor > 0.0f) {
                    float blend = 1.0f - (blend_factor * 0.05f);
                    pixel.alpha = (A_u_char)(pixel.alpha * blend);
                }
                
                dst_data[idx_dst] = pixel;
            }
        }
    }
    
    return err;
}

// ============================================
// SMART RENDER
// ============================================

PF_Err SmartRender(
    PF_InData *in_data,
    PF_OutData *out_data,
    PF_SmartRenderExtra *extra)
{
    return Render(in_data, out_data, 
                  (PF_ParamDef **)in_data->params, 
                  &extra->output);
}

// ============================================
// MAIN ENTRY POINT
// ============================================

extern "C" {

PF_Err main(
    PF_Cmd cmd,
    PF_InData *in_data,
    PF_OutData *out_data,
    PF_ParamDef *params[],
    PF_LayerDef *output,
    void *extra)
{
    PF_Err err = PF_Err_NONE;

    switch(cmd) {
        case PF_Cmd_GLOBAL_SETUP:
            err = GlobalSetup(in_data, out_data, params, output);
            break;

        case PF_Cmd_GLOBAL_SETDOWN:
            err = GlobalSetdown(in_data);
            break;

        case PF_Cmd_PARAMS_SETUP:
            err = ParamSetup(in_data, out_data, params, output);
            break;

        case PF_Cmd_SEQUENCE_SETUP:
            err = SequenceSetup(in_data, out_data, params, output);
            break;

        case PF_Cmd_SEQUENCE_SETDOWN:
            err = SequenceSetdown(in_data);
            break;

        case PF_Cmd_RENDER:
            err = Render(in_data, out_data, params, output);
            break;

        case PF_Cmd_SMART_RENDER:
            err = SmartRender(in_data, out_data, 
                            (PF_SmartRenderExtra *)extra);
            break;

        default:
            break;
    }

    return err;
}

}
