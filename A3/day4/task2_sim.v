module task2_sim;

    parameter CYCLE = 10;
    parameter HALF_CYCLE = CYCLE / 2;

    reg clk;
    wire [3:0] out1, out2, sum, cout;
    reg [3:0] num1, num2;
    wire is_overflowed;

    task1_mod task1(clk, out1);
    mem_ref ref(clk, out2);
    adder4ripple adder(num1, num2, sum, cout, is_overflowed);

    initial begin
      $dumpfile("task2_sim.vcd");
      $dumpvars(0, task1);

      for (integer i = 0; i < 256; i = i + 1) begin
        clk = 1'b1;
        #HALF_CYCLE;
        num1 <= num2;
        num2 <= out1;
        #HALF_CYCLE;
        clk = 1'b0;
        $display("mem[%d] + mem[%d]: %h (%b) + %h (%b) = %h (%b)%s <> %h (%b) (%s)", i - 1, i, num1, num1, num2, num2, sum, sum, is_overflowed ? " (overflowed)" : "", out2, out2, sum == out2 ? "OK" : "NG");
        #CYCLE;
      end

      $finish;
    end

endmodule
