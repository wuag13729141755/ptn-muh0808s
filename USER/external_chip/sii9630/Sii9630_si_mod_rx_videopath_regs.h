/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed “AS IS” WITHOUT ANY WARRANTY of any kind,
* whether express or implied, including without limitation the implied warranty
* of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE or NON-INFRINGEMENT.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
* You may contact Lattice Semiconductor Corporation at 111 SW 5th Ave,
* Suite 700, Portland, OR 97204
*/

#ifndef _SHERMAN_VID_H_
#define _SHERMAN_VID_H_

#define BASE_ADDRESS 0x0000

enum page_t
{
    video_path_core_registers = (BASE_ADDRESS | 0x1A00),
    video_path_core_registers__cms_core_registers = (BASE_ADDRESS | 0x1A00),
    video_path_core_registers__cms_core_registers__csc_core_registers0 = (BASE_ADDRESS | 0x1A00),
    video_path_core_registers__cms_core_registers__csc_core_registers1 = (BASE_ADDRESS | 0x1A80),
};

struct page_info{
    enum page_t page_num;
    uint8_t dev_id;
};

//***************************************************************************
// video_path_core_registers. address: A0
// module features
#define REG_ADDR__VP__FEATURES                                           (video_path_core_registers | 0x0000)
  // (read_only, bits 0)
  // video path core module
  #define BIT_MSK__VP__FEATURES__VIDEO_PATH_CORE                                       0x01
  // (read_only, bits 1)
  // input pin muxing
  #define BIT_MSK__VP__FEATURES__VI_MUXING                                             0x02
  // (read_only, bits 2)
  // input format detector
  #define BIT_MSK__VP__FEATURES__VI_FDET                                               0x04
  // (read_only, bits 3)
  // input sync polarity adjustment
  #define BIT_MSK__VP__FEATURES__VI_SYNC_ADJUST                                        0x08
  // (read_only, bits 4)
  // input rate conversion
  #define BIT_MSK__VP__FEATURES__VI_RATECONV                                           0x10
  // (read_only, bits 5)
  // retiming VTG/FIFO
  #define BIT_MSK__VP__FEATURES__VI_RETIMING                                           0x20
  // (read_only, bits 6)
  // 656 decoder
  #define BIT_MSK__VP__FEATURES__DEC656                                                0x40
  // (read_only, bits 7)
  // DE generator
  #define BIT_MSK__VP__FEATURES__DEGEN                                                 0x80
  // (read_only, bits 8)
  // pixel capture at the input
  #define BIT_MSK__VP__FEATURES__PIXCAP_IN                                             0x100
  // (read_only, bits 9)
  // chroma vertical upsampler
  #define BIT_MSK__VP__FEATURES__C420_C422                                             0x200
  // (read_only, bits 10)
  // chroma horizontal upsampler
  #define BIT_MSK__VP__FEATURES__C422_C444                                             0x400
  // (read_only, bits 11)
  // multi-_colorspace converter #0
  #define BIT_MSK__VP__FEATURES__MULTI_CSC0                                            0x800
  // (read_only, bits 12)
  // pixel capture before CMS
  #define BIT_MSK__VP__FEATURES__PIXCAP_PRE                                            0x1000
  // (read_only, bits 13)
  // PWLI #0
  #define BIT_MSK__VP__FEATURES__PWLI0                                                 0x2000
  // (read_only, bits 14)
  // CMS matrix
  #define BIT_MSK__VP__FEATURES__CMS_MATRIX                                            0x4000
  // (read_only, bits 15)
  // PWLI #1
  #define BIT_MSK__VP__FEATURES__PWLI1                                                 0x8000
  // (read_only, bits 16)
  // PWLI #2
  #define BIT_MSK__VP__FEATURES__PWLI2                                                 0x10000
  // (read_only, bits 17)
  // pixel capture after CMS
  #define BIT_MSK__VP__FEATURES__PIXCAP_POST                                           0x20000
  // (read_only, bits 18)
  // multi-_colorspace converter #1
  #define BIT_MSK__VP__FEATURES__MULTI_CSC1                                            0x40000
  // (read_only, bits 19)
  // chroma horizontal subsampler
  #define BIT_MSK__VP__FEATURES__C444_C422                                             0x80000
  // (read_only, bits 20)
  // chroma vertical subsampler
  #define BIT_MSK__VP__FEATURES__C422_C420                                             0x100000
  // (read_only, bits 21)
  // dither/_round
  #define BIT_MSK__VP__FEATURES__DITHER_RND                                            0x200000
  // (read_only, bits 22)
  // range clip
  #define BIT_MSK__VP__FEATURES__RANGE_CLIP                                            0x400000
  // (read_only, bits 23)
  // pixel capture at the output
  #define BIT_MSK__VP__FEATURES__PIXCAP_OUT                                            0x800000
  // (read_only, bits 24)
  // output rate conversion
  #define BIT_MSK__VP__FEATURES__VO_RATECONV                                           0x1000000
  // (read_only, bits 25)
  // output blanking control
  #define BIT_MSK__VP__FEATURES__VO_BLANK                                              0x2000000
  // (read_only, bits 26)
  // output embedded sync encoder
  #define BIT_MSK__VP__FEATURES__VO_EMBD_SYNC_ENC                                      0x4000000
  // (read_only, bits 27)
  // output pin muxing
  #define BIT_MSK__VP__FEATURES__VO_MUXING                                             0x8000000

// device build time stamp
#define REG_ADDR__VP__BUILD_TIME                                         (video_path_core_registers | 0x0008)

// software reset
#define REG_ADDR__VP__SOFT_RESET                                         (video_path_core_registers | 0x000c)
  // (read_write, bits 0)
  // video input software reset for frontend only (clk_in domain)
  #define BIT_MSK__VP__SOFT_RESET__RESET_CLK_IN                                          0x01
  // (read_write, bits 1)
  // video input software reset for core only (clk_core domain)
  #define BIT_MSK__VP__SOFT_RESET__RESET_CLK_CORE                                        0x02
  // (read_write, bits 2)
  // video input software reset for backend only (clk_out domain)
  #define BIT_MSK__VP__SOFT_RESET__RESET_CLK_OUT                                         0x04

// number of data bits for the datapath
#define REG_ADDR__VP__DATA_BITS_VALUE                                    (video_path_core_registers | 0x000e)

// input muting
#define REG_ADDR__VP__INPUT_MUTE                                         (video_path_core_registers | 0x0010)
  // (read_write, bits 0)
  // disable vsync input
  #define BIT_MSK__VP__INPUT_MUTE__VSYNC_DISABLE                                         0x01
  // (read_write, bits 1)
  // disable hsync input
  #define BIT_MSK__VP__INPUT_MUTE__HSYNC_DISABLE                                         0x02
  // (read_write, bits 2)
  // disable DE input
  #define BIT_MSK__VP__INPUT_MUTE__DE_DISABLE                                            0x04
  // (read_write, bits 3)
  // disable Y input pins
  #define BIT_MSK__VP__INPUT_MUTE__Y_DISABLE                                             0x08
  // (read_write, bits 4)
  // disable cb input pins
  #define BIT_MSK__VP__INPUT_MUTE__CB_DISABLE                                            0x10
  // (read_write, bits 5)
  // disable cr input pins
  #define BIT_MSK__VP__INPUT_MUTE__CR_DISABLE                                            0x20

// input sync configuration
#define REG_ADDR__VP__INPUT_SYNC_CONFIG                                  (video_path_core_registers | 0x0012)
  // (read_write, bits 0)
  // select VSYNC polarity
  #define BIT_MSK__VP__INPUT_SYNC_CONFIG__VSYNC_POLARITY                                        0x01
  // (read_write, bits 1)
  // select HSYNC polarity
  #define BIT_MSK__VP__INPUT_SYNC_CONFIG__HSYNC_POLARITY                                        0x02
  // (read_write, bits 2)
  // select DE polarity
  #define BIT_MSK__VP__INPUT_SYNC_CONFIG__DE_POLARITY                                           0x04
  // (read_write, bits 3)
  // DE input is FIELD signal (requires DE generator to be enabled!)
  #define BIT_MSK__VP__INPUT_SYNC_CONFIG__DE_IS_FIELD                                           0x08

// input format selection
#define REG_ADDR__VP__INPUT_FORMAT                                       (video_path_core_registers | 0x0014)
  // (read_write, bits 0)
  // strobe data on both posedge and negedge of input clock
  #define BIT_MSK__VP__INPUT_FORMAT__DDR_ENABLE                                            0x01
  // (read_write, bits 1)
  // select polarity of DDR decoder
  #define BIT_MSK__VP__INPUT_FORMAT__DDR_POLARITY                                          0x02
  // (read_write, bits 4:2)
  // select DDR mode
  #define BIT_MSK__VP__INPUT_FORMAT__DDR_MODE                                              0x1C
  // (read_write, bits 5)
  // enable Y/C demux logic
  #define BIT_MSK__VP__INPUT_FORMAT__YC_DEMUX_ENABLE                                       0x20
  // (read_write, bits 6)
  // select polarity of Y/C demux logic
  #define BIT_MSK__VP__INPUT_FORMAT__YC_DEMUX_POLARITY                                     0x40
  // (read_write, bits 7)
  // select cb/_cr order for muxed signals
  #define BIT_MSK__VP__INPUT_FORMAT__CBCR_ORDER                                            0x80
  // (read_write, bits 9:8)
  // remove replicated pixels
  #define BIT_MSK__VP__INPUT_FORMAT__PIXEL_RATE                                            0x300
  // (read_write, bits 10)
  // enable 4:2:0 Y mux
  #define BIT_MSK__VP__INPUT_FORMAT__MUX_420_ENABLE                                        0x400
  // (read_write, bits 11)
  // select whether cb or cr bus are used as chroma input
  #define BIT_MSK__VP__INPUT_FORMAT__MUX_CB_OR_CR                                          0x800

// input pin mapping configuration
#define REG_ADDR__VP__INPUT_MAPPING                                      (video_path_core_registers | 0x0016)
  // (read_write, bits 0)
  // reverse input pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__INPUT_MAPPING__REVERSE_Y                                             0x01
  // (read_write, bits 1)
  // reverse input pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__INPUT_MAPPING__REVERSE_CB                                            0x02
  // (read_write, bits 2)
  // reverse input pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__INPUT_MAPPING__REVERSE_CR                                            0x04
  // (read_write, bits 5:3)
  // select input pins for internal Y datapath
  #define BIT_MSK__VP__INPUT_MAPPING__SELECT_Y                                              0x38
  // (read_write, bits 8:6)
  // select input pins for internal cb datapath
  #define BIT_MSK__VP__INPUT_MAPPING__SELECT_CB                                             0x1C0
  // (read_write, bits 11:9)
  // select input pins for internal cr datapath
  #define BIT_MSK__VP__INPUT_MAPPING__SELECT_CR                                             0xE00

// input mask
#define REG_ADDR__VP__INPUT_MASK                                         (video_path_core_registers | 0x0018)
  // (read_write, bits 1:0)
  // disable l_s_bs on Y datapath
  #define BIT_MSK__VP__INPUT_MASK__DISABLE_LSBS_Y                                        0x03
  // (read_write, bits 3:2)
  // disable l_s_bs on cb datapath
  #define BIT_MSK__VP__INPUT_MASK__DISABLE_LSBS_CB                                       0x0C
  // (read_write, bits 5:4)
  // disable l_s_bs on cr datapath
  #define BIT_MSK__VP__INPUT_MASK__DISABLE_LSBS_CR                                       0x30

