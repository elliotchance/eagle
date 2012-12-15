OBJROOT = `xcodebuild -project eagle.xcodeproj -target eagle_test -showBuildSettings | grep OBJROOT | cut -c15-`
GIT_BRANCH = `git branch | grep \* | cut -c3-`

all: clean build

release: clean build coverage doxygen

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
	- rm -rf coverage/html
	- rm -rf coverage/$(GIT_BRANCH)
	- mkdir -p coverage/html
	- mkdir -p coverage/$(GIT_BRANCH)
	geninfo -q --no-checksum --base-directory $(OBJROOT) --output-filename coverage.info $(OBJROOT)
	genhtml -q -s -t eagle --legend -o coverage/html coverage.info
	mv coverage/html coverage/$(GIT_BRANCH)

doxygen:
	- rm -rf doc/html
	- rm -rf doc/$(GIT_BRANCH)
	doxygen
	mv doc/html doc/$(GIT_BRANCH)

master-only:
	if [ $(GIT_BRANCH) -neq "master" ]; then \
		echo "Run from 'master' branch. Exiting."; \
    	exit 1; \
	fi
	
checkout-gh-pages:
	git checkout --force gh-pages

gh-pages: master-only checkout-gh-pages coverage doxygen
	git reset HEAD *
	git commit --amend -m "Auto generated"
	git push origin gh-pages
