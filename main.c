#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "noise_suppression.h" // 包含 WebRTC 噪声抑制模块的头文件

#define IN_FILE_PATH "AfterCap_8k_mono_16bit.pcm"  // 输入 PCM 音频文件路径
#define OUT_FILE_PATH "output.pcm" // 输出处理后的 PCM 音频文件路径
#define FS 8000  // 采样频率，单位为 Hz
#define FRAME_SIZE 80  // 每帧的样本数，对于 16000 Hz 的采样频率，10 ms 的帧长为 160 样本

int main() {
    // 创建噪声抑制实例
    NsHandle *ns_inst = WebRtcNs_Create();
    if (ns_inst == NULL) {
        fprintf(stderr, "Failed to create noise suppression instance.\n");
        return -1;
    }

    // 初始化噪声抑制实例
    int init_result = WebRtcNs_Init(ns_inst, FS);
    if (init_result != 0) {
        fprintf(stderr, "Failed to initialize noise suppression instance.\n");
        WebRtcNs_Free(ns_inst);
        return -1;
    }

    // 设置噪声抑制策略，这里选择中等模式
    int set_policy_result = WebRtcNs_set_policy(ns_inst, 1);
    if (set_policy_result != 0) {
        fprintf(stderr, "Failed to set noise suppression policy.\n");
        WebRtcNs_Free(ns_inst);
        return -1;
    }

    // 打开输入 PCM 音频文件
    FILE *in_file = fopen(IN_FILE_PATH, "rb");
    if (in_file == NULL) {
        fprintf(stderr, "Failed to open input PCM file: %s\n", IN_FILE_PATH);
        WebRtcNs_Free(ns_inst);
        return -1;
    }

    // 打开输出 PCM 音频文件
    FILE *out_file = fopen(OUT_FILE_PATH, "wb");
    if (out_file == NULL) {
        fprintf(stderr, "Failed to open output PCM file: %s\n", OUT_FILE_PATH);
        fclose(in_file);
        WebRtcNs_Free(ns_inst);
        return -1;
    }

    // 分配内存用于存储一帧音频数据
    int16_t *in_frame = (int16_t *)malloc(FRAME_SIZE * sizeof(int16_t));
    if (in_frame == NULL) {
        fprintf(stderr, "Failed to allocate memory for input frame.\n");
        fclose(in_file);
        fclose(out_file);
        WebRtcNs_Free(ns_inst);
        return -1;
    }

    int16_t *out_frame = (int16_t *)malloc(FRAME_SIZE * sizeof(int16_t));
    if (out_frame == NULL) {
        fprintf(stderr, "Failed to allocate memory for output frame.\n");
        free(in_frame);
        fclose(in_file);
        fclose(out_file);
        WebRtcNs_Free(ns_inst);
        return -1;
    }

    // 逐帧读取输入 PCM 文件，进行噪声抑制处理，并写入输出 PCM 文件
    size_t num_bands = 1; // 对于单声道音频，带数为 1
    while (1) {
        // 读取一帧音频数据
        size_t read_count = fread(in_frame, sizeof(int16_t), FRAME_SIZE, in_file);
        if (read_count < FRAME_SIZE) {
            // 读取到文件末尾或发生错误
            break;
        }

        // 分析当前帧，估计背景噪声
        WebRtcNs_Analyze(ns_inst, in_frame);

        // 对当前帧进行噪声抑制处理
        WebRtcNs_Process(ns_inst, &in_frame, num_bands, &out_frame);

        // 将处理后的帧写入输出 PCM 文件
        fwrite(out_frame, sizeof(int16_t), FRAME_SIZE, out_file);
    }

    // 释放内存
    free(in_frame);
    free(out_frame);

    // 关闭文件
    fclose(in_file);
    fclose(out_file);

    // 释放噪声抑制实例
    WebRtcNs_Free(ns_inst);

    printf("Noise suppression completed. Output file: %s\n", OUT_FILE_PATH);
    return 0;
}
