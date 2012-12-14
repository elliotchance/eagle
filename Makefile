OBJROOT = `xcodebuild -project eagle.xcodeproj -target eagle_test -showBuildSettings | grep OBJROOT | cut -c15-`

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
	build/Debug/eagle_test wait &
	sleep 2
	leaks eagle_test | grep leaks
	killall eagle_test

test: build_eagle_test
	build/Debug/eagle_test
	
coverage: test
	geninfo --no-checksum --base-directory $(OBJROOT) --output-filename coverage.info $(OBJROOT)
	genhtml -o coverage coverage.info

doxygen:
	doxygen
