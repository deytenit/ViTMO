module cache(a1, d1, c1, a2, d2, c2, clk, rst, c_dump, hits, misses);
    localparam int Addr1BusSize = 14;
    localparam int Data1BusSize = 16;
    localparam int Ctr1BusSize = 3;

    localparam int Addr2BusSize = 14;
    localparam int Data2BusSize = 16;
    localparam int Ctr2BusSize = 2;

    localparam int CacheSetsCount = 64;
    localparam int CacheLineSize = 128;
    localparam int CacheSetSize = 6;
    localparam int CacheOffsetSize = 4;
    localparam int AddressSize = 18;

    localparam int CacheTagSize = 8;

    localparam int ActualLineSize = CacheTagSize + 3 + CacheLineSize;
    localparam int ValidBit = ActualLineSize - CacheTagSize - 2;
    localparam int DirtyBit = ActualLineSize - CacheTagSize - 3;
    localparam int DeltaBit = ActualLineSize - CacheTagSize - 4;
    localparam int DataBegin = ActualLineSize - CacheTagSize - 5;

    //typedef logic [ActualLineSize-1:0] line;
    //typedef line [CacheSetsCount] way;
    typedef logic [AddressSize-1:0] address;

    input [Addr1BusSize-1:0] a1;
    inout [Data1BusSize-1:0] d1;
    inout [Ctr1BusSize-1:0] c1;

    logic [Addr1BusSize-1:0] addr1_tmp;
    logic [Data1BusSize-1:0] data1_tmp;
    logic [Ctr1BusSize-1:0] ctrl1_tmp;

    assign a1 = addr1_tmp;
    assign d1 = data1_tmp;
    assign c1 = ctrl1_tmp;

    output [Addr2BusSize-1:0] a2;
    inout [Data2BusSize-1:0] d2;
    inout [Ctr2BusSize-1:0] c2;

    logic [Addr2BusSize-1:0] addr2_tmp;
    logic [Data2BusSize-1:0] data2_tmp;
    logic [Ctr2BusSize-1:0] ctrl2_tmp;

    assign a2 = addr2_tmp;
    assign d2 = data2_tmp;
    assign c2 = ctrl2_tmp;

    input clk;
    input rst;
    input c_dump;

    output [31:0] hits;
    output [31:0] misses;

    int hits_tmp = 0;
    int miss_tmp = 0;

    assign hits = hits_tmp;
    assign misses = miss_tmp;

    logic [ActualLineSize-1:0] way1 [CacheSetsCount];
    logic [ActualLineSize-1:0] way2 [CacheSetsCount];

    int i;

    initial begin
        hits_tmp = 0;
        miss_tmp = 0;
        ctrl1_tmp = 'z;
        data1_tmp = 'z;
        ctrl2_tmp = 2'b00;
        for (i = 0; i < CacheSetsCount; i++) begin
            way1[i] = 0;
            way2[i] = 0;
        end
    end

    always @(posedge clk) begin
        if (rst) begin
            hits_tmp = 0;
            miss_tmp = 0;
            ctrl1_tmp = 'z;
            data1_tmp = 'z;
            ctrl2_tmp = 2'b00;
            for (i = 0; i < CacheSetsCount; i++) begin
                way1[i] = 0;
                way2[i] = 0;
            end
        end
        if (c_dump) begin
            for (int i = 0; i < CacheSetsCount; i++) begin
                $display(way1[i]);
                $display(way2[i]);
            end
        end
    end

    logic [Addr1BusSize-1:0] tag_set;
    logic [CacheOffsetSize-1:0] offset;

    int way_to;

    logic [CacheSetSize-1:0] set_to;

    always @(posedge clk) begin
        case (c1)
        3'b001: begin
            tag_set = a1;
            set_to = tag_set[CacheSetSize:0];
            # 2 offset = a1;
            # 1 ctrl1_tmp = 3'b000;
            find(tag_set, way_to);

            if (way_to == 0) begin
                read_line(addr_tag_set(tag_set), way_to);
            end

            ctrl1_tmp = 3'b111;

            if (way_to == 1) begin
                for (int i = DataBegin-offset*8-1; i >= DataBegin-offset*8-8; i--) begin
                data1_tmp[DataBegin-offset*8-1 - i]
                    = way1[set_to][i];
                end
            end
            else begin
                for (int i = DataBegin-offset*8-1; i >= DataBegin-offset*8-8; i--) begin
                data1_tmp[DataBegin-offset*8-1 - i]
                    = way2[set_to][i];
                end
            end

            #2

            ctrl1_tmp = 'z;
            data1_tmp = 'z;
        end
        3'b010: begin
            tag_set = a1;
            set_to = tag_set[CacheSetSize:0];
            # 2 offset = a1;
            # 1 ctrl1_tmp = 3'b000;
            find(tag_set, way_to);

            if (way_to == 0) begin
                read_line(addr_tag_set(tag_set), way_to);
            end

            ctrl1_tmp = 3'b111;

            if (way_to == 1) begin
                for (int i = DataBegin-offset*8-1; i >= DataBegin-offset*8-16; i--) begin
                data1_tmp[DataBegin-offset*8-1-i]
                    = way1[set_to][i];
                end
            end
            else begin
                for (int i = DataBegin-offset*8-1; i >= DataBegin-offset*8-16; i--) begin
                data1_tmp[DataBegin-offset*8-1-i]
                    = way2[set_to][i];
                end
            end


            #2

            ctrl1_tmp = 'z;
            data1_tmp = 'z;
        end
        3'b111: begin
            tag_set = a1;
            set_to = tag_set[CacheSetSize:0];
            # 2 offset = a1;
            find(tag_set, way_to);

            if (way_to == 0) begin
                #1 read_line(addr_tag_set(tag_set), way_to);
                #1;
            end

            if (way_to == 1) begin
                for (int i = DataBegin-offset*8-1; i >= DataBegin-offset*8-16; i--) begin
                    way1[set_to][i] = data1_tmp[DataBegin-offset*8-1-i];
                end
                #2;
                for (int i = DataBegin-offset*8-17; i >= DataBegin-offset*8-32; i--) begin
                    way1[set_to][i] = data1_tmp[DataBegin-offset*8-17-i];
                end
            end
            else begin
                for (int i = DataBegin-offset*8-1; i >= DataBegin-offset*8-16; i--) begin
                    way2[set_to][i] = data1_tmp[DataBegin-offset*8-1-i];
                end
                #2;
                for (int i = DataBegin-offset*8-17; i >= DataBegin-offset*8-32; i--) begin
                    way2[set_to][i] = data1_tmp[DataBegin-offset*8-17-i];
                end
            end

            #2

            ctrl1_tmp = 'z;
        end
        default: begin
        end
        endcase
    end

    task automatic write_back(input int way_select, address addr);
        logic [CacheSetSize-1:0] set = addr_set(addr);
        int offset = 0;

        if (way_select == 1
        && way1[set][ValidBit] == 1 && way1[set][DirtyBit] == 1) begin
            addr2_tmp = addr[AddressSize-1:CacheOffsetSize];
            ctrl2_tmp = 2'b11;

            #2 ctrl2_tmp = 'z;

            while (ctrl2_tmp != 2'b01) begin
                for (
                    int i = DataBegin*Data2BusSize-1;
                    i >= DataBegin-offset*Data2BusSize-Data2BusSize;
                    i--
                ) begin
                    data2_tmp[DataBegin*Data2BusSize-1-i] = way1[set][i];
                end
                offset++;
            end

            way1[set][ValidBit] = 0;
            way1[set][DirtyBit] = 0;
            way1[set][DeltaBit] = 1;
        end
        else if (way_select == 2
        && way2[set][ValidBit] == 1 && way2[set][DirtyBit] == 1) begin
            addr2_tmp = addr[AddressSize-1:CacheOffsetSize];
            ctrl2_tmp = 2'b11;

            #2 ctrl2_tmp = 'z;

            while (ctrl2_tmp != 2'b01) begin
                for (
                    int i = DataBegin*Data2BusSize-1;
                    i >= DataBegin-offset*Data2BusSize-Data2BusSize;
                    i--
                ) begin
                    data2_tmp[DataBegin*Data2BusSize-1-i] = way2[set][i];
                end
                offset++;
            end

            way2[set][ValidBit] = 0;
            way2[set][DirtyBit] = 0;
            way2[set][DeltaBit] = 1;
        end
    endtask

    task automatic lru(input address addr, output int way_idx);
        logic [CacheSetSize-1:0] set = addr_set(addr);

        if (way1[set][ValidBit] == 1 && way2[set][ValidBit] == 1) begin
            way_idx = way1[set][DeltaBit] > way2[set][DeltaBit] ? 1 : 0;
        end
        else begin
            way_idx = way1[set][ValidBit] == 1 ? 2 : 1;
        end
    endtask

    task automatic read_line(input address addr, output int way_idx);
        logic [CacheSetSize-1:0] set = addr_set(addr);

        int way_select;
        lru(addr, way_select);

        write_back(way_select, addr);

        if (way_select == 1) begin
            way1[set][ActualLineSize-1:ActualLineSize-CacheTagSize] = addr_tag(addr);
            way1[set][DeltaBit] = 0;
            way1[set][ValidBit] = 1;
            way1[set][DirtyBit] = 0;

            way_idx = 1;
        end
        else begin
            way2[set][ActualLineSize-1:ActualLineSize-CacheTagSize] = addr_tag(addr);
            way2[set][DeltaBit] = 0;
            way2[set][ValidBit] = 1;
            way2[set][DirtyBit] = 0;

            way_idx = 2;
        end
    endtask

    task automatic find(input logic [Addr1BusSize-1:0] query, output int set_idx);
        logic [CacheSetSize-1:0] set = query;
        logic [CacheTagSize-1:0] tag = query >> CacheSetSize;

        if (way1[set][ValidBit] == 1
            && way1[set][ActualLineSize-1:ActualLineSize-CacheTagSize] == tag) begin
            if (way2[set][ValidBit] == 1) begin
                way1[set][DeltaBit] = 0;
                way2[set][DeltaBit] = 1;
            end

            hits_tmp++;
            #6 set_idx = 1;
        end
        else if (way2[set][ValidBit] == 1
            && way2[set][ActualLineSize-1:ActualLineSize-CacheTagSize] == tag) begin
            if (way1[set][ValidBit] == 1) begin
                way2[set][DeltaBit] = 0;
                way1[set][DeltaBit] = 1;
            end

            hits_tmp++;
            #6 set_idx = 2;
        end
        else begin

            miss_tmp++;
            #4 set_idx = 0;
        end
    endtask

    function automatic address addr_tag_set(input logic [Addr1BusSize-1:0] value);
        address result = value;
        return result << CacheOffsetSize;
    endfunction

    function static logic [CacheTagSize-1:0] addr_tag(address addr);
        return addr[AddressSize-1:AddressSize-CacheTagSize];
    endfunction

    function static logic [CacheSetSize-1:0] addr_set(address addr);
        return addr[AddressSize-CacheTagSize-1:AddressSize-CacheTagSize-CacheSetSize];
    endfunction

    function static logic [CacheOffsetSize-1:0] addr_offset(address addr);
        return addr[CacheOffsetSize-1:0];
    endfunction

endmodule
