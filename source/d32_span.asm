 .486P
 .model FLAT
 externdef _d_sdivzstepu
 externdef _d_tdivzstepu
 externdef _d_zistepu
 externdef _d_sdivzstepv
 externdef _d_tdivzstepv
 externdef _d_zistepv
 externdef _d_sdivzorigin
 externdef _d_tdivzorigin
 externdef _d_ziorigin
 externdef _sadjust
 externdef _tadjust
 externdef _bbextents
 externdef _bbextentt
 externdef _cacheblock
 externdef _cachewidth
 externdef _ds_transluc
 externdef _ds_transluc16
 externdef _r_lightptr
 externdef _r_lightptrr
 externdef _r_lightptrg
 externdef _r_lightptrb
 externdef _r_lightwidth
 externdef _r_numvblocks
 externdef _r_sourcemax
 externdef _r_stepback
 externdef _prowdestbase
 externdef _pbasesource
 externdef _sourcetstep
 externdef _surfrowbytes
 externdef _lightright
 externdef _lightrightstep
 externdef _lightdeltastep
 externdef _lightdelta
 externdef _lightrleft
 externdef _lightrright
 externdef _lightrleftstep
 externdef _lightrrightstep
 externdef _lightgleft
 externdef _lightgright
 externdef _lightgleftstep
 externdef _lightgrightstep
 externdef _lightbleft
 externdef _lightbright
 externdef _lightbleftstep
 externdef _lightbrightstep
 externdef sdivz8stepu
 externdef tdivz8stepu
 externdef zi8stepu
 externdef sdivz16stepu
 externdef tdivz16stepu
 externdef zi16stepu
 externdef s
 externdef t
 externdef snext
 externdef tnext
 externdef sstep
 externdef tstep
 externdef sfracf
 externdef tfracf
 externdef spancountminus1
 externdef izi
 externdef izistep
 externdef advancetable
 externdef pbase
 externdef pz
 externdef reciprocal_table
 externdef pspantemp
 externdef counttemp
 externdef jumptemp
 externdef mmbuf
 externdef fp_64k
 externdef fp_8
 externdef fp_16
 externdef Float2ToThe31nd
 externdef FloatMinus2ToThe31nd
 externdef fp_64kx64k
 externdef float_1
 externdef float_particle_z_clip
 externdef float_point5
 externdef DP_u
 externdef DP_v
 externdef DP_32768
 externdef DP_Color
 externdef DP_Pix
 externdef ceil_cw
 externdef single_cw
 externdef _ylookup
 externdef _zbuffer
 externdef _scrn
 externdef _scrn16
 externdef _pal8_to16
 externdef _mmx_mask4
 externdef _mmx_mask8
 externdef _mmx_mask16
 externdef _d_rowbytes
 externdef _d_zrowbytes
 externdef _vieworg
 externdef _r_ppn
 externdef _r_pup
 externdef _r_pright
 externdef _centerxfrac
 externdef _centeryfrac
 externdef _d_particle_right
 externdef _d_particle_top
 externdef _d_pix_min
 externdef _d_pix_max
 externdef _d_pix_shift
 externdef _d_y_aspect_shift
 externdef _d_rgbtable
 externdef _rshift
 externdef _gshift
 externdef _bshift
 externdef _roffs
 externdef _goffs
 externdef _boffs
 externdef _fadetable
 externdef _fadetable16
 externdef _fadetable16r
 externdef _fadetable16g
 externdef _fadetable16b
 externdef _fadetable32
 externdef _fadetable32r
 externdef _fadetable32g
 externdef _fadetable32b
 externdef _pr_globals
 externdef _pr_stackPtr
 externdef _pr_statements
 externdef _pr_functions
 externdef _pr_globaldefs
 externdef _pr_builtins
 externdef _current_func
 externdef _D_DrawZSpan
 externdef _PR_RFInvalidOpcode
