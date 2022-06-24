module top(in1, in2, in3, out1);
  input [31:0] in1;
  input [31:0] in2;
  input [31:0] in3;
  output [64:0] out1;
  wire [31:0] in1;
  wire [31:0] in2;
  wire [31:0] in3;
  wire [64:0] out1;
  assign out1 = in3 + in2 + in1*in1;
endmodule
