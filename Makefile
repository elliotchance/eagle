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
	xcodebuild -project eagle.xcodeproj -configuration Release -target eagle_test build

test: build_eagle_test
	build/Release/eagle_test
	
coverage: test
	geninfo --no-checksum --base-directory $(OBJROOT) --output-filename coverage.info $(OBJROOT)
	genhtml -o coverage coverage.info

doxygen:
	doxygen
