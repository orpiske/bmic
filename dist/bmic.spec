Summary:            Broker Management Interface Client (BMIC)
Name:               bmic
Version:            0.0.2
Release:            2%{?dist}
License:            Apache-2.0
Group:              Development/Tools
Source:             bmic-%{version}.tar.gz
URL:                https://github.com/orpiske/bmic
BuildRequires:      cmake
BuildRequires:      make
BuildRequires:      gcc
BuildRequires:      json-c-devel
BuildRequires:      curl-devel
BuildRequires:      gru
BuildRequires:      gru-devel
Requires:           json-c
Requires:           curl
Requires:           gru


%description
This project allows access to the REST management interface exposed by brokers such as 
ActiveMQ and Artemis


%package devel
Summary:            Broker Management Interface Client (BMIC) development kit
Requires:           json-c-devel
Requires:           curl-devel
Requires:           gru-devel
Requires:           bmic
Group:              Development/Libraries

%description devel
Development packages for the BMIC

%prep
%autosetup -n bmic-%{version}

%build
mkdir build && cd build
%cmake -DBUILD_WITH_DOCUMENTATION=ON -DCMAKE_USER_C_FLAGS="-fPIC" ..
make all documentation

%install
cd build
make install DESTDIR=%{buildroot}

%files
%doc AUTHORS README.md LICENSE COPYING
%{_bindir}/*
%{_libdir}/*


%files devel
%{_includedir}/*


%changelog
* Thu Jul 27 2017 Otavio R. Piske <angusyoung@gmail.com> - 0.0.2-2
- Updated package to comply with Fedora packaging guidelines
- Enabled generation of debuginfo package

* Fri Feb 24 2017 Otavio R. Piske <angusyoung@gmail.com> - 20170224
- Initial release
