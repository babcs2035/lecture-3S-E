`timescale 1ns / 1ps

module adder (
    input wire inA,
    input wire inB,
    input wire inC,
    output wire sum,
    output wire cout
);

assign sum = inA ^ inB ^ inC;
assign cout = (inA & inB) | (inA & inC) | (inB & inC);

endmodule

module adder4ripple (
    input [3:0] inA, inB,
    output [3:0] sum, cout
);

genvar i;
generate
    for (i = 0; i < 4; i = i + 1) begin
        if (i == 0)
            adder hoge (inA[i], inB[i], 0, sum[i], cout[i]);
        else
            adder hoge (inA[i], inB[i], cout[i - 1], sum[i], cout[i]);
    end
endgenerate

endmodule

module adder4 (
    input [3:0] inA, inB,
    output [3:0] sum,
    output wire is_overflowed
);

assign sum = inA + inB;
assign is_overflowed = (~inA[3] & ~inB[3] & sum[3]) | (inA[3] & inB[3] & ~sum[3]);

endmodule
