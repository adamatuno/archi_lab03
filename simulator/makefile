comp = fc
case1 = example3
case2 = 42
case3 = 43
case4 = 44
case5 = 45
case6 = 46
case7 = 47
case8 = 48
case9 = 49
case10 = 10
case11 = 20
case12 = 30
case13 = 40
case14 = my
testcases = ../testcases
CMP: *.o
	gcc *.o -o CMP -O3
*.o: *.c
	gcc -c *.c
.PHONY: clean
clean:
	rm -f *.o *.bin *.rpt
.PHONY: decode
decode:
	cp $(testcases)/$(case1)/*.bin ../decoder
.PHONY: testcase
testcase: *.o
	gcc *.o -o CMP -O3
	cp ../testcase/*.bin ../simulator
	CMP.exe
.PHONY: run
run: *.o
	gcc *.o -o CMP -O3
	cp $(testcases)/$(case1)/*.bin ../simulator
.PHONY: runcomp
runcomp: *.o
	gcc *.o -o CMP -O3
	cp $(testcases)/$(case1)/*.bin ../simulator
	$(comp) snapshot.rpt $(testcases)/$(case1)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case1)/report.rpt
.PHONY: test2
test2: *.o
	gcc *.o -o CMP -O3
	cp $(testcases)/$(case2)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case2)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case2)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case3)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case3)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case3)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case4)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case4)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case4)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case5)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case5)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case5)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case6)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case6)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case6)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case7)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case7)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case7)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case8)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case8)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case8)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case9)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case9)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case9)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case10)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case10)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case10)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case11)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case11)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case11)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case12)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case12)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case12)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case13)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case13)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case13)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case14)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case14)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case14)/report.rpt
	rm -f *.bin *.rpt
	cp $(testcases)/$(case1)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case1)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case1)/report.rpt
	rm -f *.bin *.rpt