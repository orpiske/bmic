# Debug info package generation currently breaks the RPM build
%global _enable_debug_package 0
%global debug_package %{nil}

Summary:            Broker Management Interface Client (BMIC)
Name:               bmic
Version:            0.0.2
Release:            4%{?dist}
License:            Apache-2.0
Source:             bmic-%{version}.tar.gz
URL:                https://github.com/orpiske/bmic
BuildRequires:      cmake
BuildRequires:      make
BuildRequires:      gcc
BuildRequires:      json-c-devel
BuildRequires:      curl-devel
BuildRequires:      gru-devel


%description
This project allows access to the REST management interface exposed by brokers such as 
ActiveMQ and Artemis


%package devel
Summary:            Broker Management Interface Client (BMIC) development kit
Requires:           json-c-devel
Requires:           curl-devel
Requires:           gru-devel
Requires:           %{name}%{?_isa} = %{version}-%{release}

%description devel
Development packages for the BMIC

%package devel-doc
Summary:            Broker Management Interface Client (BMIC) development kit documentation
BuildArch:          noarch

%description devel-doc
Development documentation for the BMIC library

%prep
%autosetup -n bmic-%{version}

%build
mkdir build && cd build
%cmake -DBUILD_WITH_DOCUMENTATION=ON -DCMAKE_USER_C_FLAGS="-fPIC" ..
%make_build all documentation

%install
cd build
%make_install

%files
%doc AUTHORS README.md
%license LICENSE COPYING
%{_bindir}/*
%{_libdir}/*.so.*

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files devel
%{_includedir}/*
%{_libdir}/*.so

%post devel -p /sbin/ldconfig

%postun devel -p /sbin/ldconfig

%files devel-doc
%license LICENSE COPYING
%{_datadir}/*


%changelog
* Wed Aug 30 2017 Otavio R. Piske <angusyoung@gmail.com> - 0.0.2-4
- Fixed mismatch between this package version and the SO version

* Sat Aug 26 2017 Otavio R. Piske <angusyoung@gmail.com> - 0.0.2-3
- Adjusted to match fedora packaging guidelines
- Enable documentation package

* Thu Jul 27 2017 Otavio R. Piske <angusyoung@gmail.com> - 0.0.2-2
- Updated package to comply with Fedora packaging guidelines

* Fri Feb 24 2017 Otavio R. Piske <angusyoung@gmail.com> - 20170224
- Initial release
