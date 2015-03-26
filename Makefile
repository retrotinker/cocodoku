CARTLOAD=0xc000
TARGETS=libbcc.a solver.rom 
EXTRA=solver.aout

all: $(TARGETS)

.s.o:
	as09 -u -o $@ $<

.c.o:
	bcc09 -Me -o $@ -c $<

libbcc.a: imul.o isl.o isr.o isru.o imod.o imodu.o idiv.o idivu.o \
		_memcpy.o _memset.o
	rm -f $@
	ar09 cr $@ $^

solver.aout: crtrom.o solver.o screen.o cocochr.o
	ld09 -o $@ -T$(CARTLOAD) $^ -lbcc

solver.rom: solver.aout
	objcopy09 -O binary $< $@

clean:
	rm -f *.o *.bin *.rom *.s19 $(TARGETS) $(EXTRA)