// configure sync polarity adjustment for datapath #0
#define REG_ADDR__VP__INPUT_SYNC_ADJUST_CONFIG                           (video_path_core_registers | 0x001c)
  // (read_write, bits 0)
  // configure sync polarity adjustment for datapath #0
  #define BIT_MSK__VP__INPUT_SYNC_ADJUST_CONFIG__AUTO_DISABLE                                          0x01

// DE generator configuration
#define REG_ADDR__VP__DEGEN_CONFIG                                       (video_path_core_registers | 0x0020)
  // (read_write, bits 0)
  // enable DE generator
  #define BIT_MSK__VP__DEGEN_CONFIG__ENABLE                                                0x01

// number of clocks from start of HSYNC before generated DE
#define REG_ADDR__VP__DEGEN_PIXEL_DELAY                                  (video_path_core_registers | 0x0022)

// count (per line) for DE to be active (0=1 clk period, 1=2 clk periods, etc.)
#define REG_ADDR__VP__DEGEN_PIXEL_COUNT_MINUS_ONE                        (video_path_core_registers | 0x0024)

// number of lines from start of VSYNC before generated DE minus one (0=1 line, 1=2 lines, etc.)
#define REG_ADDR__VP__DEGEN_LINE_DELAY                                   (video_path_core_registers | 0x0026)

// number of lines of generated DE
#define REG_ADDR__VP__DEGEN_LINE_COUNT                                   (video_path_core_registers | 0x0028)

// 656 decoder configuration
#define REG_ADDR__VP__DEC656_CONFIG                                      (video_path_core_registers | 0x0030)
  // (read_write, bits 0)
  // search for embedded 656 syncs for DE, hsync, vsync generation
  #define BIT_MSK__VP__DEC656_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // inverts field ID sense for internal vertical timing reference of 656 input module
  #define BIT_MSK__VP__DEC656_CONFIG__INVERT_FIELD_ID                                       0x02
  // (read_write, bits 2)
  // force Y/C demux phase
  #define BIT_MSK__VP__DEC656_CONFIG__FORCE_YC_PHASE                                        0x04
  // (read_write, bits 3)
  // Y/C demux phase
  #define BIT_MSK__VP__DEC656_CONFIG__YC_PHASE_VALUE                                        0x08
  // (read_write, bits 4)
  // don't generate DE in the first line of every even field
  #define BIT_MSK__VP__DEC656_CONFIG__SKIP_FIRST_EVEN_LINE                                  0x10
  // (read_write, bits 5)
  // don't generate DE in the first line of every odd field
  #define BIT_MSK__VP__DEC656_CONFIG__SKIP_FIRST_ODD_LINE                                   0x20

// for embedded syncs, delay from EAV to generated HSYNC
#define REG_ADDR__VP__DEC656_DELAY_EAV_TO_HSYNC_ACTIVE                   (video_path_core_registers | 0x0032)

// for embedded syncs, pulse width of hsync pulse (minus 1)
#define REG_ADDR__VP__DEC656_PULSE_WIDTH_HSYNC_MINUS_ONE                 (video_path_core_registers | 0x0034)

// number of lines after EAV of last active data line to start VSYNC
#define REG_ADDR__VP__DEC656_VSYNC_FRONT_PORCH                           (video_path_core_registers | 0x0036)

// number of clocks after EAV to delay start of VSYNC pulse for even fields or progressive frames
#define REG_ADDR__VP__DEC656_EAV_TO_VSYNC_DELAY_EVEN                     (video_path_core_registers | 0x0038)

// number of clocks after EAV to delay start of VSYNC pulse for odd fields
#define REG_ADDR__VP__DEC656_EAV_TO_VSYNC_DELAY_ODD                      (video_path_core_registers | 0x003a)

// number of lines of width of VSYNC pulse
#define REG_ADDR__VP__DEC656_PULSE_WIDTH_VSYNC                           (video_path_core_registers | 0x003c)

// 656 decoder status
#define REG_ADDR__VP__DEC656_STATUS                                      (video_path_core_registers | 0x003e)
  // (read_only, bits 0)
  // interlaced signal detected
  #define BIT_MSK__VP__DEC656_STATUS__INTERLACED                                            0x01
  // (read_only, bits 1)
  // 656 signal detected
  #define BIT_MSK__VP__DEC656_STATUS__VIDEO656                                              0x02
  // (read_only, bits 2)
  // Y/C demux phase
  #define BIT_MSK__VP__DEC656_STATUS__YC_PHASE                                              0x04
  // (read_only, bits 5:3)
  // alignment of embedded sync control word
  #define BIT_MSK__VP__DEC656_STATUS__VIDEO656_ALIGNMENT                                    0x38

// output muting
#define REG_ADDR__VP__OUTPUT_MUTE                                        (video_path_core_registers | 0x0040)
  // (read_write, bits 0)
  // disable vsync output
  #define BIT_MSK__VP__OUTPUT_MUTE__VSYNC_DISABLE                                         0x01
  // (read_write, bits 1)
  // disable hsync output
  #define BIT_MSK__VP__OUTPUT_MUTE__HSYNC_DISABLE                                         0x02
  // (read_write, bits 2)
  // disable csync output
  #define BIT_MSK__VP__OUTPUT_MUTE__CSYNC_DISABLE                                         0x04
  // (read_write, bits 3)
  // disable DE output
  #define BIT_MSK__VP__OUTPUT_MUTE__DE_DISABLE                                            0x08
  // (read_write, bits 4)
  // disable Y/G output pins
  #define BIT_MSK__VP__OUTPUT_MUTE__Y_DISABLE                                             0x10
  // (read_write, bits 5)
  // disable cb/_b output pins
  #define BIT_MSK__VP__OUTPUT_MUTE__CB_DISABLE                                            0x20
  // (read_write, bits 6)
  // disable cr/_r output pins
  #define BIT_MSK__VP__OUTPUT_MUTE__CR_DISABLE                                            0x40
  // (read_write, bits 7)
  // tristate video output pins
  #define BIT_MSK__VP__OUTPUT_MUTE__TRISTATE                                              0x80

// output port configuration
#define REG_ADDR__VP__OUTPUT_SYNC_CONFIG                                 (video_path_core_registers | 0x0042)
  // (read_write, bits 0)
  // select VSYNC polarity
  #define BIT_MSK__VP__OUTPUT_SYNC_CONFIG__VSYNC_POLARITY                                        0x01
  // (read_write, bits 1)
  // select HSYNC polarity
  #define BIT_MSK__VP__OUTPUT_SYNC_CONFIG__HSYNC_POLARITY                                        0x02
  // (read_write, bits 2)
  // select CSYNC polarity
  #define BIT_MSK__VP__OUTPUT_SYNC_CONFIG__CSYNC_POLARITY                                        0x04
  // (read_write, bits 3)
  // select DE polarity
  #define BIT_MSK__VP__OUTPUT_SYNC_CONFIG__DE_POLARITY                                           0x08
  // (read_write, bits 4)
  // swap HSYNC and CSYNC output controls
  #define BIT_MSK__VP__OUTPUT_SYNC_CONFIG__SWAP_HS_CS                                            0x10

// output pin mapping configuration
#define REG_ADDR__VP__OUTPUT_MAPPING                                     (video_path_core_registers | 0x0044)
  // (read_write, bits 0)
  // reverse output pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__OUTPUT_MAPPING__REVERSE_Y                                             0x01
  // (read_write, bits 1)
  // reverse output pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__OUTPUT_MAPPING__REVERSE_CB                                            0x02
  // (read_write, bits 2)
  // reverse output pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__OUTPUT_MAPPING__REVERSE_CR                                            0x04
  // (read_write, bits 5:3)
  // select internal datapath for Y output pins
  #define BIT_MSK__VP__OUTPUT_MAPPING__SELECT_Y                                              0x38
  // (read_write, bits 8:6)
  // select internal datapath for cb output pins
  #define BIT_MSK__VP__OUTPUT_MAPPING__SELECT_CB                                             0x1C0
  // (read_write, bits 11:9)
  // select internal datapath for cr output pins
  #define BIT_MSK__VP__OUTPUT_MAPPING__SELECT_CR                                             0xE00

// output masking
#define REG_ADDR__VP__OUTPUT_MASK                                        (video_path_core_registers | 0x0046)
  // (read_write, bits 1:0)
  // disable l_s_bs on Y output pins
  #define BIT_MSK__VP__OUTPUT_MASK__DISABLE_LSBS_Y                                        0x03
  // (read_write, bits 3:2)
  // disable l_s_bs on cb output pins
  #define BIT_MSK__VP__OUTPUT_MASK__DISABLE_LSBS_CB                                       0x0C
  // (read_write, bits 5:4)
  // disable l_s_bs on cr output pins
  #define BIT_MSK__VP__OUTPUT_MASK__DISABLE_LSBS_CR                                       0x30

// output format
#define REG_ADDR__VP__OUTPUT_FORMAT                                      (video_path_core_registers | 0x0048)
  // (read_write, bits 0)
  // send data on both posedge and negedge of output clock
  #define BIT_MSK__VP__OUTPUT_FORMAT__DDR_ENABLE                                            0x01
  // (read_write, bits 1)
  // select polarity of DDR encoder
  #define BIT_MSK__VP__OUTPUT_FORMAT__DDR_POLARITY                                          0x02
  // (read_write, bits 4:2)
  // select DDR mode
  #define BIT_MSK__VP__OUTPUT_FORMAT__DDR_MODE                                              0x1C
  // (read_write, bits 5)
  // enable Y/C mux logic
  #define BIT_MSK__VP__OUTPUT_FORMAT__YC_MUX_ENABLE                                         0x20
  // (read_write, bits 6)
  // select polarity of Y/C mux logic
  #define BIT_MSK__VP__OUTPUT_FORMAT__YC_MUX_POLARITY                                       0x40
  // (read_write, bits 7)
  // select cb/_cr order for muxed signals
  #define BIT_MSK__VP__OUTPUT_FORMAT__CBCR_ORDER                                            0x80
  // (read_write, bits 9:8)
  // remove replicated pixels
  #define BIT_MSK__VP__OUTPUT_FORMAT__PIXEL_RATE                                            0x300
  // (read_write, bits 10)
  // enable 4:2:0 Y demux
  #define BIT_MSK__VP__OUTPUT_FORMAT__DEMUX_420_ENABLE                                      0x400
  // (read_write, bits 11)
  // select whether cb or cr bus are used as chroma output
  #define BIT_MSK__VP__OUTPUT_FORMAT__DEMUX_CB_OR_CR                                        0x800

// configure blanking data
#define REG_ADDR__VP__OUTPUT_BLANK_CONFIG                                (video_path_core_registers | 0x0050)
  // (read_write, bits 0)
  // enable blanking data override
  #define BIT_MSK__VP__OUTPUT_BLANK_CONFIG__ENABLE_BLANKING_OVERRIDE                              0x01
  // (read_write, bits 1)
  // enable active data override
  #define BIT_MSK__VP__OUTPUT_BLANK_CONFIG__ENABLE_ACTIVE_OVERRIDE                                0x02
  // (read_write, bits 3:2)
  // select chroma mode
  #define BIT_MSK__VP__OUTPUT_BLANK_CONFIG__CHROMA_MODE                                           0x0C

