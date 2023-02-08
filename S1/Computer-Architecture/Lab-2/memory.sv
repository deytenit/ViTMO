module memory(a2, d2, c2, clk, rst, m_dump);

    localparam int CacheAddrSize = 18;
    localparam int CacheOffsetSize = 4;

    localparam int ByteSize = 8;

    localparam int Addr2BusSize = 14;
    localparam int Data2BusSize = 16;
    localparam int Ctr2BusSize = 2;

    localparam int MemSize = (1<<CacheAddrSize);

    localparam int Seed = 225526;

    typedef logic [CacheAddrSize-1:0] address;

    input [Addr2BusSize-1:0] a2;
    inout [Data2BusSize-1:0] d2;
    inout [Ctr2BusSize-1:0] c2;

    input clk;
    input rst;
    input m_dump;

    logic [ByteSize-1:0] mem [MemSize];

    logic [Data2BusSize-1:0] data_ray = 'z;
    logic [Ctr2BusSize-1:0] control = 'z;
    logic [3] line_offset;

    assign d2 = data_ray;
    assign c2 = control;

    initial begin
        init(Seed);
    end

    int i;

    address addr;

    always @(posedge clk) begin
        if (rst) begin
            init(Seed);
        end

        if (m_dump) begin
            for (i = 0; i < MemSize; i++) begin
                $display(mem[i]);
            end
        end
    end

    always @(posedge clk) begin
        case (c2)
        2'b10: begin
            addr = a2;
            addr <<= CacheOffsetSize;

            #2;
            #1 control = 2'b00;
            #180;
            control = 2'b01;

            for (i = 0; i < 8; i++) begin
                data_ray[15:8] = mem[addr++];
                data_ray[7:0] = mem[addr++];
                #2;
            end

            control = 'z;
            data_ray = 'z;
            #1;
        end
        2'b11: begin
            addr = a2;
            addr <<= CacheOffsetSize;

            #3 control = 2'b00;

            #183;

            for (i = 0; i < 8; i++) begin
                mem[addr++] = data_ray[15:8];
                mem[addr++] = data_ray[7:0];
                #2;
            end

            #1 control = 2'b01;
            #2 control = 'z;
            #1;
        end
        default: begin
        end
        endcase
    end

    task static init(input int seed);
        for (i = 0; i < MemSize; i += 1) begin
            mem[i] = $random(seed) >> 16;
        end
    endtask
endmodule
