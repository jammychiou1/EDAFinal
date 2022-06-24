module top(in1, in2, in3, in4, out1);
  input [15:0] in1;
  input [15:0] in2;
  input [15:0] in3;
  input [15:0] in4;
  output [33:0] out1;
  wire [15:0] in1;
  wire [15:0] in2;
  wire [15:0] in3;
  wire [15:0] in4;
  wire [33:0] out1;
  assign out1 = 789*in4 + in3 + 456*in2 + 123*in1;
endmodule
