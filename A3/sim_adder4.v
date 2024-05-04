module sim_adder4;
    
    // parameter
    parameter HALF_CYCLE = 100;
    
    // wire/reg
    reg [3:0] inA, inB;
    wire [3:0] S_r, C_r, S;
    wire is_overflowed;

    adder4ripple adder4ripple_mod(
        .inA(inA),
        .inB(inB),
        .sum(S_r),
        .cout(C_r)
    );

    adder4 adder4_mod(
        .inA(inA),
        .inB(inB),
        .sum(S),
        .is_overflowed(is_overflowed)
    );

    // test scenario
    initial begin
    
        $dumpfile("app.vcd");
        $dumpvars(0, sim_adder4);   

        for (integer i = 0; i < 16; i = i + 1) begin
          inA <= i;
            for (integer j = 0; j < 16; j = j + 1) begin
              inB <= j;
              #100 $display("inA=%b, inB=%b, S_r=%b, S=%b (%s)%s", inA, inB, S_r, S,
                  S_r == S ? "OK" : "NG",
                  is_overflowed ? " overflowed": ""
              );
            end
        end
        
        $finish;
    end

endmodule