// use this value for Y during blanking when enabled
#define REG_ADDR__VP__OUTPUT_BLANK_Y                                     (video_path_core_registers | 0x0052)

// use this value for cb during blanking when enabled
#define REG_ADDR__VP__OUTPUT_BLANK_CB                                    (video_path_core_registers | 0x0054)

// use this value for cr during blanking when enabled
#define REG_ADDR__VP__OUTPUT_BLANK_CR                                    (video_path_core_registers | 0x0056)

// use this value for Y during active DE when enabled
#define REG_ADDR__VP__OUTPUT_ACTIVE_Y                                    (video_path_core_registers | 0x0058)

// use this value for cb during active DE when enabled
#define REG_ADDR__VP__OUTPUT_ACTIVE_CB                                   (video_path_core_registers | 0x005a)

// use this value for cr during active DE when enabled
#define REG_ADDR__VP__OUTPUT_ACTIVE_CR                                   (video_path_core_registers | 0x005c)

// format detector #0 configuration
#define REG_ADDR__VP__FDET_CONFIG                                        (video_path_core_registers | 0x0080)
  // (read_write, bits 0)
  // select which sync polarity to use
  #define BIT_MSK__VP__FDET_CONFIG__SYNC_POLARITY_FORCE                                   0x01
  // (read_write, bits 1)
  // select forced HSYNC polarity
  #define BIT_MSK__VP__FDET_CONFIG__HSYNC_POLARITY                                        0x02
  // (read_write, bits 2)
  // select forced VSYNC polarity
  #define BIT_MSK__VP__FDET_CONFIG__VSYNC_POLARITY                                        0x04
  // (read_write, bits 3)
  // enable 656 mode
  #define BIT_MSK__VP__FDET_CONFIG__ENABLE_656                                            0x08

// format detector status. write any value to clear the status
#define REG_ADDR__VP__FDET_STATUS                                        (video_path_core_registers | 0x0081)
  // (read_only, bits 0)
  // detected HSYNC polarity (requires HSYNC/DE input active)
  #define BIT_MSK__VP__FDET_STATUS__HSYNC_POLARITY                                        0x01
  // (read_only, bits 1)
  // detected VSYNC polarity (requires VSYNC/DE input active)
  #define BIT_MSK__VP__FDET_STATUS__VSYNC_POLARITY                                        0x02
  // (read_only, bits 2)
  // interlaced signal detected
  #define BIT_MSK__VP__FDET_STATUS__INTERLACED                                            0x04
  // (read_only, bits 3)
  // 656 signal detected
  #define BIT_MSK__VP__FDET_STATUS__VIDEO656                                              0x08

// delta threshold. if frame rate counter differs by more than this amount between frames an IRQ is issued.
#define REG_ADDR__VP__FDET_FRAME_RATE_DELTA_THRESHOLD                    (video_path_core_registers | 0x0084)

// frame rate = (1 / fdet_frame_rate) * f_clk_reg. write any value to clear the counter
#define REG_ADDR__VP__FDET_FRAME_RATE                                    (video_path_core_registers | 0x0088)

// pixels per line (requires DE input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_PIXEL_COUNT                                   (video_path_core_registers | 0x008c)

// active video lines per frame (requires DE input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_LINE_COUNT                                    (video_path_core_registers | 0x008e)

// number of HSYNC low cycles (requires HSYNC input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_HSYNC_LOW_COUNT                               (video_path_core_registers | 0x0090)

// number of HSYNC high cycles (requires HSYNC input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_HSYNC_HIGH_COUNT                              (video_path_core_registers | 0x0092)

// number of HFRONT cycles (requires HSYNC/DE inputs active). write any value to clear the counter
#define REG_ADDR__VP__FDET_HFRONT_COUNT                                  (video_path_core_registers | 0x0094)

// number of HBACK cycles (requires HSYNC/DE inputs active). write any value to clear the counter
#define REG_ADDR__VP__FDET_HBACK_COUNT                                   (video_path_core_registers | 0x0096)

// number of VSYNC low cycles in the EVEN field (or progressive frame) (requires VSYNC input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_VSYNC_LOW_COUNT_EVEN                          (video_path_core_registers | 0x0098)

// number of VSYNC high cycles in the EVEN field (or progressive frame) (requires VSYNC input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_VSYNC_HIGH_COUNT_EVEN                         (video_path_core_registers | 0x009a)

// number of VFRONT cycles in the EVEN field (or progressive frame) (requires VSYNC/DE inputs active). write any value to clear the counter
#define REG_ADDR__VP__FDET_VFRONT_COUNT_EVEN                             (video_path_core_registers | 0x009c)

// number of VBACK cycles in the EVEN field (or progressive frame) (requires VSYNC/DE input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_VBACK_COUNT_EVEN                              (video_path_core_registers | 0x009e)

// number of VSYNC low cycles in the ODD field (requires VSYNC input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_VSYNC_LOW_COUNT_ODD                           (video_path_core_registers | 0x00a0)

// number of VSYNC high cycles in the ODD field (requires VSYNC input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_VSYNC_HIGH_COUNT_ODD                          (video_path_core_registers | 0x00a2)

// number of VFRONT cycles in the ODD field (requires VSYNC/DE input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_VFRONT_COUNT_ODD                              (video_path_core_registers | 0x00a4)

// number of VBACK cycles in the ODD field (requires VSYNC/DE input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_VBACK_COUNT_ODD                               (video_path_core_registers | 0x00a6)

// running frame count (requires VSYNC input active). write any value to clear the counter
#define REG_ADDR__VP__FDET_FRAME_COUNT                                   (video_path_core_registers | 0x00a8)

// configure which format changes cause IRQ - each bit enables the corresponding bit in the IRQ status register to cause an interrupt
#define REG_ADDR__VP__FDET_IRQ_MASK                                      (video_path_core_registers | 0x00b0)
  // (read_write, bits 0)
  // HSYNC polarity changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__HSYNC_POLARITY                                        0x01
  // (read_write, bits 1)
  // VSYNC polarity changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_POLARITY                                        0x02
  // (read_write, bits 2)
  // interlaced status changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__INTERLACED                                            0x04
  // (read_write, bits 3)
  // BT.656 status changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__VIDEO656                                              0x08
  // (read_write, bits 4)
  // frame rate changes above threshold
  #define BIT_MSK__VP__FDET_IRQ_MASK__FRAME_RATE                                            0x10
  // (read_write, bits 5)
  // pixel count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__PIXEL_COUNT                                           0x20
  // (read_write, bits 6)
  // line count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__LINE_COUNT                                            0x40
  // (read_write, bits 7)
  // HSYNC low count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__HSYNC_LOW_COUNT                                       0x80
  // (read_write, bits 8)
  // HSYNC high count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__HSYNC_HIGH_COUNT                                      0x100
  // (read_write, bits 9)
  // HFRONT count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__HFRONT_COUNT                                          0x200
  // (read_write, bits 10)
  // HBACK count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__HBACK_COUNT                                           0x400
  // (read_write, bits 11)
  // VSYNC low count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_LOW_COUNT_EVEN                                  0x800
  // (read_write, bits 12)
  // VSYNC hight count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_HIGH_COUNT_EVEN                                 0x1000
  // (read_write, bits 13)
  // VFRONT count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VFRONT_COUNT_EVEN                                     0x2000
  // (read_write, bits 14)
  // VBACK count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VBACK_COUNT_EVEN                                      0x4000
  // (read_write, bits 15)
  // VSYNC low count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_LOW_COUNT_ODD                                   0x8000
  // (read_write, bits 16)
  // VSYNC hight count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_HIGH_COUNT_ODD                                  0x10000
  // (read_write, bits 17)
  // VFRONT count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VFRONT_COUNT_ODD                                      0x20000
  // (read_write, bits 18)
  // VBACK count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VBACK_COUNT_ODD                                       0x40000

// IRQ status register
#define REG_ADDR__VP__FDET_IRQ_STATUS                                    (video_path_core_registers | 0x00b4)
  // (read_write, bits 0)
  // HSYNC polarity changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__HSYNC_POLARITY                                        0x01
  // (read_write, bits 1)
  // VSYNC polarity changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_POLARITY                                        0x02
  // (read_write, bits 2)
  // interlaced status changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__INTERLACED                                            0x04
  // (read_write, bits 3)
  // BT.656 status changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VIDEO656                                              0x08
  // (read_write, bits 4)
  // frame rate changes above threshold
  #define BIT_MSK__VP__FDET_IRQ_STATUS__FRAME_RATE                                            0x10
  // (read_write, bits 5)
  // pixel count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__PIXEL_COUNT                                           0x20
  // (read_write, bits 6)
  // line count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__LINE_COUNT                                            0x40
  // (read_write, bits 7)
  // HSYNC low count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__HSYNC_LOW_COUNT                                       0x80
  // (read_write, bits 8)
  // HSYNC high count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__HSYNC_HIGH_COUNT                                      0x100
  // (read_write, bits 9)
  // HFRONT count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__HFRONT_COUNT                                          0x200
  // (read_write, bits 10)
  // HBACK count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__HBACK_COUNT                                           0x400
  // (read_write, bits 11)
  // VSYNC low count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_LOW_COUNT_EVEN                                  0x800
  // (read_write, bits 12)
  // VSYNC hight count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_HIGH_COUNT_EVEN                                 0x1000
  // (read_write, bits 13)
  // VFRONT count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VFRONT_COUNT_EVEN                                     0x2000
  // (read_write, bits 14)
  // VBACK count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VBACK_COUNT_EVEN                                      0x4000
  // (read_write, bits 15)
  // VSYNC low count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_LOW_COUNT_ODD                                   0x8000
  // (read_write, bits 16)
  // VSYNC hight count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_HIGH_COUNT_ODD                                  0x10000
  // (read_write, bits 17)
  // VFRONT count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VFRONT_COUNT_ODD                                      0x20000
  // (read_write, bits 18)
  // VBACK count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VBACK_COUNT_ODD                                       0x40000

// embedded sync encoder configuration
#define REG_ADDR__VP__EMBD_SYNC_ENC_CONFIG                               (video_path_core_registers | 0x00c0)
  // (read_write, bits 0)
  // enable embedded sync encoder to create embedded sync signals
  #define BIT_MSK__VP__EMBD_SYNC_ENC_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // select polarity for field ID
  #define BIT_MSK__VP__EMBD_SYNC_ENC_CONFIG__FIELD_ID_POLARITY                                     0x02
  // (read_write, bits 2)
  // select color space
  #define BIT_MSK__VP__EMBD_SYNC_ENC_CONFIG__INPUT_IS_RGB                                          0x04

// configuration for section #0
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT0_CONFIG                         (video_path_core_registers | 0x00c4)
  // (read_write, bits 2:0)
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT0_CONFIG__SAV                                                   0x07
  // (read_write, bits 5:3)
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT0_CONFIG__EAV                                                   0x38
  // (read_write, bits 21:6)
  // number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT0_CONFIG__LINES                                                 0x3FFFC0
  // (read_write, bits 22)
  // select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT0_CONFIG__DATA                                                  0x400000
  // (read_write, bits 23)
  // last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT0_CONFIG__LAST                                                  0x800000

