#ifndef __PROCESSOR_IIC_CATEGORIES__HPP
#define __PROCESSOR_IIC_CATEGORIES__HPP

#define PROCESSOR_IIC_CATEGORIES(IIC, CHANGE_CAT)       \
  CHANGE_CAT(GENERIC_ALU)				\
  IIC(ALU_MEM)						\
  IIC(ALU_NONMEM)					\
  IIC(LEA)						\
  IIC(LEA_16)						\
  IIC(MUL8)						\
  IIC(MUL16_MEM)					\
  IIC(MUL16_REG)					\
  IIC(MUL32_MEM)					\
  IIC(MUL32_REG)					\
  IIC(MUL64)						\
                                                        \
  CHANGE_CAT(INTEGER_MULTIPLICATION)			\
  IIC(IMUL8)						\
  IIC(IMUL16_MEM)					\
  IIC(IMUL16_REG)					\
  IIC(IMUL32_MEM)					\
  IIC(IMUL32_REG)					\
  IIC(IMUL64)						\
  IIC(IMUL16_RM)					\
  IIC(IMUL16_RR)					\
  IIC(IMUL32_RM)					\
  IIC(IMUL32_RR)					\
  IIC(IMUL64_RM)					\
  IIC(IMUL64_RR)					\
  IIC(IMUL16_RMI)					\
  IIC(IMUL16_RRI)					\
  IIC(IMUL32_RMI)					\
  IIC(IMUL32_RRI)					\
  IIC(IMUL64_RMI)					\
  IIC(IMUL64_RRI)					\
							\
  CHANGE_CAT(FLOAT_DIVISION)				\
  IIC(DIV8_MEM)						\
  IIC(DIV8_REG)						\
  IIC(DIV16)						\
  IIC(DIV32)						\
  IIC(DIV64)						\
							\
  CHANGE_CAT(INTEGER_DIVISION)				\
  IIC(IDIV8)						\
  IIC(IDIV16)						\
  IIC(IDIV32)						\
  IIC(IDIV64)						\
							\
  CHANGE_CAT(UNARY_ARITHMETIC_OPERATION)		\
  IIC(UNARY_REG)					\
  IIC(UNARY_MEM)					\
							\
  CHANGE_CAT(BINARY_ARITHMETIC_OPERATION)		\
  IIC(BIN_MEM)						\
  IIC(BIN_NONMEM)					\
  	                                                \
  CHANGE_CAT(SHIFT_ROTATE)                              \
  IIC(SR)                                               \
                                                        \
  CHANGE_CAT(DOUBLE_SHIFT)                              \
  IIC(SHD16_REG_IM)                                     \
  IIC(SHD16_REG_CL)                                     \
  IIC(SHD16_MEM_IM)                                     \
  IIC(SHD16_MEM_CL)                                     \
  IIC(SHD32_REG_IM)                                     \
  IIC(SHD32_REG_CL)                                     \
  IIC(SHD32_MEM_IM)                                     \
  IIC(SHD32_MEM_CL)                                     \
  IIC(SHD64_REG_IM)                                     \
  IIC(SHD64_REG_CL)                                     \
  IIC(SHD64_MEM_IM)                                     \
  IIC(SHD64_MEM_CL)                                     \
                                                        \
  CHANGE_CAT(CONDITIONAL_MOVE)                          \
  IIC(CMOV16_RM)                                        \
  IIC(CMOV16_RR)                                        \
  IIC(CMOV32_RM)                                        \
  IIC(CMOV32_RR)                                        \
  IIC(CMOV64_RM)                                        \
  IIC(CMOV64_RR)                                        \
                                                        \
  CHANGE_CAT(SET)                                       \
  IIC(SET_R)                                            \
  IIC(SET_M)                                            \
                                                        \
  CHANGE_CAT(JUMP)                                      \
  IIC(Jcc)                                              \
  IIC(JCXZ)                                             \
  IIC(JMP_REL)                                          \
  IIC(JMP_REG)                                          \
  IIC(JMP_MEM)                                          \
  IIC(JMP_FAR_MEM)                                      \
  IIC(JMP_FAR_PTR)                                      \
                                                        \
  CHANGE_CAT(LOOP)                                      \
  IIC(LOOP)                                             \
  IIC(LOOPE)                                            \
  IIC(LOOPNE)                                           \
  IIC(REP_MOVS)                                         \
  IIC(REP_STOS)                                         \
                                                        \
  CHANGE_CAT(CALL)                                      \
  IIC(CALL_RI)                                          \
  IIC(CALL_MEM)                                         \
  IIC(CALL_FAR_MEM)                                     \
  IIC(CALL_FAR_PTR)                                     \
                                                        \
  CHANGE_CAT(RETURN)                                    \
  IIC(RET)                                              \
  IIC(RET_IMM)                                          \
                                                        \
  CHANGE_CAT(SIGN_EXTENSION)                            \
  IIC(MOVSX)                                            \
  IIC(MOVSX_R16_R8)                                     \
  IIC(MOVSX_R16_M8)                                     \
  IIC(MOVSX_R16_R16)                                    \
  IIC(MOVSX_R32_R32)                                    \
                                                        \
  CHANGE_CAT(ZERO_EXTENSION)                            \
  IIC(MOVZX)                                            \
  IIC(MOVZX_R16_R8)                                     \
  IIC(MOVZX_R16_M8)                                     \
                                                        \
  CHANGE_CAT(SSE)                                       \
  IIC(SSE_ALU_F32S_RR)                                  \
  IIC(SSE_ALU_F32S_RM)                                  \
  IIC(SSE_ALU_F64S_RR)                                  \
  IIC(SSE_ALU_F64S_RM)                                  \
  IIC(SSE_MUL_F32S_RR)                                  \
  IIC(SSE_MUL_F32S_RM)                                  \
  IIC(SSE_MUL_F64S_RR)                                  \
  IIC(SSE_MUL_F64S_RM)                                  \
  IIC(SSE_DIV_F32S_RR)                                  \
  IIC(SSE_DIV_F32S_RM)                                  \
  IIC(SSE_DIV_F64S_RR)                                  \
  IIC(SSE_DIV_F64S_RM)                                  \
  IIC(SSE_ALU_F32P_RR)                                  \
  IIC(SSE_ALU_F32P_RM)                                  \
  IIC(SSE_ALU_F64P_RR)                                  \
  IIC(SSE_ALU_F64P_RM)                                  \
  IIC(SSE_MUL_F32P_RR)                                  \
  IIC(SSE_MUL_F32P_RM)                                  \
  IIC(SSE_MUL_F64P_RR)                                  \
  IIC(SSE_MUL_F64P_RM)                                  \
  IIC(SSE_DIV_F32P_RR)                                  \
  IIC(SSE_DIV_F32P_RM)                                  \
  IIC(SSE_DIV_F64P_RR)                                  \
  IIC(SSE_DIV_F64P_RM)                                  \
                                                        \
  IIC(SSE_COMIS_RR)                                     \
  IIC(SSE_COMIS_RM)                                     \
                                                        \
  IIC(SSE_HADDSUB_RR)                                   \
  IIC(SSE_HADDSUB_RM)                                   \
                                                        \
  IIC(SSE_BIT_P_RR )                                    \
  IIC(SSE_BIT_P_RM )                                    \
                                                        \
  IIC(SSE_INTALU_P_RR )                                 \
  IIC(SSE_INTALU_P_RM )                                 \
  IIC(SSE_INTALUQ_P_RR )                                \
  IIC(SSE_INTALUQ_P_RM )                                \
                                                        \
  IIC(SSE_INTMUL_P_RR)                                  \
  IIC(SSE_INTMUL_P_RM)                                  \
                                                        \
  IIC(SSE_INTSH_P_RR)                                   \
  IIC(SSE_INTSH_P_RM)                                   \
  IIC(SSE_INTSH_P_RI)                                   \
                                                        \
  IIC(SSE_CMPP_RR)                                      \
  IIC(SSE_CMPP_RM)                                      \
                                                        \
  IIC(SSE_SHUFP)                                        \
  IIC(SSE_PSHUF)                                        \
                                                        \
  IIC(SSE_UNPCK)                                        \
                                                        \
  IIC(SSE_MOVMSK)                                       \
  IIC(SSE_MASKMOV)                                      \
                                                        \
  IIC(SSE_PEXTRW)                                       \
  IIC(SSE_PINSRW)                                       \
                                                        \
  IIC(SSE_PABS_RR)                                      \
  IIC(SSE_PABS_RM)                                      \
                                                        \
  IIC(SSE_SQRTP_RR)                                     \
  IIC(SSE_SQRTP_RM)                                     \
  IIC(SSE_SQRTS_RR)                                     \
  IIC(SSE_SQRTS_RM)                                     \
                                                        \
  IIC(SSE_RCPP_RR)                                      \
  IIC(SSE_RCPP_RM)                                      \
  IIC(SSE_RCPS_RR)                                      \
  IIC(SSE_RCPS_RM)                                      \
                                                        \
  IIC(SSE_MOV_S_RR)                                     \
  IIC(SSE_MOV_S_RM)                                     \
  IIC(SSE_MOV_S_MR)                                     \
                                                        \
  IIC(SSE_MOVA_P_RR)                                    \
  IIC(SSE_MOVA_P_RM)                                    \
  IIC(SSE_MOVA_P_MR)                                    \
                                                        \
  IIC(SSE_MOVU_P_RR)                                    \
  IIC(SSE_MOVU_P_RM)                                    \
  IIC(SSE_MOVU_P_MR)                                    \
                                                        \
  IIC(SSE_MOVDQ)                                        \
  IIC(SSE_MOVD_ToGP)                                    \
  IIC(SSE_MOVQ_RR)                                      \
                                                        \
  IIC(SSE_MOV_LH)                                       \
                                                        \
  IIC(SSE_LDDQU)                                        \
                                                        \
  IIC(SSE_MOVNT)                                        \
                                                        \
  IIC(SSE_PHADDSUBD_RR)                                 \
  IIC(SSE_PHADDSUBD_RM)                                 \
  IIC(SSE_PHADDSUBSW_RR)                                \
  IIC(SSE_PHADDSUBSW_RM)                                \
  IIC(SSE_PHADDSUBW_RR)                                 \
  IIC(SSE_PHADDSUBW_RM)                                 \
  IIC(SSE_PSHUFB_RR)                                    \
  IIC(SSE_PSHUFB_RM)                                    \
  IIC(SSE_PSIGN_RR)                                     \
  IIC(SSE_PSIGN_RM)                                     \
                                                        \
  IIC(SSE_PMADD)                                        \
  IIC(SSE_PMULHRSW)                                     \
  IIC(SSE_PALIGNR)                                      \
  IIC(SSE_MWAIT)                                        \
  IIC(SSE_MONITOR)                                      \
                                                        \
  IIC(SSE_PREFETCH)                                     \
  IIC(SSE_PAUSE)                                        \
  IIC(SSE_LFENCE)                                       \
  IIC(SSE_MFENCE)                                       \
  IIC(SSE_SFENCE)                                       \
  IIC(SSE_LDMXCSR)                                      \
  IIC(SSE_STMXCSR)                                      \
                                                        \
  IIC(SSE_CVT_PD_RR)                                    \
  IIC(SSE_CVT_PD_RM)                                    \
  IIC(SSE_CVT_PS_RR)                                    \
  IIC(SSE_CVT_PS_RM)                                    \
  IIC(SSE_CVT_PI2PS_RR)                                 \
  IIC(SSE_CVT_PI2PS_RM)                                 \
  IIC(SSE_CVT_Scalar_RR)                                \
  IIC(SSE_CVT_Scalar_RM)                                \
  IIC(SSE_CVT_SS2SI32_RM)                               \
  IIC(SSE_CVT_SS2SI32_RR)                               \
  IIC(SSE_CVT_SS2SI64_RM)                               \
  IIC(SSE_CVT_SS2SI64_RR)                               \
  IIC(SSE_CVT_SD2SI_RM)                                 \
  IIC(SSE_CVT_SD2SI_RR)                                 \
                                                        \
  CHANGE_CAT(MMX)                                       \
  IIC(MMX_MOV_MM_RM)                                    \
  IIC(MMX_MOV_REG_MM)                                   \
  IIC(MMX_MOVQ_RM)                                      \
  IIC(MMX_MOVQ_RR)                                      \
                                                        \
  IIC(MMX_ALU_RM)                                       \
  IIC(MMX_ALU_RR)                                       \
  IIC(MMX_ALUQ_RM)                                      \
  IIC(MMX_ALUQ_RR)                                      \
  IIC(MMX_PHADDSUBW_RM)                                 \
  IIC(MMX_PHADDSUBW_RR)                                 \
  IIC(MMX_PHADDSUBD_RM)                                 \
  IIC(MMX_PHADDSUBD_RR)                                 \
  IIC(MMX_PMUL)                                         \
  IIC(MMX_MISC_FUNC_MEM)                                \
  IIC(MMX_MISC_FUNC_REG)                                \
  IIC(MMX_PSADBW)                                       \
  IIC(MMX_SHIFT_RI)                                     \
  IIC(MMX_SHIFT_RM)                                     \
  IIC(MMX_SHIFT_RR)                                     \
  IIC(MMX_UNPCK_H_RM)                                   \
  IIC(MMX_UNPCK_H_RR)                                   \
  IIC(MMX_UNPCK_L)                                      \
  IIC(MMX_PCK_RM)                                       \
  IIC(MMX_PCK_RR)                                       \
  IIC(MMX_PSHUF)                                        \
  IIC(MMX_PEXTR)                                        \
  IIC(MMX_PINSRW)                                       \
  IIC(MMX_MASKMOV)                                      \
                                                        \
  IIC(MMX_CVT_PD_RR)                                    \
  IIC(MMX_CVT_PD_RM)                                    \
  IIC(MMX_CVT_PS_RR)                                    \
  IIC(MMX_CVT_PS_RM)                                    \
                                                        \
  IIC(CMPX_LOCK)                                        \
  IIC(CMPX_LOCK_8)                                      \
  IIC(CMPX_LOCK_8B)                                     \
  IIC(CMPX_LOCK_16B)                                    \
                                                        \
  IIC(XADD_LOCK_MEM)                                    \
  IIC(XADD_LOCK_MEM8)                                   \
                                                        \
  IIC(FILD)                                             \
  IIC(FLD)                                              \
  IIC(FLD80)                                            \
  IIC(FST)                                              \
  IIC(FST80)                                            \
  IIC(FIST)                                             \
  IIC(FLDZ)                                             \
  IIC(FUCOM)                                            \
  IIC(FUCOMI)                                           \
  IIC(FCOMI)                                            \
  IIC(FNSTSW)                                           \
  IIC(FNSTCW)                                           \
  IIC(FLDCW)                                            \
  IIC(FNINIT)                                           \
  IIC(FFREE)                                            \
  IIC(FNCLEX)                                           \
  IIC(WAIT)                                             \
  IIC(FXAM)                                             \
  IIC(FNOP)                                             \
  IIC(FLDL)                                             \
  IIC(F2XM1)                                            \
  IIC(FYL2X)                                            \
  IIC(FPTAN)                                            \
  IIC(FPATAN)                                           \
  IIC(FXTRACT)                                          \
  IIC(FPREM1)                                           \
  IIC(FPSTP)                                            \
  IIC(FPREM)                                            \
  IIC(FYL2XP1)                                          \
  IIC(FSINCOS)                                          \
  IIC(FRNDINT)                                          \
  IIC(FSCALE)                                           \
  IIC(FCOMPP)                                           \
  IIC(FXSAVE)                                           \
  IIC(FXRSTOR)                                          \
                                                        \
  IIC(FXCH)                                             \
                                                        \
  CHANGE_CAT(SYSTEM)                                    \
  IIC(CPUID)                                            \
  IIC(INT)                                              \
  IIC(INT3)                                             \
  IIC(INVD)                                             \
  IIC(INVLPG)                                           \
  IIC(IRET)                                             \
  IIC(HLT)                                              \
  IIC(LXS)                                              \
  IIC(LTR)                                              \
  IIC(RDTSC)                                            \
  IIC(RSM)                                              \
  IIC(SIDT)                                             \
  IIC(SGDT)                                             \
  IIC(SLDT)                                             \
  IIC(STR)                                              \
  IIC(SWAPGS)                                           \
  IIC(SYSCALL)                                          \
  IIC(SYS_ENTER_EXIT)                                   \
  IIC(IN_RR)                                            \
  IIC(IN_RI)                                            \
  IIC(OUT_RR)                                           \
  IIC(OUT_IR)                                           \
  IIC(INS)                                              \
  IIC(MOV_REG_DR)                                       \
  IIC(MOV_DR_REG)                                       \
  IIC(MOV_REG_CR)                                       \
  IIC(MOV_CR_REG)                                       \
  IIC(MOV_REG_SR)                                       \
  IIC(MOV_MEM_SR)                                       \
  IIC(MOV_SR_REG)                                       \
  IIC(MOV_SR_MEM)                                       \
  IIC(LAR_RM)                                           \
  IIC(LAR_RR)                                           \
  IIC(LSL_RM)                                           \
  IIC(LSL_RR)                                           \
  IIC(LGDT)                                             \
  IIC(LIDT)                                             \
  IIC(LLDT_REG)                                         \
  IIC(LLDT_MEM)                                         \
  IIC(PUSH_CS)                                          \
  IIC(PUSH_SR)                                          \
  IIC(POP_SR)                                           \
  IIC(POP_SR_SS)                                        \
  IIC(VERR)                                             \
  IIC(VERW_REG)                                         \
  IIC(VERW_MEM)                                         \
  IIC(WRMSR)                                            \
  IIC(RDMSR)                                            \
  IIC(RDPMC)                                            \
  IIC(SMSW)                                             \
  IIC(LMSW_REG)                                         \
  IIC(LMSW_MEM)                                         \
  IIC(ENTER)                                            \
  IIC(LEAVE)                                            \
  IIC(POP_MEM)                                          \
  IIC(POP_REG16)                                        \
  IIC(POP_REG)                                          \
  IIC(POP_F)                                            \
  IIC(POP_FD)                                           \
  IIC(POP_A)                                            \
  IIC(PUSH_IMM)                                         \
  IIC(PUSH_MEM)                                         \
  IIC(PUSH_REG)                                         \
  IIC(PUSH_F)                                           \
  IIC(PUSH_A)                                           \
  IIC(BSWAP)                                            \
  IIC(BSF)                                              \
  IIC(BSR)                                              \
  IIC(MOVS)                                             \
  IIC(STOS)                                             \
  IIC(SCAS)                                             \
  IIC(CMPS)                                             \
  IIC(MOV)                                              \
  IIC(MOV_MEM)                                          \
  IIC(AHF)                                              \
  IIC(BT_MI)                                            \
  IIC(BT_MR)                                            \
  IIC(BT_RI)                                            \
  IIC(BT_RR)                                            \
  IIC(BTX_MI)                                           \
  IIC(BTX_MR)                                           \
  IIC(BTX_RI)                                           \
  IIC(BTX_RR)                                           \
  IIC(XCHG_REG)                                         \
  IIC(XCHG_MEM)                                         \
  IIC(XADD_REG)                                         \
  IIC(XADD_MEM)                                         \
  IIC(CMPXCHG_MEM)                                      \
  IIC(CMPXCHG_REG)                                      \
  IIC(CMPXCHG_MEM8)                                     \
  IIC(CMPXCHG_REG8)                                     \
  IIC(CMPXCHG_8B)                                       \
  IIC(CMPXCHG_16B)                                      \
  IIC(LODS)                                             \
  IIC(OUTS)                                             \
  IIC(CLC)                                              \
  IIC(CLD)                                              \
  IIC(CLI)                                              \
  IIC(CMC)                                              \
  IIC(CLTS)                                             \
  IIC(STC)                                              \
  IIC(STI)                                              \
  IIC(STD)                                              \
  IIC(XLAT)                                             \
  IIC(AAA)                                              \
  IIC(AAD)                                              \
  IIC(AAM)                                              \
  IIC(AAS)                                              \
  IIC(DAA)                                              \
  IIC(DAS)                                              \
  IIC(BOUND)                                            \
  IIC(ARPL_REG)                                         \
  IIC(ARPL_MEM)                                         \
  IIC(MOVBE)

#endif // __PROCESSOR_IIC_CATEGORIES__HPP
