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
 align 4
 public _D_DrawParticle_8
_D_DrawParticle_8:
 push ebp
 push edi
 push ebx
 mov edi,ds:dword ptr[12+4+esp]
 fld ds:dword ptr[_vieworg]
 fsubr ds:dword ptr[0+edi]
 fld ds:dword ptr[0+4+edi]
 fsub ds:dword ptr[_vieworg+4]
 fld ds:dword ptr[0+8+edi]
 fsub ds:dword ptr[_vieworg+8]
 fxch st(2)
 fld ds:dword ptr[_r_ppn]
 fmul st(0),st(1)
 fld ds:dword ptr[_r_ppn+4]
 fmul st(0),st(3)
 fld ds:dword ptr[_r_ppn+8]
 fmul st(0),st(5)
 fxch st(2)
 faddp st(1),st(0)
 faddp st(1),st(0)
 fld st(0)
 fdivr ds:dword ptr[float_1]
 fxch st(1)
 fcomp ds:dword ptr[float_particle_z_clip]
 fxch st(3)
 fld ds:dword ptr[_r_pup]
 fmul st(0),st(2)
 fld ds:dword ptr[_r_pup+4]
 fnstsw ax
 test ah,1
 jnz LPop6AndDone
 fmul st(0),st(4)
 fld ds:dword ptr[_r_pup+8]
 fmul st(0),st(3)
 fxch st(2)
 faddp st(1),st(0)
 faddp st(1),st(0)
 fxch st(3)
 fmul ds:dword ptr[_r_pright+4]
 fxch st(2)
 fmul ds:dword ptr[_r_pright]
 fxch st(1)
 fmul ds:dword ptr[_r_pright+8]
 fxch st(2)
 faddp st(1),st(0)
 faddp st(1),st(0)
 fxch st(1)
 fmul st(0),st(2)
 fxch st(1)
 fmul st(0),st(2)
 fxch st(1)
 fadd ds:dword ptr[_centeryfrac]
 fxch st(1)
 fadd ds:dword ptr[_centerxfrac]
 fxch st(1)
 fadd ds:dword ptr[float_point5]
 fxch st(1)
 fadd ds:dword ptr[float_point5]
 fxch st(2)
 fmul ds:dword ptr[DP_32768]
 fxch st(2)
 fistp ds:dword ptr[DP_u]
 fistp ds:dword ptr[DP_v]
 mov eax,ds:dword ptr[DP_u]
 mov edx,ds:dword ptr[DP_v]
 cmp edx,0
 jl LPop1AndDone
 cmp eax,0
 jl LPop1AndDone
 mov ebx,ds:dword ptr[_d_particle_top]
 mov ecx,ds:dword ptr[_d_particle_right]
 cmp edx,ebx
 jg LPop1AndDone
 cmp eax,ecx
 jg LPop1AndDone
 mov ebx,ds:dword ptr[12+edi]
 mov ecx,ebx
 shr ecx,9
 mov ebp,ebx
 shr ebp,6
 and ecx,31744
 shr ebx,3
 and ebp,992
 and ebx,31
 add ecx,ebp
 mov ebp,ds:dword ptr[_d_rgbtable]
 add ebx,ecx
 mov cl,ds:byte ptr[ebp+ebx]
 mov ds:byte ptr[DP_Color],cl
 mov ebx,ds:dword ptr[_scrn]
 add ebx,eax
 mov edi,ds:dword ptr[_ylookup+edx*4]
 lea edx,ds:dword ptr[+edi*2]
 lea edx,ds:dword ptr[edx+eax*2]
 mov eax,ds:dword ptr[_zbuffer]
 fistp ds:dword ptr[izi]
 add edi,ebx
 add edx,eax
 mov eax,ds:dword ptr[izi]
 mov ecx,ds:dword ptr[_d_pix_shift]
 shr eax,cl
 mov ebp,ds:dword ptr[izi]
 mov ebx,ds:dword ptr[_d_pix_min]
 mov ecx,ds:dword ptr[_d_pix_max]
 cmp eax,ebx
 jnl LTestPixMax
 mov eax,ebx
 jmp LTestDone
LTestPixMax:
 cmp eax,ecx
 jng LTestDone
 mov eax,ecx