// configuration for section #1
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT1_CONFIG                         (video_path_core_registers | 0x00c8)
  // (read_write, bits 2:0)
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT1_CONFIG__SAV                                                   0x07
  // (read_write, bits 5:3)
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT1_CONFIG__EAV                                                   0x38
  // (read_write, bits 21:6)
  // number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT1_CONFIG__LINES                                                 0x3FFFC0
  // (read_write, bits 22)
  // select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT1_CONFIG__DATA                                                  0x400000
  // (read_write, bits 23)
  // last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT1_CONFIG__LAST                                                  0x800000

// configuration for section #2
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT2_CONFIG                         (video_path_core_registers | 0x00cc)
  // (read_write, bits 2:0)
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT2_CONFIG__SAV                                                   0x07
  // (read_write, bits 5:3)
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT2_CONFIG__EAV                                                   0x38
  // (read_write, bits 21:6)
  // number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT2_CONFIG__LINES                                                 0x3FFFC0
  // (read_write, bits 22)
  // select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT2_CONFIG__DATA                                                  0x400000
  // (read_write, bits 23)
  // last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT2_CONFIG__LAST                                                  0x800000

// configuration for section #3
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT3_CONFIG                         (video_path_core_registers | 0x00d0)
  // (read_write, bits 2:0)
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT3_CONFIG__SAV                                                   0x07
  // (read_write, bits 5:3)
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT3_CONFIG__EAV                                                   0x38
  // (read_write, bits 21:6)
  // number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT3_CONFIG__LINES                                                 0x3FFFC0
  // (read_write, bits 22)
  // select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT3_CONFIG__DATA                                                  0x400000
  // (read_write, bits 23)
  // last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT3_CONFIG__LAST                                                  0x800000

// configuration for section #4
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT4_CONFIG                         (video_path_core_registers | 0x00d4)
  // (read_write, bits 2:0)
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT4_CONFIG__SAV                                                   0x07
  // (read_write, bits 5:3)
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT4_CONFIG__EAV                                                   0x38
  // (read_write, bits 21:6)
  // number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT4_CONFIG__LINES                                                 0x3FFFC0
  // (read_write, bits 22)
  // select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT4_CONFIG__DATA                                                  0x400000
  // (read_write, bits 23)
  // last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT4_CONFIG__LAST                                                  0x800000

// configuration for section #5
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT5_CONFIG                         (video_path_core_registers | 0x00d8)
  // (read_write, bits 2:0)
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT5_CONFIG__SAV                                                   0x07
  // (read_write, bits 5:3)
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT5_CONFIG__EAV                                                   0x38
  // (read_write, bits 21:6)
  // number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT5_CONFIG__LINES                                                 0x3FFFC0
  // (read_write, bits 22)
  // select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT5_CONFIG__DATA                                                  0x400000
  // (read_write, bits 23)
  // last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT5_CONFIG__LAST                                                  0x800000

// number of active pixels per line
#define REG_ADDR__VP__EMBD_SYNC_ENC_HORIZONTAL_DATA_PIXELS               (video_path_core_registers | 0x00dc)

// number of blanking pixels per line
#define REG_ADDR__VP__EMBD_SYNC_ENC_HORIZONTAL_BLANKING_PIXELS           (video_path_core_registers | 0x00de)

// bank update request for embedded sync encoder parameters (write any data value - this is a strobe only)
#define REG_ADDR__VP__EMBD_SYNC_ENC_UPDATE_REQUEST                       (video_path_core_registers | 0x00e0)

// read/write bank configuration for embedded sync encoder parameters
#define REG_ADDR__VP__EMBD_SYNC_ENC_BANK_CONFIG                          (video_path_core_registers | 0x00e1)
  // (read_write, bits 0)
  // select which bank to read
  #define BIT_MSK__VP__EMBD_SYNC_ENC_BANK_CONFIG__READ_BANK                                             0x01
  // (read_write, bits 1)
  // select which bank to write
  #define BIT_MSK__VP__EMBD_SYNC_ENC_BANK_CONFIG__WRITE_BANK                                            0x02
  // (read_write, bits 2)
  // select when to update double-buffer
  #define BIT_MSK__VP__EMBD_SYNC_ENC_BANK_CONFIG__UPDATE_MODE                                           0x04
  // (read_write, bits 3)
  // when 1, this double buffer is bypassed for all_update request
  #define BIT_MSK__VP__EMBD_SYNC_ENC_BANK_CONFIG__ALL_UPDATE_BYPASS                                     0x08

//***************************************************************************
// video_path_core_registers__cms_core_registers. address: A0
// module features
#define REG_ADDR__VP__CMS__FEATURES                                      (video_path_core_registers__cms_core_registers | 0x0000)
  // (read_only, bits 0)
  // CMS module enabled
  #define BIT_MSK__VP__CMS__FEATURES__CMS_CORE                                              0x01
  // (read_only, bits 1)
  // pixel capture at the input
  #define BIT_MSK__VP__CMS__FEATURES__PIXCAP_IN                                             0x02
  // (read_only, bits 2)
  // chroma vertical upsampler
  #define BIT_MSK__VP__CMS__FEATURES__C420_C422                                             0x04
  // (read_only, bits 3)
  // chroma horizontal upsampler
  #define BIT_MSK__VP__CMS__FEATURES__C422_C444                                             0x08
  // (read_only, bits 4)
  // multi-_colorspace converter #0
  #define BIT_MSK__VP__CMS__FEATURES__MULTI_CSC0                                            0x10
  // (read_only, bits 5)
  // pixel capture before CMS
  #define BIT_MSK__VP__CMS__FEATURES__PIXCAP_PRE                                            0x20
  // (read_only, bits 6)
  // PWLI #0
  #define BIT_MSK__VP__CMS__FEATURES__PWLI0                                                 0x40
  // (read_only, bits 7)
  // CMS matrix
  #define BIT_MSK__VP__CMS__FEATURES__CMS_MATRIX                                            0x80
  // (read_only, bits 8)
  // PWLI #1
  #define BIT_MSK__VP__CMS__FEATURES__PWLI1                                                 0x100
  // (read_only, bits 9)
  // PWLI #2
  #define BIT_MSK__VP__CMS__FEATURES__PWLI2                                                 0x200
  // (read_only, bits 10)
  // pixel capture after CMS
  #define BIT_MSK__VP__CMS__FEATURES__PIXCAP_POST                                           0x400
  // (read_only, bits 11)
  // multi-_colorspace converter #1
  #define BIT_MSK__VP__CMS__FEATURES__MULTI_CSC1                                            0x800
  // (read_only, bits 12)
  // chroma horizontal subsampler
  #define BIT_MSK__VP__CMS__FEATURES__C444_C422                                             0x1000
  // (read_only, bits 13)
  // chroma vertical subsampler
  #define BIT_MSK__VP__CMS__FEATURES__C422_C420                                             0x2000
  // (read_only, bits 14)
  // dither/_round
  #define BIT_MSK__VP__CMS__FEATURES__DITHER_RND                                            0x4000
  // (read_only, bits 15)
  // range clip
  #define BIT_MSK__VP__CMS__FEATURES__RANGE_CLIP                                            0x8000
  // (read_only, bits 16)
  // pixel capture at the output
  #define BIT_MSK__VP__CMS__FEATURES__PIXCAP_OUT                                            0x10000
  // (read_only, bits 17)
  // retiming VTG/FIFO
  #define BIT_MSK__VP__CMS__FEATURES__RETIMING                                              0x20000

// device build time stamp
#define REG_ADDR__VP__CMS__BUILD_TIME                                    (video_path_core_registers__cms_core_registers | 0x0008)

// number of data bits for the datapath
#define REG_ADDR__VP__CMS__DATA_BITS_VALUE                               (video_path_core_registers__cms_core_registers | 0x000e)

// configure capture module
#define REG_ADDR__VP__CMS__PIXCAP_PRE_CONFIG                             (video_path_core_registers__cms_core_registers | 0x0010)
  // (read_write, bits 0)
  // show capture point
  #define BIT_MSK__VP__CMS__PIXCAP_PRE_CONFIG__SHOW_POINT                                            0x01
  // (read_write, bits 1)
  // enable auto-capture mode
  #define BIT_MSK__VP__CMS__PIXCAP_PRE_CONFIG__AUTO_TRIGGER                                          0x02

// configure capture module
#define REG_ADDR__VP__CMS__PIXCAP_PRE_CONTROL                            (video_path_core_registers__cms_core_registers | 0x0011)
  // (read_write, bits 0)
  // trigger pixel value capture
  #define BIT_MSK__VP__CMS__PIXCAP_PRE_CONTROL__TRIGGER                                               0x01

// capture module status
#define REG_ADDR__VP__CMS__PIXCAP_PRE_STATUS                             (video_path_core_registers__cms_core_registers | 0x0012)
  // (read_only, bits 0)
  // capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__PIXCAP_PRE_STATUS__BUSY                                                  0x01
  // (read_only, bits 1)
  // error occurred, capture location not found
  #define BIT_MSK__VP__CMS__PIXCAP_PRE_STATUS__ERROR                                                 0x02

// specify captured pixel number
#define REG_ADDR__VP__CMS__PIXCAP_PRE_PIXEL                              (video_path_core_registers__cms_core_registers | 0x0014)

// specify captured line number
#define REG_ADDR__VP__CMS__PIXCAP_PRE_LINE                               (video_path_core_registers__cms_core_registers | 0x0016)

// captured Y value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_PRE_Y                                  (video_path_core_registers__cms_core_registers | 0x0018)

// captured cb value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_PRE_CB                                 (video_path_core_registers__cms_core_registers | 0x001a)

// captured cr value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_PRE_CR                                 (video_path_core_registers__cms_core_registers | 0x001c)

// PWLI0 red configuration
#define REG_ADDR__VP__CMS__PWLI0_R_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0020)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // inverse function
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__INVERSE                                               0x02
  // (read_write, bits 2)
  // enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__ERROR_ADJUST                                          0x04
  // (read_write, bits 3)
  // enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__ERROR_RANGE                                           0x08
  // (read_write, bits 6:4)
  // select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__ROUND                                                 0x70
  // (read_write, bits 7)
  // mute output
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__MUTE                                                  0x80
  // (read_write, bits 10:8)
  // operating mode
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__MODE                                                  0x700
  // (read_write, bits 11)
  // bank select for PWLI0 red coefficient table
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__BANK_SEL                                              0x800

// PWLI0 green configuration
#define REG_ADDR__VP__CMS__PWLI0_G_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0022)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // inverse function
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__INVERSE                                               0x02
  // (read_write, bits 2)
  // enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__ERROR_ADJUST                                          0x04
  // (read_write, bits 3)
  // enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__ERROR_RANGE                                           0x08
  // (read_write, bits 6:4)
  // select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__ROUND                                                 0x70
  // (read_write, bits 7)
  // mute output
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__MUTE                                                  0x80
  // (read_write, bits 10:8)
  // operating mode
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__MODE                                                  0x700
  // (read_write, bits 11)
  // bank select for PWLI0 green coefficient table
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__BANK_SEL                                              0x800

