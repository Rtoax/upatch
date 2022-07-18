#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>

#include <elf/elf_api.h>
#include <utils/util.h>
#include <utils/log.h>


#if defined(__x86_64__)
static const char *R_X86_64_STRING[R_X86_64_NUM] = {
#define _I(v) [v] = #v
	_I(R_X86_64_NONE),
	_I(R_X86_64_64),
	_I(R_X86_64_PC32),
	_I(R_X86_64_GOT32),
	_I(R_X86_64_PLT32),
	_I(R_X86_64_COPY),
	_I(R_X86_64_GLOB_DAT),
	_I(R_X86_64_JUMP_SLOT),
	_I(R_X86_64_RELATIVE),
	_I(R_X86_64_GOTPCREL),
	_I(R_X86_64_32),
	_I(R_X86_64_32S),
	_I(R_X86_64_16),
	_I(R_X86_64_PC16),
	_I(R_X86_64_8),
	_I(R_X86_64_PC8),
	_I(R_X86_64_DTPMOD64),
	_I(R_X86_64_DTPOFF64),
	_I(R_X86_64_TPOFF64),
	_I(R_X86_64_TLSGD),
	_I(R_X86_64_TLSLD),
	_I(R_X86_64_DTPOFF32),
	_I(R_X86_64_GOTTPOFF),
	_I(R_X86_64_TPOFF32),
	_I(R_X86_64_PC64),
	_I(R_X86_64_GOTOFF64),
	_I(R_X86_64_GOTPC32),
	_I(R_X86_64_GOT64),
	_I(R_X86_64_GOTPCREL64),
	_I(R_X86_64_GOTPC64),
	_I(R_X86_64_GOTPLT64),
	_I(R_X86_64_PLTOFF64),
	_I(R_X86_64_SIZE32),
	_I(R_X86_64_SIZE64),
	_I(R_X86_64_GOTPC32_TLSDESC),
	_I(R_X86_64_TLSDESC_CALL),
	_I(R_X86_64_TLSDESC),
	_I(R_X86_64_IRELATIVE),
	_I(R_X86_64_RELATIVE64),
	_I(R_X86_64_GOTPCRELX),
	_I(R_X86_64_REX_GOTPCRELX),
#undef _I
};

