module task1_mod(
  input wire clk,
  output wire [3:0] out
);

  reg we;
  reg [7:0] addr = 0;
  wire [3:0] w_data, r_data;

  bram_example bram_mod(clk, we, addr, w_data, r_data);

  integer index = 0;
  always @(posedge clk) begin
    index = index + 1;
    addr <= index;
    if (index >= 256)
      index = 0;
  end

  assign out = r_data;

endmodule
