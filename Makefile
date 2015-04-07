CARTLOAD=0xc000
TARGETS=libbcc.a crtrom.o cocodoku.ccc
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

cocodoku.aout: solver.o screen.o cocochr.o audio.o
	ld09 -o $@ -T$(CARTLOAD) -Crom -lbcc $^

cocodoku.ccc: cocodoku.aout
	objcopy09 -O binary $< $@

cocodoku.4k: cocodoku.ccc
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
	rm -f *.o *.aout *.bin *.ccc *.s19 *.4k *.8k *.16k *.32k $(TARGETS) $(EXTRA)
