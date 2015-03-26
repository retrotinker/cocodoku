CARTLOAD=0xc000
TARGETS=libbcc.a cocodoku.rom
EXTRA=cocodoku.aout

all: $(TARGETS)

.s.o:
	as09 -u -o $@ $<

.c.o:
	bcc09 -Me -o $@ -c $<

libbcc.a: imul.o isl.o isr.o isru.o imod.o imodu.o idiv.o idivu.o \
		_memcpy.o _memset.o
	rm -f $@
	ar09 cr $@ $^

cocodoku.aout: crtrom.o solver.o screen.o cocochr.o tink.o
	ld09 -o $@ -T$(CARTLOAD) $^ -lbcc

cocodoku.rom: cocodoku.aout
	objcopy09 -O binary $< $@

cocodoku.4k: cocodoku.rom
	rm -f $@
	dd if=/dev/zero bs=4k count=1 | \
		tr '\000' '\377' > $@
	dd if=$< of=$@ conv=notrunc

cocodoku.8k: cocodoku.4k
	rm -f $@
	cat $< > $@
	cat $< >> $@

cocodoku.16k: cocodoku.8k
	rm -f $@
	cat $< > $@
	cat $< >> $@

cocodoku.32k: cocodoku.16k
	rm -f $@
	cat $< > $@
	cat $< >> $@

clean:
	rm -f *.o *.bin *.rom *.s19 *.4k *.8k *.16k *.32k $(TARGETS) $(EXTRA)
