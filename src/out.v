module top(in1, in2, in3, out1, out2);
  input [7:0] in1;
  input [7:0] in2;
  input [7:0] in3;
  output [23:0] out1;
  output [15:0] out2;
  wire [7:0] in1;
  wire [7:0] in2;
  wire [7:0] in3;
  wire [23:0] out1;
  wire [15:0] out2;
  assign out1 = 8388608*in3*in3 + 8388608*in3*in3*in3 - 4194304*in2*in3 - 4194304*in2*in3*in3 + 8388608*in2*in3*in3*in3 + 8388608*in2*in2 - 4194304*in2*in2*in3 - 4194304*in2*in2*in3*in3 + 8388608*in2*in2*in2 + 8388608*in2*in2*in2*in3 + 8388608*in1 + 8388608*in1*in3 + 8388608*in1*in3*in3 + 8388608*in1*in3*in3*in3 + 4194304*in1*in2*in3 - 4194304*in1*in2*in3*in3 + 8388608*in1*in2*in2 - 4194304*in1*in2*in2*in3 - 4194304*in1*in2*in2*in3*in3 + 8388608*in1*in1 + 8388608*in1*in1*in3 + 8388608*in1*in1*in2*in2*in3 + 8388608*in1*in1*in2*in2*in2 + in1*in1*in1;
  assign out2 = in1*in3;
endmodule
