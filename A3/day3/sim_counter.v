module sim_counter;

    parameter HALF_CYCLE = 10;

    reg start, clk;
    wire out;

    counter counter_mod(start, clk, out);

    always begin
        clk = 1'b1;
        #(HALF_CYCLE) clk = 1'b0;
        #(HALF_CYCLE);
    end

    initial begin
      $dumpfile("sim_counter.vcd");
      $dumpvars(0, counter_mod);

      start <= 1'b0;
      #15;
      start <= 1'b1;
      #10;
      start <= 1'b0;
      #130;
      start <= 1'b1;
      #10;
      start <= 1'b0;
      #130;

      $finish;
    end

endmodule
