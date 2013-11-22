%define  ver     0.3.2
%define  rel     1
%define  prefix  /usr

Summary: Genius advanced calculator
Name: genius
Version: %ver
Release: %rel
Copyright: GPL
Group: X11/Applications
Source: genius-%{ver}.tar.gz
Url: http://www.gnome.org/
BuildRoot: /var/tmp/genius-%{ver}-root
Prefix: %prefix

%description
Genius is an advanced calculator and a mathematical programming language.
It handles multiple precision floating point numbers, infinite precision
integers, complex numbers and matrixes.

%changelog
* Tue Apr 20 1999 Erik Walthinsen <omega@cse.ogi.edu>
- added spec.in file, changes to makefiles as approrpiate

%prep
%setup

%build
./configure --prefix=%prefix

if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install
rm -rf $RPM_BUILD_ROOT

make prefix=$RPM_BUILD_ROOT%{prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)

%doc README ABOUT-NLS AUTHORS COPYING NEWS TODO ChangeLog

%{prefix}/bin/*
%{prefix}/share/locale/*/*/*
%{prefix}/share/genius/*
%{prefix}/share/apps/Utilities/*