// PWLI0 blue configuration
#define REG_ADDR__VP__CMS__PWLI0_B_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0024)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // inverse function
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__INVERSE                                               0x02
  // (read_write, bits 2)
  // enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__ERROR_ADJUST                                          0x04
  // (read_write, bits 3)
  // enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__ERROR_RANGE                                           0x08
  // (read_write, bits 6:4)
  // select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__ROUND                                                 0x70
  // (read_write, bits 7)
  // mute output
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__MUTE                                                  0x80
  // (read_write, bits 10:8)
  // operating mode
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__MODE                                                  0x700
  // (read_write, bits 11)
  // bank select for PWLI0 blue coefficient table
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__BANK_SEL                                              0x800

// bank update request for PWLI0 parameters (write any data value - this is a strobe only)
#define REG_ADDR__VP__CMS__PWLI0_UPDATE_REQUEST                          (video_path_core_registers__cms_core_registers | 0x0026)

// read/write bank configuration for PWLI0 parameters
#define REG_ADDR__VP__CMS__PWLI0_BANK_CONFIG                             (video_path_core_registers__cms_core_registers | 0x0027)
  // (read_write, bits 0)
  // select which bank to read
  #define BIT_MSK__VP__CMS__PWLI0_BANK_CONFIG__READ_BANK                                             0x01
  // (read_write, bits 1)
  // select which bank to write
  #define BIT_MSK__VP__CMS__PWLI0_BANK_CONFIG__WRITE_BANK                                            0x02
  // (read_write, bits 2)
  // select when to update double-buffer
  #define BIT_MSK__VP__CMS__PWLI0_BANK_CONFIG__UPDATE_MODE                                           0x04
  // (read_write, bits 3)
  // when 1, this double buffer is bypassed for all_update request
  #define BIT_MSK__VP__CMS__PWLI0_BANK_CONFIG__ALL_UPDATE_BYPASS                                     0x08

// start address of PWLI0 coefficient table. this register auto-increments when any of the pwli0_r_data/pwli0_g_data/pwli0_b_data registers is read or written. 2 m_s_bs select the coefficient bank
#define REG_ADDR__VP__CMS__PWLI0_ADDR                                    (video_path_core_registers__cms_core_registers | 0x002c)

// PWLI0 red coefficient table data. reading or writing this register increments the pwli0_addr value
#define REG_ADDR__VP__CMS__PWLI0_R_DATA                                  (video_path_core_registers__cms_core_registers | 0x0030)

// PWLI0 green coefficient table data. reading or writing this register increments the pwli0_addr value
#define REG_ADDR__VP__CMS__PWLI0_G_DATA                                  (video_path_core_registers__cms_core_registers | 0x0034)

// PWLI0 blue coefficient table data. reading or writing this register increments the pwli0_addr value
#define REG_ADDR__VP__CMS__PWLI0_B_DATA                                  (video_path_core_registers__cms_core_registers | 0x0038)

// PWLI0 combined coefficient table data. writing this registers increments the pwli0_addr value. no read access!
#define REG_ADDR__VP__CMS__PWLI0_DATA                                    (video_path_core_registers__cms_core_registers | 0x003c)

// 3x3 matrix configuration
#define REG_ADDR__VP__CMS__MATRIX3X3_CONFIG                              (video_path_core_registers__cms_core_registers | 0x0040)
  // (read_write, bits 0)
  // enable 3x3 matrix
  #define BIT_MSK__VP__CMS__MATRIX3X3_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // select dither/round operation
  #define BIT_MSK__VP__CMS__MATRIX3X3_CONFIG__DITHER_ENABLE                                         0x02
  // (read_write, bits 2)
  // enable CMS mode
  #define BIT_MSK__VP__CMS__MATRIX3X3_CONFIG__CMS_MODE                                              0x04
  // (read_write, bits 5:3)
  // coefficient set for forced mode
  #define BIT_MSK__VP__CMS__MATRIX3X3_CONFIG__FORCE_SET                                             0x38
  // (read_write, bits 6)
  // force selected coefficient set
  #define BIT_MSK__VP__CMS__MATRIX3X3_CONFIG__FORCE_ENABLE                                          0x40

// 3x3 matrix set configuration
#define REG_ADDR__VP__CMS__MATRIX3X3_CMS_CONFIG                          (video_path_core_registers__cms_core_registers | 0x0041)
  // (read_write, bits 2:0)
  // coefficient set address
  #define BIT_MSK__VP__CMS__MATRIX3X3_CMS_CONFIG__SET_ADDR                                              0x07
  // (read_write, bits 6:3)
  // coefficient read address
  #define BIT_MSK__VP__CMS__MATRIX3X3_CMS_CONFIG__COEFF_ADDR                                            0x78

// 3x3 matrix set control
#define REG_ADDR__VP__CMS__MATRIX3X3_CMS_CONTROL                         (video_path_core_registers__cms_core_registers | 0x0042)
  // (read_write, bits 0)
  // start write operation
  #define BIT_MSK__VP__CMS__MATRIX3X3_CMS_CONTROL__WRITE                                                 0x01
  // (read_write, bits 1)
  // start read operation
  #define BIT_MSK__VP__CMS__MATRIX3X3_CMS_CONTROL__READ                                                  0x02

// 3x3 matrix set status
#define REG_ADDR__VP__CMS__MATRIX3X3_CMS_STATUS                          (video_path_core_registers__cms_core_registers | 0x0043)
  // (read_only, bits 0)
  // transaction in progress
  #define BIT_MSK__VP__CMS__MATRIX3X3_CMS_STATUS__BUSY                                                  0x01

// R1C1 matrix multiplier coefficient. assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR1C1                       (video_path_core_registers__cms_core_registers | 0x0044)

// R1C2 matrix multiplier coefficient. assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR1C2                       (video_path_core_registers__cms_core_registers | 0x0048)

// R1C3 matrix multiplier coefficient. assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR1C3                       (video_path_core_registers__cms_core_registers | 0x004c)

// R2C1 matrix multiplier coefficient. assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR2C1                       (video_path_core_registers__cms_core_registers | 0x0050)

// R2C2 matrix multiplier coefficient. assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR2C2                       (video_path_core_registers__cms_core_registers | 0x0054)

// R2C3 matrix multiplier coefficient. assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR2C3                       (video_path_core_registers__cms_core_registers | 0x0058)

// R3C1 matrix multiplier coefficient. assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR3C1                       (video_path_core_registers__cms_core_registers | 0x005c)

// R3C2 matrix multiplier coefficient. assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR3C2                       (video_path_core_registers__cms_core_registers | 0x0060)

// R3C3 matrix multiplier coefficient. assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR3C3                       (video_path_core_registers__cms_core_registers | 0x0064)

// CMS matrix coefficient read data
#define REG_ADDR__VP__CMS__MATRIX3X3_CMS_COEFF                           (video_path_core_registers__cms_core_registers | 0x0068)

// signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_IN_GY_OFFSET                        (video_path_core_registers__cms_core_registers | 0x0070)

// signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_IN_BCB_OFFSET                       (video_path_core_registers__cms_core_registers | 0x0072)

// signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_IN_RCR_OFFSET                       (video_path_core_registers__cms_core_registers | 0x0074)

// signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_OUT_GY_OFFSET                       (video_path_core_registers__cms_core_registers | 0x0076)

// signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_OUT_BCB_OFFSET                      (video_path_core_registers__cms_core_registers | 0x0078)

// signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_OUT_RCR_OFFSET                      (video_path_core_registers__cms_core_registers | 0x007a)

// PWLI1 red configuration
#define REG_ADDR__VP__CMS__PWLI1_R_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0080)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // inverse function
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__INVERSE                                               0x02
  // (read_write, bits 2)
  // enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__ERROR_ADJUST                                          0x04
  // (read_write, bits 3)
  // enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__ERROR_RANGE                                           0x08
  // (read_write, bits 6:4)
  // select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__ROUND                                                 0x70
  // (read_write, bits 7)
  // mute output
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__MUTE                                                  0x80
  // (read_write, bits 10:8)
  // operating mode
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__MODE                                                  0x700
  // (read_write, bits 11)
  // bank select for PWLI1 red coefficient table
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__BANK_SEL                                              0x800

// PWLI1 green configuration
#define REG_ADDR__VP__CMS__PWLI1_G_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0082)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // inverse function
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__INVERSE                                               0x02
  // (read_write, bits 2)
  // enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__ERROR_ADJUST                                          0x04
  // (read_write, bits 3)
  // enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__ERROR_RANGE                                           0x08
  // (read_write, bits 6:4)
  // select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__ROUND                                                 0x70
  // (read_write, bits 7)
  // mute output
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__MUTE                                                  0x80
  // (read_write, bits 10:8)
  // operating mode
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__MODE                                                  0x700
  // (read_write, bits 11)
  // bank select for PWLI1 green coefficient table
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__BANK_SEL                                              0x800

// PWLI1 blue configuration
#define REG_ADDR__VP__CMS__PWLI1_B_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0084)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // inverse function
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__INVERSE                                               0x02
  // (read_write, bits 2)
  // enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__ERROR_ADJUST                                          0x04
  // (read_write, bits 3)
  // enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__ERROR_RANGE                                           0x08
  // (read_write, bits 6:4)
  // select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__ROUND                                                 0x70
  // (read_write, bits 7)
  // mute output
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__MUTE                                                  0x80
  // (read_write, bits 10:8)
  // operating mode
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__MODE                                                  0x700
  // (read_write, bits 11)
  // bank select for PWLI1 blue coefficient table
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__BANK_SEL                                              0x800

// bank update request for PWLI1 parameters (write any data value - this is a strobe only)
#define REG_ADDR__VP__CMS__PWLI1_UPDATE_REQUEST                          (video_path_core_registers__cms_core_registers | 0x0086)

// read/write bank configuration for PWLI1 parameters
#define REG_ADDR__VP__CMS__PWLI1_BANK_CONFIG                             (video_path_core_registers__cms_core_registers | 0x0087)
  // (read_write, bits 0)
  // select which bank to read
  #define BIT_MSK__VP__CMS__PWLI1_BANK_CONFIG__READ_BANK                                             0x01
  // (read_write, bits 1)
  // select which bank to write
  #define BIT_MSK__VP__CMS__PWLI1_BANK_CONFIG__WRITE_BANK                                            0x02
  // (read_write, bits 2)
  // select when to update double-buffer
  #define BIT_MSK__VP__CMS__PWLI1_BANK_CONFIG__UPDATE_MODE                                           0x04
  // (read_write, bits 3)
  // when 1, this double buffer is bypassed for all_update request
  #define BIT_MSK__VP__CMS__PWLI1_BANK_CONFIG__ALL_UPDATE_BYPASS                                     0x08

// start address of PWLI1 coefficient table. this register auto-increments when any of the pwli1_r_data/pwli1_g_data/pwli1_b_data registers is read or written. 2 m_s_bs select the coefficient bank
#define REG_ADDR__VP__CMS__PWLI1_ADDR                                    (video_path_core_registers__cms_core_registers | 0x008c)

// PWLI1 red coefficient table data. reading or writing this register increments the pwli1_addr value
#define REG_ADDR__VP__CMS__PWLI1_R_DATA                                  (video_path_core_registers__cms_core_registers | 0x0090)

