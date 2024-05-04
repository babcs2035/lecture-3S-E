module sim_example;

    parameter HALF_CYCLE = 30;

    reg inA, clk;
    wire out1, out2;

    example example_mod(inA, clk, out1, out2);

    always begin
        clk = 1'b1;
        #(HALF_CYCLE) clk = 1'b0;
        #(HALF_CYCLE);
    end

    initial begin
      $dumpfile("sim_example.vcd");
      $dumpvars(0, example_mod);

      inA <= 1'b0;
      #100;
      inA <= 1'b1;
      #100;
      inA <= 1'b0;
      #100;
      inA <= 1'b1;
      #100;

      $finish;
    end

endmodule
