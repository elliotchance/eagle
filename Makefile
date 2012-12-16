OBJROOT = `xcodebuild -project eagle.xcodeproj -target eagle_test -showBuildSettings | grep OBJROOT | cut -c15-`
GIT_BRANCH = `git branch | grep \* | cut -c3-`

all: clean build

release: clean build leaks coverage doxygen

clean: clean_eagle clean_eagle_test
	
clean_eagle:
	xcodebuild -project eagle.xcodeproj -target eagle clean
	
clean_eagle_test:
	xcodebuild -project eagle.xcodeproj -target eagle_test clean
	
build: build_eagle build_eagle_test

build_eagle:
	xcodebuild -project eagle.xcodeproj -configuration Release -target eagle build

build_eagle_test:
	xcodebuild -project eagle.xcodeproj -configuration Debug -target eagle_test build
	
# test for leaks
leaks: build_eagle_test
	- killall eagle_test
	MallocStackLogging=1 build/Debug/eagle_test wait &
	sleep 2
	#malloc_history eagle_test -callTree -invert -collapseRecursion -chargeSystemLibraries
	leaks eagle_test
	killall eagle_test

test: clean_eagle_test build_eagle_test
	- rm coverage.info
	build/Debug/eagle_test
	
coverage: test
	- rm -rf coverage
	mkdir -p coverage/$(GIT_BRANCH)
	geninfo -q --no-checksum --base-directory $(OBJROOT) --output-filename coverage.info $(OBJROOT)
	genhtml -q -s -t eagle --legend -o coverage/$(GIT_BRANCH) coverage.info

doxygen:
	- rm -rf doc
	mkdir -p doc
	doxygen
	mv doc/html doc/$(GIT_BRANCH)

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