_TEXT SEGMENT
LClampHigh0:
 mov esi,ds:dword ptr[_bbextents]
 jmp LClampReentry0
LClampHighOrLow0:
 jg LClampHigh0
 xor esi,esi
 jmp LClampReentry0
LClampHigh1:
 mov edx,ds:dword ptr[_bbextentt]
 jmp LClampReentry1
LClampHighOrLow1:
 jg LClampHigh1
 xor edx,edx
 jmp LClampReentry1
LClampLow2:
 mov ebp,2048
 jmp LClampReentry2
LClampHigh2:
 mov ebp,ds:dword ptr[_bbextents]
 jmp LClampReentry2
LClampLow3:
 mov ecx,2048
 jmp LClampReentry3
LClampHigh3:
 mov ecx,ds:dword ptr[_bbextentt]
 jmp LClampReentry3
LClampLow4:
 mov eax,2048
 jmp LClampReentry4
LClampHigh4:
 mov eax,ds:dword ptr[_bbextents]
 jmp LClampReentry4
LClampLow5:
 mov ebx,2048
 jmp LClampReentry5
LClampHigh5:
 mov ebx,ds:dword ptr[_bbextentt]
 jmp LClampReentry5
 align 4
 public _D_DrawSpans8_32
_D_DrawSpans8_32:
 push ebp
 push edi
 push esi
 push ebx
 fld ds:dword ptr[_d_sdivzstepu]
 fmul ds:dword ptr[fp_8]
 mov edx,ds:dword ptr[_cacheblock]
 fld ds:dword ptr[_d_tdivzstepu]
 fmul ds:dword ptr[fp_8]
 mov ebx,ds:dword ptr[4+16+esp]
 fld ds:dword ptr[_d_zistepu]
 fmul ds:dword ptr[fp_8]
 mov ds:dword ptr[pbase],edx
 fstp ds:dword ptr[zi8stepu]
 fstp ds:dword ptr[tdivz8stepu]
 fstp ds:dword ptr[sdivz8stepu]
LSpanLoop:
 fild ds:dword ptr[4+ebx]
 fild ds:dword ptr[0+ebx]
 fld st(1)
 fmul ds:dword ptr[_d_sdivzstepv]
 fld st(1)
 fmul ds:dword ptr[_d_sdivzstepu]
 fld st(2)
 fmul ds:dword ptr[_d_tdivzstepu]
 fxch st(1)
 faddp st(2),st(0)
 fxch st(1)
 fld st(3)
 fmul ds:dword ptr[_d_tdivzstepv]
 fxch st(1)
 fadd ds:dword ptr[_d_sdivzorigin]
 fxch st(4)
 fmul ds:dword ptr[_d_zistepv]
 fxch st(1)
 faddp st(2),st(0)
 fxch st(2)
 fmul ds:dword ptr[_d_zistepu]
 fxch st(1)
 fadd ds:dword ptr[_d_tdivzorigin]
 fxch st(2)
 faddp st(1),st(0)
 fld ds:dword ptr[fp_64k]
 fxch st(1)
 fadd ds:dword ptr[_d_ziorigin]
 fdiv st(1),st(0)
 mov edi,ds:dword ptr[_scrn]
 mov eax,ds:dword ptr[4+ebx]
 mov ds:dword ptr[pspantemp],ebx
 mov edx,ds:dword ptr[_tadjust]
 mov esi,ds:dword ptr[_sadjust]
 mov ecx,ds:dword ptr[_ylookup+eax*4]
 add ecx,ds:dword ptr[0+ebx]
 lea edi,ds:dword ptr[edi+ecx*4]
 mov ecx,ds:dword ptr[8+ebx]
 cmp ecx,8
 ja LSetupNotLast1
 dec ecx
 jz LCleanup1
 mov ds:dword ptr[spancountminus1],ecx
 fxch st(1)
 fld st(0)
 fmul st(0),st(4)
 fxch st(1)
 fmul st(0),st(3)
 fxch st(1)
 fistp ds:dword ptr[s]
 fistp ds:dword ptr[t]
 fild ds:dword ptr[spancountminus1]
 fld ds:dword ptr[_d_tdivzstepu]
 fld ds:dword ptr[_d_zistepu]
 fmul st(0),st(2)
 fxch st(1)
 fmul st(0),st(2)
 fxch st(2)
 fmul ds:dword ptr[_d_sdivzstepu]
 fxch st(1)
 faddp st(3),st(0)
 fxch st(1)
 faddp st(3),st(0)
 faddp st(3),st(0)
 fld ds:dword ptr[fp_64k]
 fdiv st(0),st(1)
 jmp LFDIVInFlight1
