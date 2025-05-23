`ifndef DEFINE_STATE

typedef enum logic [4:0] {//32 states possible
	S_WAIT_NEW_PIXEL_ROW,
	S_NEW_PIXEL_ROW_DELAY_1,
	S_NEW_PIXEL_ROW_DELAY_2,
	S_NEW_PIXEL_ROW_DELAY_3,
	S_NEW_PIXEL_ROW_DELAY_4,
	S_NEW_PIXEL_ROW_DELAY_5,
	S_FETCH_PIXEL_DATA_0,
	S_FETCH_PIXEL_DATA_1,
	S_FETCH_PIXEL_DATA_2,
	S_FETCH_PIXEL_DATA_3,
	S_IDLE,
	S_FILL_SRAM_RED_SEGMENT_STEP_0,
	S_FILL_SRAM_RED_SEGMENT_STEP_1,
	S_FILL_SRAM_GREEN_SEGMENT_STEP_0,
	S_FILL_SRAM_GREEN_SEGMENT_STEP_1,
	S_FILL_SRAM_BLUE_SEGMENT_STEP_0,
	S_FILL_SRAM_BLUE_SEGMENT_STEP_1,
	S_FINISH_FILL_SRAM,//18th state
	S_9,
	S_10,
	S_11,
	S_12,
	S_13,
	S_14,
	S_15,
	S_16
	
} state_type;

// define the base addresses for the four memory regions		
parameter RED_START_ADDRESS = 18'd0,
	GREEN_EVEN_START_ADDRESS = 18'd38400,
	GREEN_ODD_START_ADDRESS = 18'd57600,
	BLUE_EVEN_START_ADDRESS = 18'd223744,
	BLUE_ODD_START_ADDRESS = 18'd242944;

parameter NUM_ROW_RECTANGLE = 8,
          NUM_COL_RECTANGLE = 8,
          RECT_WIDTH = 40,
          RECT_HEIGHT = 30,
          VIEW_AREA_LEFT = 160,
          VIEW_AREA_RIGHT = 480,
          VIEW_AREA_TOP = 120,
          VIEW_AREA_BOTTOM = 360;

`define DEFINE_STATE 1
`endif

