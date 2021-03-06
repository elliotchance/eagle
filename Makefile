SHELL := /bin/bash
OBJROOT = $(shell xcodebuild -project eagle.xcodeproj -target eagle_test -showBuildSettings | grep OBJROOT | cut -c15-)
GIT_BRANCH = $(shell git branch | grep \* | cut -c3-)

# these are used by the `leaks` target
OBJS = $(OBJROOT)/eagle.build/Debug/eagle.build/Objects-normal/x86_64
NM = nm $(OBJS)/*.o -o 2>&1 | grep -v EagleMemory.o | grep -w '_malloc\|_calloc\|_free'

all: clean build

release: clean build build_analyze operator-tests cli-tests leaks coverage doxygen

clean: clean_eagle clean_eagle_test
	
clean_eagle:
	xcodebuild -project eagle.xcodeproj -target eagle clean
	
clean_eagle_test:
	xcodebuild -project eagle.xcodeproj -target eagle_test clean
	
build: build_eagle build_eagle_test

build_analyze:
	rm -rf analyze
	rm -rf build/eagle.build/Release/eagle.build/StaticAnalyzer
	mkdir analyze
	xcodebuild -project eagle.xcodeproj -configuration Release RUN_CLANG_STATIC_ANALYZER=YES CLANG_ANALYZER_OUTPUT=html -target eagle build
	if [ `find build/eagle.build/Release/eagle.build/StaticAnalyzer -name *.html | wc -l` -gt 0 ]; then \
		cp `ls -1 build/eagle.build/Release/eagle.build/StaticAnalyzer/normal/x86_64/*.plist/*.html` analyze; \
	fi
	if [ `ls -1 analyze | wc -l` -gt 0 ]; then \
		echo "There are analyzer errors!"; \
		find `pwd`/analyze -name *.html; \
		exit 1; \
	fi

build_eagle:
	xcodebuild -project eagle.xcodeproj -configuration Release -target eagle build

build_eagle_test: operator-tests
	xcodebuild -project eagle.xcodeproj -configuration Debug -target eagle_test build

bench: build_eagle_test
	build/Debug/eagle_test bench
	
# test for leaks
leaks: build_eagle_test
	# check for stdlib memory functions
	if [ `$(NM) | wc -l` -gt 0 ]; then \
		echo "\n==> Do not use stdlib memory functions, use EagleMemory functions instead. <=="; \
		$(NM); \
		exit 1; \
	fi
	
	# make sure all allocations have tests
	grep -w "EagleMemory_Allocate\|EagleMemory_MultiAllocate" -r eagle | perl -nle 'm/"(.*)"/; print $$1' | sort | uniq > leaks.alloc.tmp
	grep "EagleMemory_Mock(" -r eagle_test | perl -nle 'm/"(.*)"/; print $$1' | sort | uniq > leaks.alloc_test.tmp
	if [ `grep -f leaks.alloc_test.tmp -v leaks.alloc.tmp | wc -l` -gt 0 ]; then \
		echo "\n==> The following allocations do not have MemorySuite tests: <=="; \
		grep -f leaks.alloc_test.tmp -v leaks.alloc.tmp; \
		exit 1; \
	fi

	- killall eagle_test
	MallocStackLogging=1 build/Debug/eagle_test --wait --all-suites --exclude-bench &
	sleep 10
	#malloc_history eagle_test -callTree -invert -collapseRecursion -chargeSystemLibraries
	leaks eagle_test
	killall eagle_test

test: clean_eagle_test build_eagle_test operator-tests cli-tests
	build/Debug/eagle_test --all-suites
	
coverage: clean_eagle_test build_eagle_test
	- rm coverage.info
	build/Debug/eagle_test --all-suites --exclude-bench --exclude-sqlfuzz --exclude-sql
	
	- rm -rf coverage
	mkdir -p coverage/$(GIT_BRANCH)
	bin/geninfo -q --no-checksum --base-directory $(OBJROOT) --output-filename coverage.info $(OBJROOT)
	bin/genhtml --sort --no-branch-coverage -q -s -t eagle --legend -o coverage/$(GIT_BRANCH) coverage.info
	
	# check percentage
	perl -e 'use List::Util qw(first); open(DOC, "coverage/$(GIT_BRANCH)/index.html"); $$file = join("", <DOC>); @n = ($$file =~ m/(\d+\.\d)&nbsp;%/g); @m = ($$file =~ m/<a\shref=".*">(.*)<\/a>/g); $$idx = first { $$m[$$_] eq "eagle/eagle" } 0..$$#m; $$coverage = ($$n[($$idx - 2) * 2] + $$n[($$idx - 1) * 2]) / 2; die("Coverage $$coverage% is below minimum the coverage (100%).\n") if($$coverage < 100.0);'

doxygen:
	# generate docs
	- rm -rf doc
	mkdir -p doc
	if [ `doxygen 2>&1 | grep Warning | wc -l` -gt 0 ]; then \
		doxygen; \
		exit 1; \
	fi
	mv doc/html doc/$(GIT_BRANCH)
	
	# validate docs
	php bin/doxygen.php

master-only:
	if [ $(GIT_BRANCH) -neq "master" ]; then \
		echo "Run from 'master' branch. Exiting."; \
    	exit 1; \
	fi

gh-pages: master-only coverage doxygen
	# move some stuff to allow us to change branches
	- rm -rf /tmp/eagle_doc /tmp/eagle_coverage
	mv doc /tmp/eagle_doc
	mv coverage /tmp/eagle_coverage
	
	# checkout gh-pages
	git checkout gh-pages
	git reset HEAD *
	
	# replace the old files
	- rm -rf doc coverage
	mv /tmp/eagle_doc doc
	mv /tmp/eagle_coverage coverage
	git add doc coverage
	
	# commit and push
	git commit --amend -m "Auto generated"
	git push --force origin gh-pages
	git checkout master

operator-tests:
	php bin/operators.php

cli-tests:
	bin/cli.sh
