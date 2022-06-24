module top(in1, in2, in3, in4, out1);
  input [15:0] in1;
  input [15:0] in2;
  input [15:0] in3;
  input [1:0] in4;
  output [31:0] out1;
  wire [15:0] in1;
  wire [15:0] in2;
  wire [15:0] in3;
  wire [1:0] in4;
  wire [31:0] out1;
  if (in4 == 0) begin
    assign out1 = 0;
  end
  else if (in4 == 1) begin
    assign out1 = in2*in3;
  end
  else if (in4 == 2) begin
    assign out1 = in1*in3;
  end
  else if (in4 == 3) begin
    assign out1 = in1*in2;
  end
endmodule
