comp = fc
case1 = 32
case2 = example2
case3 = example3

arg2 = 256 256 32 32 16 4 4 16 4 4
arg3 = 512 1024 128 64 64 4 8 32 4 4

testcases = ../valid
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
	./CMP
.PHONY: run
run: *.o
	gcc *.o -o CMP -O3
	cp $(testcases)/$(case1)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case1)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case1)/report1.rpt
	./CMP ${arg2}
	$(comp) snapshot.rpt $(testcases)/$(case1)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case1)/report2.rpt
	./CMP ${arg3}
	$(comp) snapshot.rpt $(testcases)/$(case1)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case1)/report3.rpt
.PHONY: comp1
comp1: *.o
	gcc *.o -o CMP -O3
	cp $(testcases)/$(case1)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case1)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case1)/report1.rpt
.PHONY: comp2
comp2: *.o
	gcc *.o -o CMP -O3
	cp $(testcases)/$(case1)/*.bin ../simulator
	./CMP ${arg2}
	$(comp) snapshot.rpt $(testcases)/$(case1)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case1)/report2.rpt
.PHONY: comp3
comp3: *.o
	gcc *.o -o CMP -O3
	cp $(testcases)/$(case1)/*.bin ../simulator
	./CMP ${arg3}
	$(comp) snapshot.rpt $(testcases)/$(case1)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case1)/report3.rpt
.PHONY: trace
trace: *.o
	gcc *.o -o CMP -O3
	cp $(testcases)/$(case1)/*.bin ../simulator
	./CMP ${arg3}
	$(comp) trace.rpt $(testcases)/$(case1)/trace.rpt
