/*
Copyright by Henry Ko and Nicola Nicolici
Department of Electrical and Computer Engineering
McMaster University
Ontario, Canada
*/

`timescale 1ns/100ps
`ifndef DISABLE_DEFAULT_NET
`default_nettype none
`endif

module experiment3 (
		/////// board clocks                      ////////////
		input logic CLOCK_50_I,                   // 50 MHz clock

		/////// switches                          ////////////
		input logic[17:0] SWITCH_I,               // toggle switches

		/////// LEDs                              ////////////
		output logic[8:0] LED_GREEN_O             // 9 green LEDs
);

logic resetn;
assign resetn = ~SWITCH_I[17];

enum logic [1:0] {
	S_READ,
	S_WRITE,
	S_END,
	S_IDLE
} state;

logic [8:0] read_address,read_address256;
logic [7:0] write_data_a [1:0];
logic [7:0] write_data_b [1:0];
logic write_enable_a [1:0];
logic write_enable_b [1:0];
logic [7:0] read_data_a [1:0];
logic [7:0] read_data_b [1:0];

// instantiate RAM0
dual_port_RAM0 RAM_inst0 (
	.address_a ( read_address ),
	.address_b ( read_address256 ),
	.clock ( CLOCK_50_I ),
	.data_a ( write_data_a[0] ),
	.data_b ( write_data_b[0] ),
	.wren_a ( write_enable_a[0] ),
	.wren_b ( write_enable_b[0] ),
	.q_a ( read_data_a[0] ),
	.q_b ( read_data_b[0] )
	);

// instantiate RAM1
dual_port_RAM1 RAM_inst1 (
	.address_a ( read_address ),
	.address_b ( read_address256 ),
	.clock ( CLOCK_50_I ),
	.data_a ( write_data_a[1] ),
	.data_b ( write_data_b[1] ),
	.wren_a ( write_enable_a[1] ),
	.wren_b ( write_enable_b[1] ),
	.q_a ( read_data_a[1] ),
	.q_b ( read_data_b[1] )
	);

// the top port is used only for reading for both memories
// hence we disable the write enable for the top port 
//assign write_enable_a [0] = 1'b0;
//assign write_enable_a [1] = 1'b0;
// since write enable is disabled for the top port we can 
// assign write data on the top port to some dummy values

// the adder for the write port of the first RAM
assign write_data_a[0] = (write_enable_a[0] == 1'b1) ?read_data_b[0] + read_data_b[1] : write_data_a[0];
assign write_data_a[1] = (write_enable_a[1] == 1'b1) ?read_data_a[0] - read_data_a[1] : write_data_a[1];
assign write_data_b[0] = (write_enable_b[0] == 1'b1) ?read_data_a[0] + read_data_b[1] : write_data_b[0];
assign write_data_b[1] = (write_enable_b[1] == 1'b1) ?read_data_b[0] - read_data_a[1] : write_data_b[1];
// expand as requested for the write port of the RAM1
// note: this write enable must be registered
// and asserted ONLY when write data is valid

// FSM to control the read and write sequence
always_ff @ (posedge CLOCK_50_I or negedge resetn) begin
	if (resetn == 1'b0) begin
		read_address <= 9'd0;
		read_address256 <= 9'd256;
		write_enable_a[0] <= 1'b0;
		write_enable_a[1] <= 1'b0;
		write_enable_b[0] <= 1'b0;
		write_enable_b[1] <= 1'b0;
		state <= S_IDLE;
	end else begin
		case (state)
			S_IDLE: begin	
				// wait for switch[0] to be asserted
				if (SWITCH_I[0])
					state <= S_READ;
			end
			//RAM 0 -> W
			//R0A W[k] R0B W[k+256]
			//0A -> W[k+256] + X[k+256]
			//0B -> W[k] + X[k+256]
			//RAM 1 -> X
			//R1A X[k] R1B X[k+256]
			//1A -> W[k] - X[k]
			//1B -> W[k+256] - X[k]
			S_READ: begin				
				write_enable_a[0] <= 1'b1;
				write_enable_a[1] <= 1'b1;
				write_enable_b[0] <= 1'b1;
				write_enable_b[1] <= 1'b1;
				state <= S_WRITE;
			end
			S_WRITE: begin
				write_enable_a[0] <= 1'b0;
				write_enable_a[1] <= 1'b0;
				write_enable_b[0] <= 1'b0;
				write_enable_b[1] <= 1'b0;				
				if(read_address == 255) begin
					state <= S_END;
				end else begin
					read_address <= read_address + 9'd1;
					read_address256 <= read_address256 + 9'd1;
					state <= S_READ;
				end				
			end
			S_END: begin	
				// in this state the last write completes
				// then the next cc we move back to S_IDLE
				read_address <= 9'd0;
				read_address256 <= 9'd0;
				state <= S_IDLE;
			end
		endcase
	end
end

// dump some dummy values on the output green LEDs to constrain 
// the synthesis tools not to remove the circuit logic
assign LED_GREEN_O = {1'b0, {write_data_b[1] ^ write_data_b[0]}};

endmodule
