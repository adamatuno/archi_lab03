comp = diff
case1 = my
case2 = example2
case3 = example3

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
	./CMP
.PHONY: run
run: *.o
	gcc *.o -o CMP -O3
	cp $(testcases)/$(case1)/*.bin ../simulator
.PHONY: runcomp
runcomp: *.o
	gcc *.o -o CMP -O3
	cp $(testcases)/$(case1)/*.bin ../simulator
	./CMP
	$(comp) snapshot.rpt $(testcases)/$(case1)/snapshot.rpt
	$(comp) report.rpt $(testcases)/$(case1)/report.rpt
