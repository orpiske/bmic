VERSION=0.0.1

rpm-prep:
	mkdir -p ${HOME}/rpmbuild/SOURCES/
	tar --transform="s/\./bmic-${VERSION}/" -cf ${HOME}/rpmbuild/SOURCES/bmic-${VERSION}.tar.gz --exclude=./build --exclude=.git --exclude=*.bz ./ --gzip

rpm: rpm-prep
	rpmbuild -ba dist/bmic.spec