static const char *r_x86_64_name(int r)
{
	return R_X86_64_STRING[r];
}
#elif defined(__aarch64__)
static const char *R_AARCH64_STRING[R_X86_64_NUM] = {
#define _I(v) [v] = #v
	_I(R_AARCH64_NONE)
	_I(R_AARCH64_P32_ABS32)
	_I(R_AARCH64_P32_COPY)
	_I(R_AARCH64_P32_GLOB_DAT)
	_I(R_AARCH64_P32_JUMP_SLOT)
	_I(R_AARCH64_P32_RELATIVE)
	_I(R_AARCH64_P32_TLS_DTPMOD)
	_I(R_AARCH64_P32_TLS_DTPREL)
	_I(R_AARCH64_P32_TLS_TPREL)
	_I(R_AARCH64_P32_TLSDESC)
	_I(R_AARCH64_P32_IRELATIVE)
	_I(R_AARCH64_ABS64)
	_I(R_AARCH64_ABS32)
	_I(R_AARCH64_ABS16)
	_I(R_AARCH64_PREL64)
	_I(R_AARCH64_PREL32)
	_I(R_AARCH64_PREL16)
	_I(R_AARCH64_MOVW_UABS_G0)
	_I(R_AARCH64_MOVW_UABS_G0_NC)
	_I(R_AARCH64_MOVW_UABS_G1)
	_I(R_AARCH64_MOVW_UABS_G1_NC)
	_I(R_AARCH64_MOVW_UABS_G2)
	_I(R_AARCH64_MOVW_UABS_G2_NC)
	_I(R_AARCH64_MOVW_UABS_G3)
	_I(R_AARCH64_MOVW_SABS_G0)
	_I(R_AARCH64_MOVW_SABS_G1)
	_I(R_AARCH64_MOVW_SABS_G2)
	_I(R_AARCH64_LD_PREL_LO19)
	_I(R_AARCH64_ADR_PREL_LO21)
	_I(R_AARCH64_ADR_PREL_PG_HI21)
	_I(R_AARCH64_ADR_PREL_PG_HI21_NC)
	_I(R_AARCH64_ADD_ABS_LO12_NC)
	_I(R_AARCH64_LDST8_ABS_LO12_NC)
	_I(R_AARCH64_TSTBR14)
	_I(R_AARCH64_CONDBR19)
	_I(R_AARCH64_JUMP26)
	_I(R_AARCH64_CALL26)
	_I(R_AARCH64_LDST16_ABS_LO12_NC)
	_I(R_AARCH64_LDST32_ABS_LO12_NC)
	_I(R_AARCH64_LDST64_ABS_LO12_NC)
	_I(R_AARCH64_MOVW_PREL_G0)
	_I(R_AARCH64_MOVW_PREL_G0_NC)
	_I(R_AARCH64_MOVW_PREL_G1)
	_I(R_AARCH64_MOVW_PREL_G1_NC)
	_I(R_AARCH64_MOVW_PREL_G2)
	_I(R_AARCH64_MOVW_PREL_G2_NC)
	_I(R_AARCH64_MOVW_PREL_G3)
	_I(R_AARCH64_LDST128_ABS_LO12_NC)
	_I(R_AARCH64_MOVW_GOTOFF_G0)
	_I(R_AARCH64_MOVW_GOTOFF_G0_NC)
	_I(R_AARCH64_MOVW_GOTOFF_G1)
	_I(R_AARCH64_MOVW_GOTOFF_G1_NC)
	_I(R_AARCH64_MOVW_GOTOFF_G2)
	_I(R_AARCH64_MOVW_GOTOFF_G2_NC)
	_I(R_AARCH64_MOVW_GOTOFF_G3)
	_I(R_AARCH64_GOTREL64)
	_I(R_AARCH64_GOTREL32)
	_I(R_AARCH64_GOT_LD_PREL19)
	_I(R_AARCH64_LD64_GOTOFF_LO15)
	_I(R_AARCH64_ADR_GOT_PAGE)
	_I(R_AARCH64_LD64_GOT_LO12_NC)
	_I(R_AARCH64_LD64_GOTPAGE_LO15)
	_I(R_AARCH64_TLSGD_ADR_PREL21)
	_I(R_AARCH64_TLSGD_ADR_PAGE21)
	_I(R_AARCH64_TLSGD_ADD_LO12_NC)
	_I(R_AARCH64_TLSGD_MOVW_G1)
	_I(R_AARCH64_TLSGD_MOVW_G0_NC)
	_I(R_AARCH64_TLSLD_ADR_PREL21)
	_I(R_AARCH64_TLSLD_ADR_PAGE21)
	_I(R_AARCH64_TLSLD_ADD_LO12_NC)
	_I(R_AARCH64_TLSLD_MOVW_G1)
	_I(R_AARCH64_TLSLD_MOVW_G0_NC)
	_I(R_AARCH64_TLSLD_LD_PREL19)
	_I(R_AARCH64_TLSLD_MOVW_DTPREL_G2)
	_I(R_AARCH64_TLSLD_MOVW_DTPREL_G1)
	_I(R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC)
	_I(R_AARCH64_TLSLD_MOVW_DTPREL_G0)
	_I(R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC)
	_I(R_AARCH64_TLSLD_ADD_DTPREL_HI12)
	_I(R_AARCH64_TLSLD_ADD_DTPREL_LO12)
	_I(R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC)
	_I(R_AARCH64_TLSLD_LDST8_DTPREL_LO12)
	_I(R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC)
	_I(R_AARCH64_TLSLD_LDST16_DTPREL_LO12)
	_I(R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC)
	_I(R_AARCH64_TLSLD_LDST32_DTPREL_LO12)
	_I(R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC)
	_I(R_AARCH64_TLSLD_LDST64_DTPREL_LO12)
	_I(R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC)
	_I(R_AARCH64_TLSIE_MOVW_GOTTPREL_G1)
	_I(R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC)
	_I(R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21)
	_I(R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC)
	_I(R_AARCH64_TLSIE_LD_GOTTPREL_PREL19)
	_I(R_AARCH64_TLSLE_MOVW_TPREL_G2)
	_I(R_AARCH64_TLSLE_MOVW_TPREL_G1)
	_I(R_AARCH64_TLSLE_MOVW_TPREL_G1_NC)
	_I(R_AARCH64_TLSLE_MOVW_TPREL_G0)
	_I(R_AARCH64_TLSLE_MOVW_TPREL_G0_NC)
	_I(R_AARCH64_TLSLE_ADD_TPREL_HI12)
	_I(R_AARCH64_TLSLE_ADD_TPREL_LO12)
	_I(R_AARCH64_TLSLE_ADD_TPREL_LO12_NC)
	_I(R_AARCH64_TLSLE_LDST8_TPREL_LO12)
	_I(R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC)
	_I(R_AARCH64_TLSLE_LDST16_TPREL_LO12)
	_I(R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC)
	_I(R_AARCH64_TLSLE_LDST32_TPREL_LO12)
	_I(R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC)
	_I(R_AARCH64_TLSLE_LDST64_TPREL_LO12)
	_I(R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC)
	_I(R_AARCH64_TLSDESC_LD_PREL19)
	_I(R_AARCH64_TLSDESC_ADR_PREL21)
	_I(R_AARCH64_TLSDESC_ADR_PAGE21)
	_I(R_AARCH64_TLSDESC_LD64_LO12)
	_I(R_AARCH64_TLSDESC_ADD_LO12)
	_I(R_AARCH64_TLSDESC_OFF_G1)
	_I(R_AARCH64_TLSDESC_OFF_G0_NC)
	_I(R_AARCH64_TLSDESC_LDR)
	_I(R_AARCH64_TLSDESC_ADD)
	_I(R_AARCH64_TLSDESC_CALL)
	_I(R_AARCH64_TLSLE_LDST128_TPREL_LO12)
	_I(R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC)
	_I(R_AARCH64_TLSLD_LDST128_DTPREL_LO12)
	_I(R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC)
	_I(R_AARCH64_COPY)
	_I(R_AARCH64_GLOB_DAT)
	_I(R_AARCH64_JUMP_SLOT)
	_I(R_AARCH64_RELATIVE)
	_I(R_AARCH64_TLS_DTPMOD)
	_I(R_AARCH64_TLS_DTPREL)
	_I(R_AARCH64_TLS_TPREL)
	_I(R_AARCH64_TLSDESC)
	_I(R_AARCH64_IRELATIVE)
#undef _I
};

static const char *r_aarch64_name(int r)
{
	return R_AARCH64_STRING[r];
}
#endif

const char *rela_type_string(int r)
{
#if defined(__x86_64__)
	return r_x86_64_name(r);
#elif defined(__aarch64__)
	return r_aarch64_name(r);
#endif
}

void print_rela(GElf_Rela *rela)
{
	printf("%016lx %16ld %16ld %16ld\n",
		rela->r_offset, rela->r_info,
		GELF_R_TYPE(rela->r_info), rela->r_addend);
}