LCleanup1:
 fxch st(1)
 fld st(0)
 fmul st(0),st(4)
 fxch st(1)
 fmul st(0),st(3)
 fxch st(1)
 fistp ds:dword ptr[s]
 fistp ds:dword ptr[t]
 jmp LFDIVInFlight1
 align 4
LSetupNotLast1:
 fxch st(1)
 fld st(0)
 fmul st(0),st(4)
 fxch st(1)
 fmul st(0),st(3)
 fxch st(1)
 fistp ds:dword ptr[s]
 fistp ds:dword ptr[t]
 fadd ds:dword ptr[zi8stepu]
 fxch st(2)
 fadd ds:dword ptr[sdivz8stepu]
 fxch st(2)
 fld ds:dword ptr[tdivz8stepu]
 faddp st(2),st(0)
 fld ds:dword ptr[fp_64k]
 fdiv st(0),st(1)
LFDIVInFlight1:
 add esi,ds:dword ptr[s]
 add edx,ds:dword ptr[t]
 mov ebx,ds:dword ptr[_bbextents]
 mov ebp,ds:dword ptr[_bbextentt]
 cmp esi,ebx
 ja LClampHighOrLow0
LClampReentry0:
 mov ds:dword ptr[s],esi
 mov ebx,ds:dword ptr[pbase]
 shl esi,16
 cmp edx,ebp
 mov ds:dword ptr[sfracf],esi
 ja LClampHighOrLow1
LClampReentry1:
 mov ds:dword ptr[t],edx
 mov esi,ds:dword ptr[s]
 shl edx,16
 mov eax,ds:dword ptr[t]
 sar esi,16
 mov ds:dword ptr[tfracf],edx
 sar eax,16
 mov edx,ds:dword ptr[_cachewidth]
 sal esi,2
 lea edx,ds:dword ptr[+edx*4]
 imul eax,edx
 add esi,ebx
 add esi,eax
 cmp ecx,8
 jna LLastSegment
LNotLastSegment:
 fld st(0)
 fmul st(0),st(4)
 fxch st(1)
 fmul st(0),st(3)
 fxch st(1)
 fistp ds:dword ptr[snext]
 fistp ds:dword ptr[tnext]
 mov eax,ds:dword ptr[snext]
 mov edx,ds:dword ptr[tnext]
 mov ebx,ds:dword ptr[esi]
 sub ecx,8
 mov ebp,ds:dword ptr[_sadjust]
 mov ds:dword ptr[counttemp],ecx
 mov ds:dword ptr[edi],ebx
 mov ecx,ds:dword ptr[_tadjust]
 add ebp,eax
 add ecx,edx
 mov eax,ds:dword ptr[_bbextents]
 mov edx,ds:dword ptr[_bbextentt]
 cmp ebp,2048
 jl LClampLow2
 cmp ebp,eax
 ja LClampHigh2
LClampReentry2:
 cmp ecx,2048
 jl LClampLow3
 cmp ecx,edx
 ja LClampHigh3
