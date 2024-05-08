/**
 * @file global_var.h
 *
 * @brief global variables for integrating audio/video software into \n
 *        the entire project, the variables are used for global access
 */

#if AvEnableCecFeature /* CEC Related */
extern uchar  DevicePowerStatus;
extern char   DeviceName[20];
#if 0//(GSV2K8||AvMatrix4x4Mode)
extern uchar  AudioStatus[AvGsv2k8DeviceNumber][AudioNumber];
extern CEC_AUDIO_STATUS Cec_Tx_Audio_Status[AvGsv2k8DeviceNumber][AudioNumber];
#elif 0//GSV2K6
extern uchar  AudioStatus[AvGsv2k6DeviceNumber][AudioNumber];
extern CEC_AUDIO_STATUS Cec_Tx_Audio_Status[AvGsv2k6DeviceNumber][AudioNumber];
#elif 0//GSV2K2
extern uchar  AudioStatus[AvGsv2k2DeviceNumber][AudioNumber];
extern CEC_AUDIO_STATUS Cec_Tx_Audio_Status[AvGsv2k2DeviceNumber][AudioNumber];
#else
extern uchar  AudioStatus;
extern CEC_AUDIO_STATUS Cec_Tx_Audio_Status;
#endif
#endif
