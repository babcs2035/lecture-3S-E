`timescale 1ns / 1ps

module counter (
  input wire start,
  input wire clk,
  output wire out
);

  reg out_reg;
  integer cnt = -1;

  always @(posedge clk) begin
    if (start == 1'b1)
      cnt = 0;
    if (cnt > -1) begin
      if (cnt == 0)
        out_reg <= 1'b1;
      else
        out_reg <= 1'b0;
    cnt = (cnt + 1) % 4;
    end
  end

  assign out = out_reg;

endmodule