LClampReentry3:
 mov ds:dword ptr[snext],ebp
 mov ds:dword ptr[tnext],ecx
 sub ebp,ds:dword ptr[s]
 sub ecx,ds:dword ptr[t]
 mov eax,ecx
 mov edx,ebp
 sar eax,19
 jz LZero
 sar edx,19
 mov ebx,ds:dword ptr[_cachewidth]
 lea ebx,ds:dword ptr[+ebx*4]
 imul eax,ebx
 jmp LSetUp1
LZero:
 sar edx,19
 mov ebx,ds:dword ptr[_cachewidth]
 lea ebx,ds:dword ptr[+ebx*4]
LSetUp1:
 lea edx,ds:dword ptr[+edx*4]
 add eax,edx
 mov edx,ds:dword ptr[tfracf]
 mov ds:dword ptr[advancetable+8],eax
 mov ds:dword ptr[advancetable+12],eax
 add eax,ebx
 shl ebp,13
 mov ebx,ds:dword ptr[sfracf]
 shl ecx,13
 mov ds:dword ptr[advancetable],eax
 mov ds:dword ptr[advancetable+4],eax
 add ds:dword ptr[advancetable+4],4
 add ds:dword ptr[advancetable+12],4
 mov ds:dword ptr[tstep],ecx
 add edx,ecx
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[4+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[8+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[12+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 mov ecx,ds:dword ptr[counttemp]
 cmp ecx,8
 ja LSetupNotLast2
 dec ecx
 jz LFDIVInFlight2
 mov ds:dword ptr[spancountminus1],ecx
 fild ds:dword ptr[spancountminus1]
 fld ds:dword ptr[_d_zistepu]
 fmul st(0),st(1)
 fld ds:dword ptr[_d_tdivzstepu]
 fmul st(0),st(2)
 fxch st(1)
 faddp st(3),st(0)
 fxch st(1)
 fmul ds:dword ptr[_d_sdivzstepu]
 fxch st(1)
 faddp st(3),st(0)
 fld ds:dword ptr[fp_64k]
 fxch st(1)
 faddp st(4),st(0)
 fdiv st(0),st(1)
 jmp LFDIVInFlight2
 align 4
LSetupNotLast2:
 fadd ds:dword ptr[zi8stepu]
 fxch st(2)
 fadd ds:dword ptr[sdivz8stepu]
 fxch st(2)
 fld ds:dword ptr[tdivz8stepu]
 faddp st(2),st(0)
 fld ds:dword ptr[fp_64k]
 fdiv st(0),st(1)
LFDIVInFlight2:
 mov ds:dword ptr[counttemp],ecx
 add edx,ds:dword ptr[tstep]
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[16+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[20+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[24+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edi,32
 mov ds:dword ptr[tfracf],edx
 mov edx,ds:dword ptr[snext]
 mov ds:dword ptr[sfracf],ebx
 mov ebx,ds:dword ptr[tnext]
 mov ds:dword ptr[s],edx
 mov ds:dword ptr[t],ebx
 mov ecx,ds:dword ptr[counttemp]
 cmp ecx,8
 mov ds:dword ptr[-4+edi],eax
 ja LNotLastSegment
LLastSegment:
 test ecx,ecx
 jz LNoSteps
 fld st(0)
 fmul st(0),st(4)
 fxch st(1)
 fmul st(0),st(3)
 fxch st(1)
 fistp ds:dword ptr[snext]
 fistp ds:dword ptr[tnext]
 mov eax,ds:dword ptr[esi]
 mov ebx,ds:dword ptr[_tadjust]
 mov ds:dword ptr[edi],eax
 mov eax,ds:dword ptr[_sadjust]
 add eax,ds:dword ptr[snext]
 add ebx,ds:dword ptr[tnext]
 mov ebp,ds:dword ptr[_bbextents]
 mov edx,ds:dword ptr[_bbextentt]
 cmp eax,2048
 jl LClampLow4
 cmp eax,ebp
 ja LClampHigh4
LClampReentry4:
 mov ds:dword ptr[snext],eax
 cmp ebx,2048
 jl LClampLow5
 cmp ebx,edx
 ja LClampHigh5
LClampReentry5:
 cmp ecx,1
 je LOnlyOneStep
 sub eax,ds:dword ptr[s]
 sub ebx,ds:dword ptr[t]
 add eax,eax
 add ebx,ebx
 imul ds:dword ptr[reciprocal_table-8+ecx*4]
 mov ebp,edx
 mov eax,ebx
 imul ds:dword ptr[reciprocal_table-8+ecx*4]
LSetEntryvec:
 mov ebx,ds:dword ptr[Lentryvec_table+ecx*4]
 mov eax,edx
 mov ds:dword ptr[jumptemp],ebx
 mov ecx,ebp
 sar edx,16
 mov ebx,ds:dword ptr[_cachewidth]
 sar ecx,16
 sal ecx,2
 lea ebx,ds:dword ptr[+ebx*4]
 imul edx,ebx
 add edx,ecx
 mov ecx,ds:dword ptr[tfracf]
 mov ds:dword ptr[advancetable+8],edx
 mov ds:dword ptr[advancetable+12],edx
 add edx,ebx
 shl ebp,16
 mov ebx,ds:dword ptr[sfracf]
 shl eax,16
 mov ds:dword ptr[advancetable],edx
 mov ds:dword ptr[advancetable+4],edx
 add ds:dword ptr[advancetable+4],4
 add ds:dword ptr[advancetable+12],4
 mov ds:dword ptr[tstep],eax
 mov edx,ecx
 add edx,eax
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 jmp dword ptr[jumptemp]
 align 4
Lentryvec_table:
 dd 0
 dd LEntry2_8
 dd LEntry3_8
 dd LEntry4_8
 dd LEntry5_8
 dd LEntry6_8
 dd LEntry7_8
 dd LEntry8_8
LNoSteps:
 mov eax,ds:dword ptr[esi]
 sub edi,28
 jmp LEndSpan
LOnlyOneStep:
 sub eax,ds:dword ptr[s]
 sub ebx,ds:dword ptr[t]
 mov ebp,eax
 mov edx,ebx
 jmp LSetEntryvec
LEntry2_8:
 sub edi,24
 mov eax,ds:dword ptr[esi]
 jmp LLEntry2_8
LEntry3_8:
 sub edi,20
 add edx,eax
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 jmp LLEntry3_8
LEntry4_8:
 sub edi,16
 add edx,eax
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 jmp LLEntry4_8
LEntry5_8:
 sub edi,12
 add edx,eax
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 jmp LLEntry5_8
LEntry6_8:
 sub edi,8
 add edx,eax
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 jmp LLEntry6_8
LEntry7_8:
 sub edi,4
 add edx,eax
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 jmp LLEntry7_8
LEntry8_8:
 add edx,eax
 mov eax,ds:dword ptr[esi]
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[4+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
LLEntry7_8:
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[8+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
LLEntry6_8:
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[12+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
LLEntry5_8:
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[16+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
 add edx,ds:dword ptr[tstep]
LLEntry4_8:
 sbb ecx,ecx
 add ebx,ebp
 adc ecx,ecx
 mov ds:dword ptr[20+edi],eax
 mov eax,ds:dword ptr[esi]
 add esi,ds:dword ptr[advancetable+8+ecx*4]
LLEntry3_8:
 mov ds:dword ptr[24+edi],eax
 mov eax,ds:dword ptr[esi]
LLEntry2_8:
LEndSpan:
 fstp st(0)
 fstp st(0)
 fstp st(0)
 mov ebx,ds:dword ptr[pspantemp]
 mov ebx,ds:dword ptr[12+ebx]
 test ebx,ebx
 mov ds:dword ptr[28+edi],eax
 jnz LSpanLoop
 pop ebx
 pop esi
 pop edi
 pop ebp
 ret
_TEXT ENDS
 END
