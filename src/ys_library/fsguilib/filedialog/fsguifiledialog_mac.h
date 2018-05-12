/* ////////////////////////////////////////////////////////////

File Name: fsguifiledialog_mac.h
Copyright (c) 2017 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#ifndef FSGUIFILEDIALOG_MAC_H_IS_INCLUDED
#define FSGUIFILEDIALOG_MAC_H_IS_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define FSGUIFILEDIALOG_MAC_MODE_OPEN 0
#define FSGUIFILEDIALOG_MAC_MODE_SAVE 1

struct FsGuiFileDialog_Mac_StringList
{
	char *fn;
	struct FsGuiFileDialog_Mac_StringList *next;
};

struct FsGuiFileDialog_Mac
{
	int in_multiSelect;
	int in_mode;
	struct FsGuiFileDialog_Mac_StringList *in_fileExtensionList;
	const char *defaultPath;
	const char *defaultFile;
	const char *title;

	int out_result;
	struct FsGuiFileDialog_Mac_StringList *out_selected;
};

	void FsGuiFileDialog_Mac_InitDialog(struct FsGuiFileDialog_Mac *dlg);
	void FsGuiFileDialog_Mac_AddExtension(struct FsGuiFileDialog_Mac *dlg,const char *ext);
	void FsGuiFileDialog_Mac_OpenDialog(struct FsGuiFileDialog_Mac *dlg);
	void FsGuiFileDialog_Mac_CleanUp(struct FsGuiFileDialog_Mac *dlg);

#ifdef __cplusplus
}
#endif

#endif