LTestDone:
 mov ch,ds:byte ptr[DP_Color]
 mov ebx,ds:dword ptr[_d_y_aspect_shift]
 test ebx,ebx
 jnz LDefault
 cmp eax,4
 ja LDefault
 jmp dword ptr[DP_EntryTable-4+eax*4]
DP_EntryTable:
 dd LDP_1x1
 dd LDP_2x2
 dd LDP_3x3
 dd LDP_4x4
LDP_1x1:
 cmp ds:word ptr[edx],bp
 jg LDone
 mov ds:word ptr[edx],bp
 mov ds:byte ptr[edi],ch
 jmp LDone
LDP_2x2:
 push esi
 mov ebx,ds:dword ptr[_d_rowbytes]
 mov esi,ds:dword ptr[_d_zrowbytes]
 cmp ds:word ptr[edx],bp
 jg L2x2_1
 mov ds:word ptr[edx],bp
 mov ds:byte ptr[edi],ch
L2x2_1:
 cmp ds:word ptr[2+edx],bp
 jg L2x2_2
 mov ds:word ptr[2+edx],bp
 mov ds:byte ptr[1+edi],ch
L2x2_2:
 cmp ds:word ptr[edx+esi*1],bp
 jg L2x2_3
 mov ds:word ptr[edx+esi*1],bp
 mov ds:byte ptr[edi+ebx*1],ch
L2x2_3:
 cmp ds:word ptr[2+edx+esi*1],bp
 jg L2x2_4
 mov ds:word ptr[2+edx+esi*1],bp
 mov ds:byte ptr[1+edi+ebx*1],ch
L2x2_4:
 pop esi
 jmp LDone
LDP_3x3:
 push esi
 mov ebx,ds:dword ptr[_d_rowbytes]
 mov esi,ds:dword ptr[_d_zrowbytes]
 cmp ds:word ptr[edx],bp
 jg L3x3_1
 mov ds:word ptr[edx],bp
 mov ds:byte ptr[edi],ch
L3x3_1:
 cmp ds:word ptr[2+edx],bp
 jg L3x3_2
 mov ds:word ptr[2+edx],bp
 mov ds:byte ptr[1+edi],ch
L3x3_2:
 cmp ds:word ptr[4+edx],bp
 jg L3x3_3
 mov ds:word ptr[4+edx],bp
 mov ds:byte ptr[2+edi],ch
L3x3_3:
 cmp ds:word ptr[edx+esi*1],bp
 jg L3x3_4
 mov ds:word ptr[edx+esi*1],bp
 mov ds:byte ptr[edi+ebx*1],ch
L3x3_4:
 cmp ds:word ptr[2+edx+esi*1],bp
 jg L3x3_5
 mov ds:word ptr[2+edx+esi*1],bp
 mov ds:byte ptr[1+edi+ebx*1],ch
L3x3_5:
 cmp ds:word ptr[4+edx+esi*1],bp
 jg L3x3_6
 mov ds:word ptr[4+edx+esi*1],bp
 mov ds:byte ptr[2+edi+ebx*1],ch
L3x3_6:
 cmp ds:word ptr[edx+esi*2],bp
 jg L3x3_7
 mov ds:word ptr[edx+esi*2],bp
 mov ds:byte ptr[edi+ebx*2],ch
L3x3_7:
 cmp ds:word ptr[2+edx+esi*2],bp
 jg L3x3_8
 mov ds:word ptr[2+edx+esi*2],bp
 mov ds:byte ptr[1+edi+ebx*2],ch
L3x3_8:
 cmp ds:word ptr[4+edx+esi*2],bp
 jg L3x3_9
 mov ds:word ptr[4+edx+esi*2],bp
 mov ds:byte ptr[2+edi+ebx*2],ch
L3x3_9:
 pop esi
 jmp LDone
LDP_4x4:
 push esi
 mov ebx,ds:dword ptr[_d_rowbytes]
 mov esi,ds:dword ptr[_d_zrowbytes]
 cmp ds:word ptr[edx],bp
 jg L4x4_1
 mov ds:word ptr[edx],bp
 mov ds:byte ptr[edi],ch
L4x4_1:
 cmp ds:word ptr[2+edx],bp
 jg L4x4_2
 mov ds:word ptr[2+edx],bp
 mov ds:byte ptr[1+edi],ch
