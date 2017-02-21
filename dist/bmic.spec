%global _enable_debug_package 0
%global debug_package %{nil}

Summary:            Broker Management Interface Client (BMIC)
Name:               bmic
Version:            0.0.1
Release:            1%{?dist}
License:            Apache-2.0
Group:              Development/Tools
Source:             bmic-%{version}.tar.gz
URL:                https://github.com/orpiske/bmic
BuildRequires:      cmake
BuildRequires:      make
BuildRequires:      gcc
BuildRequires:      json-c-devel
BuildRequires:      curl-devel


%description
This project allows access to the REST management interface exposed by brokers such as 
ActiveMQ and Artemis


%package devel
Summary:            Broker Management Interface Client (BMIC) development kit
Requires:           gcc
Requires:           json-c-devel
Requires:           curl-devel
Group:              Development/Libraries

%description devel
Development packages for the BMIC

%prep
%autosetup -n bmic-%{version}

%build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=%{buildroot}/usr ..
make

%install
cd build
make install

%files
%doc AUTHORS README.md LICENSE COPYING
%{_libdir}/*


%files devel
%{_includedir}/*


%changelog
* Tue Nov 04 2016 Otavio R. Piske <angusyoung@gmail.com> - 20161104
- Changed non-write interfaces of the list to use a const pointer

* Tue Sep 07 2016 Otavio R. Piske <angusyoung@gmail.com> - 20160907
- Initial release
