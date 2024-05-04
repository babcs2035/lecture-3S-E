module sim_counter2;

    parameter HALF_CYCLE = 10;

    reg start, rst, clk;
    wire out;

    counter2 counter2_mod(start, rst, clk, out);

    always begin
        clk = 1'b1;
        #(HALF_CYCLE) clk = 1'b0;
        #(HALF_CYCLE);
    end

    initial begin
      $dumpfile("sim_counter2.vcd");
      $dumpvars(0, counter2_mod);

      start <= 1'b0;
      #15;
      start <= 1'b1;
      #10;
      start <= 1'b0;
      #50;
      rst <= 1'b1;
      #10;
      rst <= 1'b0;
      #30;
      rst <= 1'b1;
      #10;
      rst <= 1'b0;
      #30;
      start <= 1'b1;
      #10;
      start <= 1'b0;
      #130;

      // #70;
      // start <= 1'b1;
      // #20;
      // start <= 1'b0;

      $finish;
    end

endmodule