// PWLI1 green coefficient table data. reading or writing this register increments the pwli1_addr value
#define REG_ADDR__VP__CMS__PWLI1_G_DATA                                  (video_path_core_registers__cms_core_registers | 0x0094)

// PWLI1 blue coefficient table data. reading or writing this register increments the pwli1_addr value
#define REG_ADDR__VP__CMS__PWLI1_B_DATA                                  (video_path_core_registers__cms_core_registers | 0x0098)

// PWLI1 combined coefficient table data. writing this registers increments the pwli1_addr value. no read access!
#define REG_ADDR__VP__CMS__PWLI1_DATA                                    (video_path_core_registers__cms_core_registers | 0x009c)

// pwli2 red configuration
#define REG_ADDR__VP__CMS__PWLI2_R_CONFIG                                (video_path_core_registers__cms_core_registers | 0x00a0)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // inverse function
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__INVERSE                                               0x02
  // (read_write, bits 2)
  // enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__ERROR_ADJUST                                          0x04
  // (read_write, bits 3)
  // enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__ERROR_RANGE                                           0x08
  // (read_write, bits 6:4)
  // select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__ROUND                                                 0x70
  // (read_write, bits 7)
  // mute output
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__MUTE                                                  0x80
  // (read_write, bits 10:8)
  // operating mode
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__MODE                                                  0x700
  // (read_write, bits 11)
  // bank select for pwli2 red coefficient table
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__BANK_SEL                                              0x800

// pwli2 green configuration
#define REG_ADDR__VP__CMS__PWLI2_G_CONFIG                                (video_path_core_registers__cms_core_registers | 0x00a2)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // inverse function
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__INVERSE                                               0x02
  // (read_write, bits 2)
  // enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__ERROR_ADJUST                                          0x04
  // (read_write, bits 3)
  // enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__ERROR_RANGE                                           0x08
  // (read_write, bits 6:4)
  // select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__ROUND                                                 0x70
  // (read_write, bits 7)
  // mute output
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__MUTE                                                  0x80
  // (read_write, bits 10:8)
  // operating mode
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__MODE                                                  0x700
  // (read_write, bits 11)
  // bank select for pwli2 green coefficient table
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__BANK_SEL                                              0x800

// pwli2 blue configuration
#define REG_ADDR__VP__CMS__PWLI2_B_CONFIG                                (video_path_core_registers__cms_core_registers | 0x00a4)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // inverse function
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__INVERSE                                               0x02
  // (read_write, bits 2)
  // enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__ERROR_ADJUST                                          0x04
  // (read_write, bits 3)
  // enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__ERROR_RANGE                                           0x08
  // (read_write, bits 6:4)
  // select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__ROUND                                                 0x70
  // (read_write, bits 7)
  // mute output
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__MUTE                                                  0x80
  // (read_write, bits 10:8)
  // operating mode
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__MODE                                                  0x700
  // (read_write, bits 11)
  // bank select for pwli2 blue coefficient table
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__BANK_SEL                                              0x800

// bank update request for pwli2 parameters (write any data value - this is a strobe only)
#define REG_ADDR__VP__CMS__PWLI2_UPDATE_REQUEST                          (video_path_core_registers__cms_core_registers | 0x00a6)

// read/write bank configuration for pwli2 parameters
#define REG_ADDR__VP__CMS__PWLI2_BANK_CONFIG                             (video_path_core_registers__cms_core_registers | 0x00a7)
  // (read_write, bits 0)
  // select which bank to read
  #define BIT_MSK__VP__CMS__PWLI2_BANK_CONFIG__READ_BANK                                             0x01
  // (read_write, bits 1)
  // select which bank to write
  #define BIT_MSK__VP__CMS__PWLI2_BANK_CONFIG__WRITE_BANK                                            0x02
  // (read_write, bits 2)
  // select when to update double-buffer
  #define BIT_MSK__VP__CMS__PWLI2_BANK_CONFIG__UPDATE_MODE                                           0x04
  // (read_write, bits 3)
  // when 1, this double buffer is bypassed for all_update request
  #define BIT_MSK__VP__CMS__PWLI2_BANK_CONFIG__ALL_UPDATE_BYPASS                                     0x08

// start address of pwli2 coefficient table. this register auto-increments when any of the pwli2_r_data/pwli2_g_data/pwli2_b_data registers is read or written. 2 m_s_bs select the coefficient bank
#define REG_ADDR__VP__CMS__PWLI2_ADDR                                    (video_path_core_registers__cms_core_registers | 0x00ac)

// pwli2 red coefficient table data. reading or writing this register increments the pwli2_addr value
#define REG_ADDR__VP__CMS__PWLI2_R_DATA                                  (video_path_core_registers__cms_core_registers | 0x00b0)

// pwli2 green coefficient table data. reading or writing this register increments the pwli2_addr value
#define REG_ADDR__VP__CMS__PWLI2_G_DATA                                  (video_path_core_registers__cms_core_registers | 0x00b4)

// pwli2 blue coefficient table data. reading or writing this register increments the pwli2_addr value
#define REG_ADDR__VP__CMS__PWLI2_B_DATA                                  (video_path_core_registers__cms_core_registers | 0x00b8)

// pwli2 combined coefficient table data. writing this registers increments the pwli2_addr value. no read access!
#define REG_ADDR__VP__CMS__PWLI2_DATA                                    (video_path_core_registers__cms_core_registers | 0x00bc)

// configure capture module
#define REG_ADDR__VP__CMS__PIXCAP_POST_CONFIG                            (video_path_core_registers__cms_core_registers | 0x00c0)
  // (read_write, bits 0)
  // show capture point
  #define BIT_MSK__VP__CMS__PIXCAP_POST_CONFIG__SHOW_POINT                                            0x01
  // (read_write, bits 1)
  // enable auto-capture mode
  #define BIT_MSK__VP__CMS__PIXCAP_POST_CONFIG__AUTO_TRIGGER                                          0x02

// configure capture module
#define REG_ADDR__VP__CMS__PIXCAP_POST_CONTROL                           (video_path_core_registers__cms_core_registers | 0x00c1)
  // (read_write, bits 0)
  // trigger pixel value capture
  #define BIT_MSK__VP__CMS__PIXCAP_POST_CONTROL__TRIGGER                                               0x01

// capture module status
#define REG_ADDR__VP__CMS__PIXCAP_POST_STATUS                            (video_path_core_registers__cms_core_registers | 0x00c2)
  // (read_only, bits 0)
  // capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__PIXCAP_POST_STATUS__BUSY                                                  0x01
  // (read_only, bits 1)
  // error occurred, capture location not found
  #define BIT_MSK__VP__CMS__PIXCAP_POST_STATUS__ERROR                                                 0x02

// specify captured pixel number
#define REG_ADDR__VP__CMS__PIXCAP_POST_PIXEL                             (video_path_core_registers__cms_core_registers | 0x00c4)

// specify captured line number
#define REG_ADDR__VP__CMS__PIXCAP_POST_LINE                              (video_path_core_registers__cms_core_registers | 0x00c6)

// captured Y value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_POST_Y                                 (video_path_core_registers__cms_core_registers | 0x00c8)

// captured cb value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_POST_CB                                (video_path_core_registers__cms_core_registers | 0x00ca)

// captured cr value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_POST_CR                                (video_path_core_registers__cms_core_registers | 0x00cc)

// CMS demo mode configuration
#define REG_ADDR__VP__CMS__CMS_DEMO_SPLIT_CONFIG                         (video_path_core_registers__cms_core_registers | 0x00d0)
  // (read_write, bits 1:0)
  // select CMS demo display
  #define BIT_MSK__VP__CMS__CMS_DEMO_SPLIT_CONFIG__SELECT                                                0x03
  // (read_write, bits 2)
  // interlace mode
  #define BIT_MSK__VP__CMS__CMS_DEMO_SPLIT_CONFIG__INTERLACE                                             0x04

// demo mode horizontal split point
#define REG_ADDR__VP__CMS__CMS_DEMO_SPLIT_X                              (video_path_core_registers__cms_core_registers | 0x00d2)

// demo mode vertical split point
#define REG_ADDR__VP__CMS__CMS_DEMO_SPLIT_Y                              (video_path_core_registers__cms_core_registers | 0x00d4)

// demo mode split bar width
#define REG_ADDR__VP__CMS__CMS_DEMO_BAR_WIDTH                            (video_path_core_registers__cms_core_registers | 0x00d6)

// demo mode split bar color (G/Y)
#define REG_ADDR__VP__CMS__CMS_DEMO_BAR_DATA_Y                           (video_path_core_registers__cms_core_registers | 0x00d8)

// demo mode split bar color (b/_cb)
#define REG_ADDR__VP__CMS__CMS_DEMO_BAR_DATA_CB                          (video_path_core_registers__cms_core_registers | 0x00da)

// demo mode split bar color (r/_cr)
#define REG_ADDR__VP__CMS__CMS_DEMO_BAR_DATA_CR                          (video_path_core_registers__cms_core_registers | 0x00dc)

//***************************************************************************
// video_path_core_registers__cms_core_registers__csc_core_registers0. address: A0
// module features
#define REG_ADDR__VP__CMS__CSC0__FEATURES                                (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0000)
  // (read_only, bits 0)
  // CSC module enabled
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__CSC_CORE                                              0x01
  // (read_only, bits 1)
  // pixel capture at the input
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__PIXCAP_IN                                             0x02
  // (read_only, bits 2)
  // chroma vertical upsampler
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__C420_C422                                             0x04
  // (read_only, bits 3)
  // chroma horizontal upsampler
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__C422_C444                                             0x08
  // (read_only, bits 4)
  // multi-_colorspace converter
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__MULTI_CSC                                             0x10
  // (read_only, bits 5)
  // chroma horizontal subsampler
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__C444_C422                                             0x20
  // (read_only, bits 6)
  // chroma vertical subsampler
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__C422_C420                                             0x40
  // (read_only, bits 7)
  // dither/_round
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__DITHER_RND                                            0x80
  // (read_only, bits 8)
  // range clip
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__RANGE_CLIP                                            0x100
  // (read_only, bits 9)
  // pixel capture at the output
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__PIXCAP_OUT                                            0x200
  // (read_only, bits 10)
  // retiming VTG/FIFO
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__RETIMING                                              0x400
  // (read_only, bits 11)
  // shared 4:2:0 filter module
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__SHARED_420                                            0x800

// number of data bits for the datapath
#define REG_ADDR__VP__CMS__CSC0__DATA_BITS_VALUE                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0002)

// device build time stamp
#define REG_ADDR__VP__CMS__CSC0__BUILD_TIME                              (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0004)

// configure chroma vertical upsampler
#define REG_ADDR__VP__CMS__CSC0__C420_C422_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0008)
  // (read_write, bits 0)
  // enable chroma vertical upsampler
  #define BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // bypass module to reduce latency
  #define BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__BYPASS                                                0x02
  // (read_write, bits 2)
  // select whether cb or cr bus are used as chroma input
  #define BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__IN_CB_OR_CR                                           0x04
  // (read_write, bits 3)
  // polarity of alternating cb/_cr
  #define BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__IN_CB_CR_POLARITY                                     0x08

