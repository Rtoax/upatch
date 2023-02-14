#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <bfd.h>
#include <dis-asm.h>


#define __unused __attribute__((unused))


static asymbol **syms;
static long symcount = 0;

static asymbol **dynsyms;
static long dynsymcount = 0;

static asymbol *synthsyms;
static long synthcount = 0;

static long sorted_symcount = 0;
static asymbol **sorted_syms;



static asymbol **slurp_symtab(bfd *abfd)
{
	symcount = 0;
	if (!(bfd_get_file_flags(abfd) & HAS_SYMS))
		return NULL;

	long storage = bfd_get_symtab_upper_bound(abfd);
	if (storage < 0) {
		fprintf(stderr, "failed to read symbol table from: %s",
			bfd_get_filename (abfd));
		fprintf(stderr, "error message was");
		abort();
	}

	if (storage == 0)
		return NULL;

	asymbol **sy = (asymbol **) malloc(storage);
	symcount = bfd_canonicalize_symtab(abfd, sy);
	if (symcount < 0)
		fprintf(stderr, bfd_get_filename(abfd));

	return sy;
}

static asymbol **slurp_dynamic_symtab(bfd *abfd)
{
	dynsymcount = 0;
	long storage = bfd_get_dynamic_symtab_upper_bound(abfd);
	if (storage < 0) {
		if (!(bfd_get_file_flags (abfd) & DYNAMIC)) {
			fprintf(stderr, "%s: not a dynamic object", bfd_get_filename(abfd));
			return NULL;
		}

		fprintf(stderr, bfd_get_filename(abfd));
		abort();
	}

	if (storage == 0)
		return NULL;

	asymbol **sy = (asymbol **) malloc(storage);
	dynsymcount = bfd_canonicalize_dynamic_symtab(abfd, sy);
	if (dynsymcount < 0) {
		fprintf(stderr, bfd_get_filename(abfd));
		abort();
	}

	return sy;
}

static bool is_significant_symbol_name(const char * name)
{
	return startswith(name, ".plt") || startswith(name, ".got");
}

static long remove_useless_symbols(asymbol **symbols, long count)
{
	asymbol **in_ptr = symbols, **out_ptr = symbols;

	while (--count >= 0) {
		asymbol *sym = *in_ptr++;

		if (sym->name == NULL || sym->name[0] == '\0')
			continue;
		if ((sym->flags & (BSF_DEBUGGING | BSF_SECTION_SYM))
			&& ! is_significant_symbol_name(sym->name))
			continue;
		if (bfd_is_und_section(sym->section)
			|| bfd_is_com_section(sym->section))
			continue;

		*out_ptr++ = sym;
	}
	return out_ptr - symbols;
}

static void disassemble_data(bfd *abfd)
{
	int i;

	sorted_symcount = symcount ? symcount : dynsymcount;
	sorted_syms = (asymbol **) malloc ((sorted_symcount + synthcount)
		* sizeof (asymbol *));

	if (sorted_symcount != 0) {
		memcpy(sorted_syms, symcount ? syms : dynsyms,
			sorted_symcount * sizeof (asymbol *));

		sorted_symcount = remove_useless_symbols(sorted_syms, sorted_symcount);
	}

	for (i = 0; i < synthcount; ++i) {
		sorted_syms[sorted_symcount] = synthsyms + i;
		++sorted_symcount;
	}

	for (i = 0; i < sorted_symcount; i++) {
		asymbol *s = sorted_syms[i];
		printf("SYM: %#016lx  %s\n", bfd_asymbol_value(s), s->name);
	}

	free(sorted_syms);
}

static void dump_bfd(bfd *abfd, bool is_mainfile)
{
	syms = slurp_symtab(abfd);

	if (is_mainfile) {
		dynsyms = slurp_dynamic_symtab(abfd);
	}

	synthcount = bfd_get_synthetic_symtab(abfd, symcount, syms,
					dynsymcount, dynsyms,
					&synthsyms);
	if (synthcount < 0)
		synthcount = 0;

	disassemble_data(abfd);

	if (syms) {
		free(syms);
		syms = NULL;
	}
	if (dynsyms) {
		free(dynsyms);
		dynsyms = NULL;
	}

	if (synthsyms) {
		free(synthsyms);
		synthsyms = NULL;
	}
	symcount = 0;
	dynsymcount = 0;
	synthcount = 0;
}

int main(int argc, char *argv[])
{
	bfd *file;
	char **matching;

	char *filename = argv[0];
	char *target = NULL;

	file = bfd_openr(filename, target);

	if (bfd_check_format(file, bfd_archive)) {
		printf("%s is bfd archive, do nothing, close\n", filename);
		goto close;
	}

	if (bfd_check_format_matches(file, bfd_object, &matching)) {
		printf("%s is bfd_object\n", filename);
		dump_bfd(file, true);
	}

close:
	bfd_close(file);

	return 0;
}