L4x4_2:
 cmp ds:word ptr[4+edx],bp
 jg L4x4_3
 mov ds:word ptr[4+edx],bp
 mov ds:byte ptr[2+edi],ch
L4x4_3:
 cmp ds:word ptr[6+edx],bp
 jg L4x4_4
 mov ds:word ptr[6+edx],bp
 mov ds:byte ptr[3+edi],ch
L4x4_4:
 cmp ds:word ptr[edx+esi*1],bp
 jg L4x4_5
 mov ds:word ptr[edx+esi*1],bp
 mov ds:byte ptr[edi+ebx*1],ch
L4x4_5:
 cmp ds:word ptr[2+edx+esi*1],bp
 jg L4x4_6
 mov ds:word ptr[2+edx+esi*1],bp
 mov ds:byte ptr[1+edi+ebx*1],ch
L4x4_6:
 cmp ds:word ptr[4+edx+esi*1],bp
 jg L4x4_7
 mov ds:word ptr[4+edx+esi*1],bp
 mov ds:byte ptr[2+edi+ebx*1],ch
L4x4_7:
 cmp ds:word ptr[6+edx+esi*1],bp
 jg L4x4_8
 mov ds:word ptr[6+edx+esi*1],bp
 mov ds:byte ptr[3+edi+ebx*1],ch
L4x4_8:
 lea edx,ds:dword ptr[edx+esi*2]
 lea edi,ds:dword ptr[edi+ebx*2]
 cmp ds:word ptr[edx],bp
 jg L4x4_9
 mov ds:word ptr[edx],bp
 mov ds:byte ptr[edi],ch
L4x4_9:
 cmp ds:word ptr[2+edx],bp
 jg L4x4_10
 mov ds:word ptr[2+edx],bp
 mov ds:byte ptr[1+edi],ch
L4x4_10:
 cmp ds:word ptr[4+edx],bp
 jg L4x4_11
 mov ds:word ptr[4+edx],bp
 mov ds:byte ptr[2+edi],ch
L4x4_11:
 cmp ds:word ptr[6+edx],bp
 jg L4x4_12
 mov ds:word ptr[6+edx],bp
 mov ds:byte ptr[3+edi],ch
L4x4_12:
 cmp ds:word ptr[edx+esi*1],bp
 jg L4x4_13
 mov ds:word ptr[edx+esi*1],bp
 mov ds:byte ptr[edi+ebx*1],ch
L4x4_13:
 cmp ds:word ptr[2+edx+esi*1],bp
 jg L4x4_14
 mov ds:word ptr[2+edx+esi*1],bp
 mov ds:byte ptr[1+edi+ebx*1],ch
L4x4_14:
 cmp ds:word ptr[4+edx+esi*1],bp
 jg L4x4_15
 mov ds:word ptr[4+edx+esi*1],bp
 mov ds:byte ptr[2+edi+ebx*1],ch
L4x4_15:
 cmp ds:word ptr[6+edx+esi*1],bp
 jg L4x4_16
 mov ds:word ptr[6+edx+esi*1],bp
 mov ds:byte ptr[3+edi+ebx*1],ch
L4x4_16:
 pop esi
 jmp LDone
LDefault:
 mov ebx,eax
 mov ds:dword ptr[DP_Pix],eax
 mov cl,ds:byte ptr[_d_y_aspect_shift]
 shl ebx,cl
LGenRowLoop:
 mov eax,ds:dword ptr[DP_Pix]
LGenColLoop:
 cmp ds:word ptr[-2+edx+eax*2],bp
 jg LGSkip
 mov ds:word ptr[-2+edx+eax*2],bp
 mov ds:byte ptr[-1+edi+eax*1],ch
LGSkip:
 dec eax
 jnz LGenColLoop
 add edx,ds:dword ptr[_d_zrowbytes]
 add edi,ds:dword ptr[_d_rowbytes]
 dec ebx
 jnz LGenRowLoop
LDone:
 pop ebx
 pop edi
 pop ebp
 ret
LPop6AndDone:
 fstp st(0)
 fstp st(0)
 fstp st(0)
 fstp st(0)
 fstp st(0)
LPop1AndDone:
 fstp st(0)
 jmp LDone
_TEXT ENDS
 END
