module task1_sim;

    parameter CYCLE = 10;
    parameter HALF_CYCLE = CYCLE / 2;

    reg clk;
    wire [3:0] out;

    task1_mod task1(clk, out);

    initial begin
      $dumpfile("task1_sim.vcd");
      $dumpvars(0, task1);

      for (integer i = 0; i < 256; i = i + 1) begin
        clk = 1'b1;
        #HALF_CYCLE;
        $display("mem[%d]: %h", i, out);
        #HALF_CYCLE;
        clk = 1'b0;
        #CYCLE;
      end

      $finish;
    end

endmodule
