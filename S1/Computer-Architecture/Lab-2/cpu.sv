module cpu(a1, d1, c1, clk, cnt, hits, misses);
    localparam int CacheAddrSize = 18;

    localparam int Addr1BusSize = 14;
    localparam int Data1BusSize = 16;
    localparam int Ctr1BusSize = 3;

    typedef logic [CacheAddrSize-1:0] address;
    typedef bit [31:0] int32;
    typedef bit [15:0] int16;
    typedef bit [7:0] int8;

    output [Addr1BusSize-1:0] a1;
    inout [Data1BusSize-1:0] d1;
    inout [Ctr1BusSize-1:0] c1;

    address addr_tmp;
    logic [Data1BusSize-1:0] data_tmp;
    logic [Ctr1BusSize-1:0] ctrl_tmp;

    assign a1 = addr_tmp;
    assign d1 = data_tmp;
    assign c1 = ctrl_tmp;

    input clk;

    input [31:0] cnt;

    input [31:0] hits;
    input [31:0] misses;

    address pa;
    address pc;
    address pb;
    int32 s;

    int8 read1;
    int16 read2;

    logic part;

    initial begin
        localparam int M = 64;
        localparam int N = 60;
        localparam int K = 32;

        #1 pa = 0;
        #1 pc = 5888;
        #1 for (int32 y = 0; y < M; y++) begin
            #1 for (int32 x = 0; x < N; x++) begin
                #1 pb = 2048;
                #1 s = 0;
                #1 for (int32 k = 0; k < K; k++) begin
                    read_int8(pa + k, read1);
                    read_int16(pb + x * 2, read2);
                    #6 s += read1 * read2;
                    #1 pb += N * 2;
                    #1;
                end
                write_int32(pc + x * 4, s);
                #1;
            end
            #1 pa += K;
            #1 pc += N * 4;
            #1;
        end
        #1;

        $display("Clk: %d", cnt);

        $display("Hits: %d", hits);
        $display("Misses: %d", misses);
        $display("Efficiency: %d", hits / (hits + misses));
        $finish;
    end

    task static read_int8(input address addr, output int8 data);
        ctrl_tmp = 3'b001;

        addr_tmp = addr[CacheAddrSize-1:CacheAddrSize-Addr1BusSize];
        #1 addr_tmp = addr[CacheAddrSize-Addr1BusSize - 1:0];

        ctrl_tmp = 'z;

        while (ctrl_tmp != 3'b011) begin
            #1;
        end

        data =  data_tmp[Data1BusSize-1:Data1BusSize-8];
    endtask

    task static read_int16(input address addr, output int16 data);
        ctrl_tmp = 3'b010;

        addr_tmp = addr[CacheAddrSize-1:CacheAddrSize-Addr1BusSize];
        #1 addr_tmp = addr[CacheAddrSize-Addr1BusSize - 1:0];

        ctrl_tmp = 'z;

        while (ctrl_tmp != 3'b011) begin
            #1;
        end

        data = data_tmp;
    endtask

    task static write_int32(input address addr, int32 value);
        ctrl_tmp = 3'b111;

        addr_tmp = addr[CacheAddrSize-1:CacheAddrSize-Addr1BusSize];
        #1 addr_tmp = addr[CacheAddrSize-Addr1BusSize - 1:0];

        ctrl_tmp = 'z;
        part = 0;

        while (ctrl_tmp != 3'b111) begin
            case (part)
                1: data_tmp = value[Data1BusSize-1:0];
                default: data_tmp = value[32-1:32-Data1BusSize];
            endcase
            #1 part = ~part;
        end

        data_tmp = 'z;
    endtask
endmodule
