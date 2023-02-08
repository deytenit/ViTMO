`include "memory.sv"
`include "cache.sv"
`include "cpu.sv"

module testbench;

    localparam int Addr1BusSize = 14;
    localparam int Data1BusSize = 16;
    localparam int Ctr1BusSize = 3;

    localparam int Addr2BusSize = 14;
    localparam int Data2BusSize = 16;
    localparam int Ctr2BusSize = 2;

    tri [Addr1BusSize-1:0] a1;
    tri [Data1BusSize-1:0] d1;
    tri [Ctr1BusSize-1:0] c1;

    tri [Addr2BusSize-1:0] a2;
    tri [Data2BusSize-1:0] d2;
    tri [Ctr2BusSize-1:0] c2;

    logic clk;
    logic rst;
    logic [31:0] cnt = 0;

    tri [31:0] misses;
    tri [31:0] hits;

    cpu cpu_instance(.a1(a1), .d1(d1), .c1(c1), .clk(clk), .cnt(cnt), .hits(hits), .misses(misses));

    cache cache_instance(
        .a1(a1),
        .d1(d1),
        .c1(c1),
        .a2(a2),
        .d2(d2),
        .c2(c2),
        .clk(clk),
        .rst(rst),
        .hits(hits),
        .misses(misses)
    );

   memory memory_instance(
        .a2(a2),
        .d2(d2),
        .c2(c2),
        .clk(clk),
        .rst(rst)
    );
    always #1 begin
        clk = ~clk;

        cnt++;
    end
endmodule
