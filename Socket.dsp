



<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<!-- ViewVC :: http://www.viewvc.org/ -->
<head>
<title>[cvs] View of /TyzxRoot/Devel/Source/Emerging/Socket/Socket.dsp</title>
<meta name="generator" content="ViewVC 1.0.3" />
<link rel="stylesheet" href="/viewvc-static/styles.css" type="text/css" />

</head>
<body>
<div class="vc_navheader">

<form method="get" action="/cgi-bin/viewvc.cgi/">

<table style="padding:0.1em;">
<tr>
<td>
<strong>

<a href="/cgi-bin/viewvc.cgi/">

[cvs]</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/">

TyzxRoot</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/">

Devel</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/">

Source</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/">

Emerging</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/">

Socket</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/Socket.dsp?view=log">

Socket.dsp</a>


</strong>

</td>
<td style="text-align:right;">


<strong>Repository:</strong>
<select name="root" onchange="submit()">


<option value="*viewroots*">Repository Listing</option>




















<optgroup label="CVS Repositories"><option selected="selected">cvs</option></optgroup>


<optgroup label="Subversion Repositories"><option>svn</option></optgroup>

</select>
<input type="submit" value="Go" />

</td>
</tr>
</table>

</form>

</div>
<div style="float: right; padding: 5px;"><a href="http://www.viewvc.org/"><img src="/viewvc-static/images/logo.png" alt="ViewVC logotype" width="128" height="48" /></a></div>
<h1>View of /TyzxRoot/Devel/Source/Emerging/Socket/Socket.dsp</h1>

<p style="margin:0;">

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/"><img src="/viewvc-static/images/back_small.png" width="16" height="16" alt="Parent Directory" /> Parent Directory</a>

| <a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/Socket.dsp?view=log#rev1.4"><img src="/viewvc-static/images/log.png" width="16" height="16" alt="Revision Log" /> Revision Log</a>




</p>

<hr />
<div class="vc_summary">
Revision <strong>1.4</strong> -
(<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/Socket.dsp?revision=1.4"><strong>download</strong></a>)

(<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/Socket.dsp?annotate=1.4"><strong>annotate</strong></a>)

<br /><em>Tue Jan  8 19:13:05 2008 UTC</em>
(3 months ago)
by <em>khoi</em>


<br />Branch: <strong>MAIN</strong>


<br />CVS Tags: <strong>Rev_6_1, HEAD</strong>




<br />Changes since <strong>1.3: +2 -2 lines</strong>





<pre class="vc_log">use TyzxSupport
</pre>

</div>
<div id="vc_markup"><pre># Microsoft Developer Studio Project File - Name="Socket" - Package Owner=&lt;4&gt;
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Socket - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Socket.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Socket.mak" CFG="Socket - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Socket - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Socket - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Socket - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
PP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(TYZXROOT)/Devel/Source/TyzxSupport" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\Socket.lib"

!ELSEIF  "$(CFG)" == "Socket - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
PP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "$(TYZXROOT)/Devel/Source/TyzxSupport" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\Socket.lib"

!ENDIF 

# Begin Target

# Name "Socket - Win32 Release"
# Name "Socket - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Socket.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\UdpSocket.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Socket.h
# End Source File
# Begin Source File

SOURCE=.\SocketManager.h
# End Source File
# Begin Source File

SOURCE=.\TcpSocket.h
# End Source File
# Begin Source File

SOURCE=.\UdpSocket.h
# End Source File
# End Group
# End Target
# End Project
</pre></div>

<hr />
<table>
<tr>
<td><address><a href="mailto:cvs-admin@insert.your.domain.here">No admin address has been configured</a></address></td>
<td style="text-align: right;"><strong><a href="/viewvc-static/help_rootview.html">ViewVC Help</a></strong></td>
</tr>
<tr>
<td>Powered by <a href="http://viewvc.tigris.org/">ViewVC 1.0.3</a></td>
<td style="text-align: right;">&nbsp;</td>
</tr>
</table>
</body>
</html>

