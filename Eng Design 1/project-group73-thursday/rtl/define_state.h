`ifndef DEFINE_STATE

// for top state - we have more states than needed
typedef enum logic [1:0] {
	S_IDLE,
	S_UART_RX,
	S_Milestone1,
	S_Milestone2
} top_state_type;

typedef enum logic [1:0] {
	S_RXC_IDLE,
	S_RXC_SYNC,
	S_RXC_ASSEMBLE_DATA,
	S_RXC_STOP_BIT
} RX_Controller_state_type;

typedef enum logic [2:0] {
	S_US_IDLE,
	S_US_STRIP_FILE_HEADER_1,
	S_US_STRIP_FILE_HEADER_2,
	S_US_START_FIRST_BYTE_RECEIVE,
	S_US_WRITE_FIRST_BYTE,
	S_US_START_SECOND_BYTE_RECEIVE,
	S_US_WRITE_SECOND_BYTE
} UART_SRAM_state_type;

typedef enum logic [3:0] {
	S_VS_WAIT_NEW_PIXEL_ROW,
	S_VS_NEW_PIXEL_ROW_DELAY_1,
	S_VS_NEW_PIXEL_ROW_DELAY_2,
	S_VS_NEW_PIXEL_ROW_DELAY_3,
	S_VS_NEW_PIXEL_ROW_DELAY_4,
	S_VS_NEW_PIXEL_ROW_DELAY_5,
	S_VS_FETCH_PIXEL_DATA_0,
	S_VS_FETCH_PIXEL_DATA_1,
	S_VS_FETCH_PIXEL_DATA_2,
	S_VS_FETCH_PIXEL_DATA_3
} VGA_SRAM_state_type;

typedef enum logic [5:0] {
	S_LI_0,
	S_LI_1,
	S_LI_2,
	S_LI_3,
	S_LI_4,
	S_LI_5,
	S_LI_6,
	S_LI_7,
	S_LI_8,
	S_LI_9,
	S_LI_10,
	S_LI_11,
	S_LI_12,
	S_LI_13,
	S_LI_14,
	S_LI_15,
	S_CC_1,
	S_CC_2,
	S_CC_3,
	S_CC_4,
	S_CC_5,
	S_CC_6,
	S_CC_7,
	S_CC_8,
	S_CC_9,
	S_CC_10,
	S_CC_11,
	S_LO_1,
	S_LO_2,
	S_LO_3,
	S_LO_4,
	S_LO_5,
	S_LO_6	
} M1_State_type;


typedef enum logic [7:0] {
	S_FetchS_0,
	S_FetchS_1,
	S_FetchS_2,
	S_FetchS_3,
	S_FetchS_4,
	S_FetchS_5,
	S_FetchS_6,
	S_FetchS_7,
	S_FetchS_8,
	S_COMPUTE_T0,
	S_COMPUTE_T1,
	S_COMPUTE_T2,
	S_COMPUTE_T3,
	S_COMPUTE_T4,
	S_COMPUTE_T5,
	S_COMPUTE_T6,
	S_COMPUTE_T7,
	S_COMPUTE_T8,
	S_COMPUTE_T9,
	S_COMPUTE_T10,
	S_COMPUTE_T11,
	S_COMPUTE_T12,
	S_COMPUTE_T13,
	S_COMPUTE_T14,
	S_COMPUTE_T15,
	S_COMPUTE_T16,
	S_COMPUTE_S0,
	S_COMPUTE_S1,
	S_COMPUTE_S2,
	S_COMPUTE_S3,
	S_COMPUTE_S4,
	S_COMPUTE_S5,
	S_COMPUTE_S6,
	S_COMPUTE_S7,
	S_COMPUTE_S8,
	S_COMPUTE_S9,
	S_COMPUTE_S10,
	S_COMPUTE_S11,
	S_COMPUTE_S12,
	S_COMPUTE_S13,
	S_COMPUTE_S14,
	S_COMPUTE_S15,
	S_COMPUTE_S16,
	S_S_FINISH,
	S_Write_S1,
	S_Write_S2
	
} M2_State_type;


parameter 

   VIEW_AREA_LEFT = 160,
   VIEW_AREA_RIGHT = 480,
   VIEW_AREA_TOP = 120,
   VIEW_AREA_BOTTOM = 360,
	//Milestone1
	Y_ADDRESS_START = 18'd0,
	U_ADDRESS_START = 18'd38400,
	V_ADDRESS_START = 18'd57600,
	RGB_ADDRESS_START = 18'd146944,
	k_a00 =32'd76284,
	k_a02 =32'd104595,
	k_a11 =-32'sd25624,
	k_a12 =-32'sd53281,
	k_a21 =32'd132251,
	k_5 = 32'd21,
	k_3 = -32'sd52,
	k_1 = 32'd159,
	//Milestone2
	YUV_PRE_START = 18'd76800;

`define DEFINE_STATE 1
`endif
