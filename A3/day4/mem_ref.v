module mem_ref(
  input wire clk,
  output wire [3:0] out
);

  reg we;
  reg [7:0] addr;
  wire [3:0] w_data, r_data;

  bram_ref bram_mod(clk, we, addr, w_data, r_data);

  integer index = -1;
  always @(posedge clk) begin
    index = index + 1;
    addr <= index;
    if (index >= 256)
      index = 0;
  end

  assign out = r_data;

endmodule