// configure chroma upsampler
#define REG_ADDR__VP__CMS__CSC0__C422_C444_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x000c)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__CSC0__C422_C444_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // select whether cb or cr bus are used as muxed chroma input
  #define BIT_MSK__VP__CMS__CSC0__C422_C444_CONFIG__USE_CB_OR_CR                                          0x02
  // (read_write, bits 2)
  // disable lowpass filter
  #define BIT_MSK__VP__CMS__CSC0__C422_C444_CONFIG__DISABLE_FILTER                                        0x04

// configure capture module
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0010)
  // (read_write, bits 0)
  // show capture point
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_IN_CONFIG__SHOW_POINT                                            0x01
  // (read_write, bits 1)
  // enable auto-capture mode
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_IN_CONFIG__AUTO_TRIGGER                                          0x02

// configure capture module
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_CONTROL                       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0011)
  // (read_write, bits 0)
  // trigger pixel value capture
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_IN_CONTROL__TRIGGER                                               0x01

// capture module status
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_STATUS                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0012)
  // (read_only, bits 0)
  // capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_IN_STATUS__BUSY                                                  0x01
  // (read_only, bits 1)
  // error occurred, capture location not found
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_IN_STATUS__ERROR                                                 0x02

// specify captured pixel number
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_PIXEL                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0014)

// specify captured line number
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_LINE                          (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0016)

// captured Y value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_Y                             (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0018)

// captured cb value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_CB                            (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x001a)

// captured cr value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_CR                            (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x001c)

// colorspace converter configuration
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0020)
  // (read_write, bits 1:0)
  // enable conversion
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__ENABLE                                                0x03
  // (read_write, bits 3:2)
  // select output color standard
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_STD                                               0x0C
  // (read_write, bits 4)
  // select output levels
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_PC                                                0x10
  // (read_write, bits 5)
  // select output color space
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_RGB                                               0x20
  // (read_write, bits 7:6)
  // select input color standard
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_STD                                                0xC0
  // (read_write, bits 8)
  // select input levels
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_PC                                                 0x100
  // (read_write, bits 9)
  // select input color space
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_RGB                                                0x200
  // (read_write, bits 10)
  // select dither/round operation
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__DITHER_ENABLE                                         0x400

// R1C1 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR1C1                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0022)

// R1C2 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR1C2                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0024)

// R1C3 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR1C3                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0026)

// R2C1 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR2C1                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0028)

// R2C2 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR2C2                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x002a)

// R2C3 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR2C3                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x002c)

// R3C1 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR3C1                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x002e)

// R3C2 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR3C2                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0030)

// R3C3 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR3C3                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0032)

// signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_IN_GY_OFFSET                  (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0034)

// signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_IN_BCB_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0036)

// signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_IN_RCR_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0038)

// signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_OUT_GY_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x003a)

// signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_OUT_BCB_OFFSET                (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x003c)

// signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_OUT_RCR_OFFSET                (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x003e)

// configure chroma downsampler
#define REG_ADDR__VP__CMS__CSC0__C444_C422_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0040)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__CSC0__C444_C422_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // reserved
  #define BIT_MSK__VP__CMS__CSC0__C444_C422_CONFIG__RESERVED_1                                            0x02
  // (read_write, bits 2)
  // disable lowpass filter
  #define BIT_MSK__VP__CMS__CSC0__C444_C422_CONFIG__DISABLE_FILTER                                        0x04

// configure chroma vertical downsampler
#define REG_ADDR__VP__CMS__CSC0__C422_C420_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0042)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__CSC0__C422_C420_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // bypass module to reduce latency
  #define BIT_MSK__VP__CMS__CSC0__C422_C420_CONFIG__BYPASS                                                0x02
  // (read_write, bits 2)
  // select whether cb or cr bus are used as chroma output
  #define BIT_MSK__VP__CMS__CSC0__C422_C420_CONFIG__OUT_CB_OR_CR                                          0x04
  // (read_write, bits 3)
  // polarity of alternating cb/_cr
  #define BIT_MSK__VP__CMS__CSC0__C422_C420_CONFIG__OUT_CB_CR_POLARITY                                    0x08

// dither configuration
#define REG_ADDR__VP__CMS__CSC0__DITHER_CONFIG                           (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0044)
  // (read_write, bits 1:0)
  // select bits
  #define BIT_MSK__VP__CMS__CSC0__DITHER_CONFIG__MODE                                                  0x03
  // (read_write, bits 2)
  // select dither/round operation
  #define BIT_MSK__VP__CMS__CSC0__DITHER_CONFIG__RND_ENABLE                                            0x04
  // (read_write, bits 3)
  // LFSR spatial mode
  #define BIT_MSK__VP__CMS__CSC0__DITHER_CONFIG__SPATIAL_ENABLE                                        0x08
  // (read_write, bits 4)
  // LFSR spatial dual mode
  #define BIT_MSK__VP__CMS__CSC0__DITHER_CONFIG__SPATIAL_DUAL                                          0x10

// enables dynamic range clipping/saturation to programmable levels. in RGB mode, the Y parameters are used for all 3 color components. there is no bypass or enable.
#define REG_ADDR__VP__CMS__CSC0__RANGE_CLIP_CONFIG                       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0046)
  // (read_write, bits 0)
  // select color space
  #define BIT_MSK__VP__CMS__CSC0__RANGE_CLIP_CONFIG__INPUT_IS_RGB                                          0x01

// Y/RGB minimum clip level
#define REG_ADDR__VP__CMS__CSC0__RANGE_CLIP_Y_MIN                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0048)

// Y/RGB maximum clip level
#define REG_ADDR__VP__CMS__CSC0__RANGE_CLIP_Y_MAX                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x004a)

// cb/_cr minimum clip level
#define REG_ADDR__VP__CMS__CSC0__RANGE_CLIP_C_MIN                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x004c)

// cb/_cr maximum clip level
#define REG_ADDR__VP__CMS__CSC0__RANGE_CLIP_C_MAX                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x004e)

// configure capture module
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_CONFIG                       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0050)
  // (read_write, bits 0)
  // show capture point
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_OUT_CONFIG__SHOW_POINT                                            0x01
  // (read_write, bits 1)
  // enable auto-capture mode
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_OUT_CONFIG__AUTO_TRIGGER                                          0x02

// configure capture module
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_CONTROL                      (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0051)
  // (read_write, bits 0)
  // trigger pixel value capture
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_OUT_CONTROL__TRIGGER                                               0x01

// capture module status
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_STATUS                       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0052)
  // (read_only, bits 0)
  // capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_OUT_STATUS__BUSY                                                  0x01
  // (read_only, bits 1)
  // error occurred, capture location not found
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_OUT_STATUS__ERROR                                                 0x02

// specify captured pixel number
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_PIXEL                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0054)

// specify captured line number
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_LINE                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0056)

// captured Y value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_Y                            (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0058)

// captured cb value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_CB                           (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x005a)

// captured cr value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_CR                           (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x005c)

// end of horizontal sync pulse -1
#define REG_ADDR__VP__CMS__CSC0__VTG_HORIZONTAL_SYNC_END                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0060)

// start of horizontal active video line -1
#define REG_ADDR__VP__CMS__CSC0__VTG_HORIZONTAL_ACTIVE_VIDEO_START       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0062)

// midpoint of horizontal line -2
#define REG_ADDR__VP__CMS__CSC0__VTG_HALFLINE                            (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0064)

// end of horizontal active video line -1
#define REG_ADDR__VP__CMS__CSC0__VTG_HORIZONTAL_ACTIVE_VIDEO_END         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0066)

// end of horizontal line -2
#define REG_ADDR__VP__CMS__CSC0__VTG_END_OF_LINE                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0068)

// end of vertical sync pulse (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_SYNC_END                   (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0070)

// trigger point for processing to start (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC0__VTG_TRIGGER_START                       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0072)

// start of vertical active video (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_ACTIVE_VIDEO_START         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0074)

// end of vertical active video (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_ACTIVE_VIDEO_END           (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0076)

// end of vertical frame (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_END_OF_FRAME               (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0078)

// configure VTG
#define REG_ADDR__VP__CMS__CSC0__VTG_CONFIG                              (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x007a)
  // (read_write, bits 0)
  // enable VTG
  #define BIT_MSK__VP__CMS__CSC0__VTG_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // force resync every frame
  #define BIT_MSK__VP__CMS__CSC0__VTG_CONFIG__RESYNC                                                0x02
  // (read_write, bits 2)
  // unlock VTG
  #define BIT_MSK__VP__CMS__CSC0__VTG_CONFIG__UNLOCK                                                0x04
  // (read_write, bits 3)
  // blank video data
  #define BIT_MSK__VP__CMS__CSC0__VTG_CONFIG__BLANK                                                 0x08

// sync threshold
#define REG_ADDR__VP__CMS__CSC0__VTG_THRESHOLD                           (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x007b)

// number of cycles of delay between reference signal and VTG signal
#define REG_ADDR__VP__CMS__CSC0__VTG_CYCLE_DELAY                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x007c)

// bank update request for video generator parameters.  (write any data value - this is a strobe only)
#define REG_ADDR__VP__CMS__CSC0__VTG_UPDATE_REQUEST                      (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x007e)

// read/write bank configuration for video generator parameters
#define REG_ADDR__VP__CMS__CSC0__VTG_BANK_CONFIG                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x007f)
  // (read_write, bits 0)
  // select which bank to read
  #define BIT_MSK__VP__CMS__CSC0__VTG_BANK_CONFIG__READ_BANK                                             0x01
  // (read_write, bits 1)
  // select which bank to write
  #define BIT_MSK__VP__CMS__CSC0__VTG_BANK_CONFIG__WRITE_BANK                                            0x02
  // (read_write, bits 2)
  // select when to update double-buffer
  #define BIT_MSK__VP__CMS__CSC0__VTG_BANK_CONFIG__UPDATE_MODE                                           0x04

//***************************************************************************
// video_path_core_registers__cms_core_registers__csc_core_registers1. address: A0
// module features
#define REG_ADDR__VP__CMS__CSC1__FEATURES                                (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0000)
  // (read_only, bits 0)
  // CSC module enabled
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__CSC_CORE                                              0x01
  // (read_only, bits 1)
  // pixel capture at the input
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__PIXCAP_IN                                             0x02
  // (read_only, bits 2)
  // chroma vertical upsampler
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__C420_C422                                             0x04
  // (read_only, bits 3)
  // chroma horizontal upsampler
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__C422_C444                                             0x08
  // (read_only, bits 4)
  // multi-_colorspace converter
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__MULTI_CSC                                             0x10
  // (read_only, bits 5)
  // chroma horizontal subsampler
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__C444_C422                                             0x20
  // (read_only, bits 6)
  // chroma vertical subsampler
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__C422_C420                                             0x40
  // (read_only, bits 7)
  // dither/_round
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__DITHER_RND                                            0x80
  // (read_only, bits 8)
  // range clip
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__RANGE_CLIP                                            0x100
  // (read_only, bits 9)
  // pixel capture at the output
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__PIXCAP_OUT                                            0x200
  // (read_only, bits 10)
  // retiming VTG/FIFO
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__RETIMING                                              0x400
  // (read_only, bits 11)
  // shared 4:2:0 filter module
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__SHARED_420                                            0x800

// number of data bits for the datapath
#define REG_ADDR__VP__CMS__CSC1__DATA_BITS_VALUE                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0002)

