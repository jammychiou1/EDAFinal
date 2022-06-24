module top(in1, in2, in3, in4, in5, in6, out1, out2, out3, out4, out5);

  input [1:0] in1, in2;

  input [4:0] in3, in5;

  input [2:0] in4, in6;

  output [4:0] out1, out3, out5;

  output out2, out4;
  wire [1:0] in1, in2;
  wire [4:0] in3, in5;
  wire [2:0] in4, in6;
  wire [4:0] out1, out3, out5;
  wire out2, out4;
  wire w__1, w__2, w__3, w__4, w__5, w__6, w__7, w__8;
  wire w__9, w__10, w__11, w__12, w__1, w__2, w__15, w__16;
  wire w__17, w__18, w__19, w__20, w__21, w__22, w__23, w__24;
  wire w__25, w__26, w__27, w__28, w__29, w__30, w__31, w__32;
  wire w__33, w__34, w__35, w__36, w__37, w__38, w__39, w__40;
  wire w__41, w__42, w__43, w__44, w__45, w__46, w__47, w__48;
  wire w__49, w__50, w__51, w__52, w__53, w__54, w__55, w__56;
  wire w__57, w__58, w__59, w__60, w__61, w__62, w__63, w__64;
  wire w__65, w__66, w__67, w__68, w__69, w__70, w__71, w__72;
  wire w__73, w__74, w__75, w__76, w__77, w__78, w__79;
  buf constbuf_n1(out5[4] ,1'b0);
  buf constbuf_n2(out3[4] ,1'b0);
  buf constbuf_n3(out1[4] ,1'b0);
  not g__1(w__79 ,in1[0]);
  and g__2(w__12 ,in1[1] ,w__10);
  xor g__3(w__11 ,w__4 ,w__8);
  or g__4(w__10 ,w__5 ,w__9);
  nor g__5(w__9 ,w__4 ,w__6);
  and g__6(out1[0] ,w__4 ,w__7);
  xnor g__7(w__8 ,in2[1] ,in1[1]);
  or g__8(w__7 ,in2[0] ,w__1);
  nor g__9(w__6 ,in2[1] ,in1[1]);
  and g__10(w__5 ,in2[1] ,in1[1]);
  or g__11(w__4 ,w__3 ,w__2);
  not g__12(w__3 ,in2[0]);
  not g__13(w__1 ,w__2);
  not g__14(w__2 ,w__79);
  buf g__15(out1[1] ,w__11);
  buf g__16(out1[3] ,w__12);
  xor g__17(out1[2] ,w__10 ,in1[1]);
  or g__18(w__28 ,w__19 ,w__27);
  xnor g__19(out3[2] ,w__25 ,w__23);
  and g__20(w__27 ,w__17 ,w__25);
  xor g__21(w__26 ,w__18 ,w__22);
  or g__22(w__25 ,w__21 ,w__24);
  nor g__23(w__24 ,w__18 ,w__16);
  and g__24(out3[0] ,w__18 ,w__20);
  xnor g__25(w__23 ,in4[2] ,in1[1]);
  xnor g__26(w__22 ,in4[1] ,in1[1]);
  and g__27(w__21 ,in4[1] ,in1[1]);
  or g__28(w__20 ,in4[0] ,w__1);
  and g__29(w__19 ,in4[2] ,in1[1]);
  or g__30(w__18 ,w__15 ,w__2);
  or g__31(w__17 ,in4[2] ,in1[1]);
  nor g__32(w__16 ,in4[1] ,in1[1]);
  not g__33(w__15 ,in4[0]);
  buf g__36(out3[1] ,w__26);
  buf g__37(out3[3] ,w__28);
  or g__38(w__44 ,w__35 ,w__43);
  xnor g__39(out5[2] ,w__41 ,w__39);
  and g__40(w__43 ,w__33 ,w__41);
  xor g__41(w__42 ,w__34 ,w__38);
  or g__42(w__41 ,w__37 ,w__40);
  nor g__43(w__40 ,w__34 ,w__32);
  and g__44(out5[0] ,w__34 ,w__36);
  xnor g__45(w__39 ,in6[2] ,in1[1]);
  xnor g__46(w__38 ,in6[1] ,in1[1]);
  and g__47(w__37 ,in6[1] ,in1[1]);
  or g__48(w__36 ,in6[0] ,w__29);
  and g__49(w__35 ,in6[2] ,in1[1]);
  or g__50(w__34 ,w__31 ,w__30);
  or g__51(w__33 ,in6[2] ,in1[1]);
  nor g__52(w__32 ,in6[1] ,in1[1]);
  not g__53(w__31 ,in6[0]);
  not g__54(w__29 ,w__30);
  not g__55(w__30 ,w__79);
  buf g__56(out5[1] ,w__42);
  buf g__57(out5[3] ,w__44);
  nor g__58(w__61 ,w__60 ,in3[4]);
  nor g__59(w__60 ,w__52 ,w__59);
  nor g__60(w__59 ,w__51 ,w__58);
  or g__61(w__58 ,w__49 ,w__57);
  nor g__62(w__57 ,w__48 ,w__56);
  nor g__63(w__56 ,w__54 ,w__55);
  nor g__64(w__55 ,in3[1] ,w__53);
  and g__65(w__54 ,out1[1] ,w__50);
  nor g__66(w__53 ,out1[1] ,w__50);
  nor g__67(w__52 ,w__45 ,out1[3]);
  and g__68(w__51 ,out1[3] ,w__45);
  or g__69(w__50 ,w__47 ,out1[0]);
  nor g__70(w__49 ,w__46 ,in3[2]);
  and g__71(w__48 ,in3[2] ,w__46);
  not g__72(w__47 ,in3[0]);
  not g__73(w__46 ,out1[2]);
  not g__74(w__45 ,in3[3]);
  buf g__75(out2 ,w__61);
  nor g__76(w__78 ,w__77 ,in5[4]);
  nor g__77(w__77 ,w__69 ,w__76);
  nor g__78(w__76 ,w__68 ,w__75);
  or g__79(w__75 ,w__66 ,w__74);
  nor g__80(w__74 ,w__65 ,w__73);
  nor g__81(w__73 ,w__71 ,w__72);
  nor g__82(w__72 ,in5[1] ,w__70);
  and g__83(w__71 ,out3[1] ,w__67);
  nor g__84(w__70 ,out3[1] ,w__67);
  nor g__85(w__69 ,w__62 ,out3[3]);
  and g__86(w__68 ,out3[3] ,w__62);
  or g__87(w__67 ,w__64 ,out3[0]);
  nor g__88(w__66 ,w__63 ,in5[2]);
  and g__89(w__65 ,in5[2] ,w__63);
  not g__90(w__64 ,in5[0]);
  not g__91(w__63 ,out3[2]);
  not g__92(w__62 ,in5[3]);
  buf g__93(out4 ,w__78);

endmodule