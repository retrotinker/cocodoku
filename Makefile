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

solver.4k: solver.rom
	rm -f $@
	dd if=/dev/zero bs=4k count=1 | \
		tr '\000' '\377' > $@
	dd if=$< of=$@ conv=notrunc

solver.8k: solver.4k
	rm -f $@
	cat $< > $@
	cat $< >> $@

solver.16k: solver.8k
	rm -f $@
	cat $< > $@
	cat $< >> $@

solver.32k: solver.16k
	rm -f $@
	cat $< > $@
	cat $< >> $@

clean:
	rm -f *.o *.bin *.rom *.s19 *.4k *.8k *.16k *.32k $(TARGETS) $(EXTRA)