// device build time stamp
#define REG_ADDR__VP__CMS__CSC1__BUILD_TIME                              (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0004)

// configure chroma vertical upsampler
#define REG_ADDR__VP__CMS__CSC1__C420_C422_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0008)
  // (read_write, bits 0)
  // enable chroma vertical upsampler
  #define BIT_MSK__VP__CMS__CSC1__C420_C422_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // bypass module to reduce latency
  #define BIT_MSK__VP__CMS__CSC1__C420_C422_CONFIG__BYPASS                                                0x02
  // (read_write, bits 2)
  // select whether cb or cr bus are used as chroma input
  #define BIT_MSK__VP__CMS__CSC1__C420_C422_CONFIG__IN_CB_OR_CR                                           0x04
  // (read_write, bits 3)
  // polarity of alternating cb/_cr
  #define BIT_MSK__VP__CMS__CSC1__C420_C422_CONFIG__IN_CB_CR_POLARITY                                     0x08

// configure chroma upsampler
#define REG_ADDR__VP__CMS__CSC1__C422_C444_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x000c)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__CSC1__C422_C444_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // select whether cb or cr bus are used as muxed chroma input
  #define BIT_MSK__VP__CMS__CSC1__C422_C444_CONFIG__USE_CB_OR_CR                                          0x02
  // (read_write, bits 2)
  // disable lowpass filter
  #define BIT_MSK__VP__CMS__CSC1__C422_C444_CONFIG__DISABLE_FILTER                                        0x04

// configure capture module
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0010)
  // (read_write, bits 0)
  // show capture point
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_IN_CONFIG__SHOW_POINT                                            0x01
  // (read_write, bits 1)
  // enable auto-capture mode
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_IN_CONFIG__AUTO_TRIGGER                                          0x02

// configure capture module
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_CONTROL                       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0011)
  // (read_write, bits 0)
  // trigger pixel value capture
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_IN_CONTROL__TRIGGER                                               0x01

// capture module status
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_STATUS                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0012)
  // (read_only, bits 0)
  // capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_IN_STATUS__BUSY                                                  0x01
  // (read_only, bits 1)
  // error occurred, capture location not found
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_IN_STATUS__ERROR                                                 0x02

// specify captured pixel number
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_PIXEL                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0014)

// specify captured line number
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_LINE                          (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0016)

// captured Y value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_Y                             (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0018)

// captured cb value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_CB                            (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x001a)

// captured cr value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_CR                            (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x001c)

// colorspace converter configuration
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0020)
  // (read_write, bits 1:0)
  // enable conversion
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__ENABLE                                                0x03
  // (read_write, bits 3:2)
  // select output color standard
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__OUT_STD                                               0x0C
  // (read_write, bits 4)
  // select output levels
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__OUT_PC                                                0x10
  // (read_write, bits 5)
  // select output color space
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__OUT_RGB                                               0x20
  // (read_write, bits 7:6)
  // select input color standard
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__IN_STD                                                0xC0
  // (read_write, bits 8)
  // select input levels
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__IN_PC                                                 0x100
  // (read_write, bits 9)
  // select input color space
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__IN_RGB                                                0x200
  // (read_write, bits 10)
  // select dither/round operation
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__DITHER_ENABLE                                         0x400

// R1C1 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR1C1                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0022)

// R1C2 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR1C2                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0024)

// R1C3 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR1C3                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0026)

// R2C1 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR2C1                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0028)

// R2C2 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR2C2                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x002a)

// R2C3 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR2C3                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x002c)

// R3C1 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR3C1                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x002e)

// R3C2 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR3C2                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0030)

// R3C3 matrix multiplier coefficient. assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR3C3                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0032)

// signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_IN_GY_OFFSET                  (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0034)

// signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_IN_BCB_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0036)

// signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_IN_RCR_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0038)

// signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_OUT_GY_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x003a)

// signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_OUT_BCB_OFFSET                (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x003c)

// signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_OUT_RCR_OFFSET                (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x003e)

// configure chroma downsampler
#define REG_ADDR__VP__CMS__CSC1__C444_C422_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0040)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__CSC1__C444_C422_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // reserved
  #define BIT_MSK__VP__CMS__CSC1__C444_C422_CONFIG__RESERVED_1                                            0x02
  // (read_write, bits 2)
  // disable lowpass filter
  #define BIT_MSK__VP__CMS__CSC1__C444_C422_CONFIG__DISABLE_FILTER                                        0x04

// configure chroma vertical downsampler
#define REG_ADDR__VP__CMS__CSC1__C422_C420_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0042)
  // (read_write, bits 0)
  // enable
  #define BIT_MSK__VP__CMS__CSC1__C422_C420_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // bypass module to reduce latency
  #define BIT_MSK__VP__CMS__CSC1__C422_C420_CONFIG__BYPASS                                                0x02
  // (read_write, bits 2)
  // select whether cb or cr bus are used as chroma output
  #define BIT_MSK__VP__CMS__CSC1__C422_C420_CONFIG__OUT_CB_OR_CR                                          0x04
  // (read_write, bits 3)
  // polarity of alternating cb/_cr
  #define BIT_MSK__VP__CMS__CSC1__C422_C420_CONFIG__OUT_CB_CR_POLARITY                                    0x08

// dither configuration
#define REG_ADDR__VP__CMS__CSC1__DITHER_CONFIG                           (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0044)
  // (read_write, bits 1:0)
  // select bits
  #define BIT_MSK__VP__CMS__CSC1__DITHER_CONFIG__MODE                                                  0x03
  // (read_write, bits 2)
  // select dither/round operation
  #define BIT_MSK__VP__CMS__CSC1__DITHER_CONFIG__RND_ENABLE                                            0x04
  // (read_write, bits 3)
  // LFSR spatial mode
  #define BIT_MSK__VP__CMS__CSC1__DITHER_CONFIG__SPATIAL_ENABLE                                        0x08
  // (read_write, bits 4)
  // LFSR spatial dual mode
  #define BIT_MSK__VP__CMS__CSC1__DITHER_CONFIG__SPATIAL_DUAL                                          0x10

// enables dynamic range clipping/saturation to programmable levels. in RGB mode, the Y parameters are used for all 3 color components. there is no bypass or enable.
#define REG_ADDR__VP__CMS__CSC1__RANGE_CLIP_CONFIG                       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0046)
  // (read_write, bits 0)
  // select color space
  #define BIT_MSK__VP__CMS__CSC1__RANGE_CLIP_CONFIG__INPUT_IS_RGB                                          0x01

// Y/RGB minimum clip level
#define REG_ADDR__VP__CMS__CSC1__RANGE_CLIP_Y_MIN                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0048)

// Y/RGB maximum clip level
#define REG_ADDR__VP__CMS__CSC1__RANGE_CLIP_Y_MAX                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x004a)

// cb/_cr minimum clip level
#define REG_ADDR__VP__CMS__CSC1__RANGE_CLIP_C_MIN                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x004c)

// cb/_cr maximum clip level
#define REG_ADDR__VP__CMS__CSC1__RANGE_CLIP_C_MAX                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x004e)

// configure capture module
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_CONFIG                       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0050)
  // (read_write, bits 0)
  // show capture point
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_OUT_CONFIG__SHOW_POINT                                            0x01
  // (read_write, bits 1)
  // enable auto-capture mode
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_OUT_CONFIG__AUTO_TRIGGER                                          0x02

// configure capture module
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_CONTROL                      (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0051)
  // (read_write, bits 0)
  // trigger pixel value capture
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_OUT_CONTROL__TRIGGER                                               0x01

// capture module status
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_STATUS                       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0052)
  // (read_only, bits 0)
  // capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_OUT_STATUS__BUSY                                                  0x01
  // (read_only, bits 1)
  // error occurred, capture location not found
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_OUT_STATUS__ERROR                                                 0x02

// specify captured pixel number
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_PIXEL                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0054)

// specify captured line number
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_LINE                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0056)

// captured Y value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_Y                            (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0058)

// captured cb value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_CB                           (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x005a)

// captured cr value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_CR                           (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x005c)

// end of horizontal sync pulse -1
#define REG_ADDR__VP__CMS__CSC1__VTG_HORIZONTAL_SYNC_END                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0060)

// start of horizontal active video line -1
#define REG_ADDR__VP__CMS__CSC1__VTG_HORIZONTAL_ACTIVE_VIDEO_START       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0062)

// midpoint of horizontal line -2
#define REG_ADDR__VP__CMS__CSC1__VTG_HALFLINE                            (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0064)

// end of horizontal active video line -1
#define REG_ADDR__VP__CMS__CSC1__VTG_HORIZONTAL_ACTIVE_VIDEO_END         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0066)

// end of horizontal line -2
#define REG_ADDR__VP__CMS__CSC1__VTG_END_OF_LINE                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0068)

// end of vertical sync pulse (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC1__VTG_VERTICAL_SYNC_END                   (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0070)

// trigger point for processing to start (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC1__VTG_TRIGGER_START                       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0072)

// start of vertical active video (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC1__VTG_VERTICAL_ACTIVE_VIDEO_START         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0074)

// end of vertical active video (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC1__VTG_VERTICAL_ACTIVE_VIDEO_END           (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0076)

// end of vertical frame (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC1__VTG_VERTICAL_END_OF_FRAME               (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0078)

// configure VTG
#define REG_ADDR__VP__CMS__CSC1__VTG_CONFIG                              (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x007a)
  // (read_write, bits 0)
  // enable VTG
  #define BIT_MSK__VP__CMS__CSC1__VTG_CONFIG__ENABLE                                                0x01
  // (read_write, bits 1)
  // force resync every frame
  #define BIT_MSK__VP__CMS__CSC1__VTG_CONFIG__RESYNC                                                0x02
  // (read_write, bits 2)
  // unlock VTG
  #define BIT_MSK__VP__CMS__CSC1__VTG_CONFIG__UNLOCK                                                0x04
  // (read_write, bits 3)
  // blank video data
  #define BIT_MSK__VP__CMS__CSC1__VTG_CONFIG__BLANK                                                 0x08

// sync threshold
#define REG_ADDR__VP__CMS__CSC1__VTG_THRESHOLD                           (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x007b)

// number of cycles of delay between reference signal and VTG signal
#define REG_ADDR__VP__CMS__CSC1__VTG_CYCLE_DELAY                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x007c)

// bank update request for video generator parameters.  (write any data value - this is a strobe only)
#define REG_ADDR__VP__CMS__CSC1__VTG_UPDATE_REQUEST                      (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x007e)

// read/write bank configuration for video generator parameters
#define REG_ADDR__VP__CMS__CSC1__VTG_BANK_CONFIG                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x007f)
  // (read_write, bits 0)
  // select which bank to read
  #define BIT_MSK__VP__CMS__CSC1__VTG_BANK_CONFIG__READ_BANK                                             0x01
  // (read_write, bits 1)
  // select which bank to write
  #define BIT_MSK__VP__CMS__CSC1__VTG_BANK_CONFIG__WRITE_BANK                                            0x02
  // (read_write, bits 2)
  // select when to update double-buffer
  #define BIT_MSK__VP__CMS__CSC1__VTG_BANK_CONFIG__UPDATE_MODE                                           0x04

#endif // _SHERMAN_VID_H_
