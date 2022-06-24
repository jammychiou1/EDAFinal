module top(in1, in2, in3, out1, out2);
  input [15:0] in1;
  input [15:0] in2;
  input [15:0] in3;
  output [31:0] out1;
  output [31:0] out2;
  wire [15:0] in1;
  wire [15:0] in2;
  wire [15:0] in3;
  wire [31:0] out1;
  wire [31:0] out2;
  assign out1 = 1 + in3 + in1*in2;
  assign out2 = -1 + in3 + in1*in2;
endmodule
