// Copyright (c) 2014, Thomas Goyne <plorkyeran@aegisub.org>
// Copyright (c) 2016, Marcin Drob
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
// Aegisub Project http://www.aegisub.org/

#include "AutomationScriptReader.h"

#include "AutomationUtils.h"
#include "OpennWrite.h"
#include <wx/tokenzr.h>
#include <wx/log.h>

#include <lauxlib.h>

namespace Auto {



	bool LoadFile(lua_State *L, wxString const& filename) {
		OpenWrite ow;
		wxString script; 
		if(!ow.FileOpen(filename, &script)){return false;}
		script.Replace("aegisub","kainote");
		//script.Replace("\\","\\\\");
		
		const char *buff = script.mb_str(wxConvUTF8).data();
		int size= strlen(buff);
		//FILE *f;
		//f = _tfopen(filename.utf8_str().data(), L"rb");
		//fseek(f,0,SEEK_END);
		//int size=ftell (f);
		//rewind(f);
		//char *buff= new char[size];
		////b[size1]=0;
		//size = fread(buff, 1, size, f);
		////wxLogStatus("sizes %i, %i", size, size1);
		//if (size >= 3 && buff[0] == -17 && buff[1] == -69 && buff[2] == -65) {
		//	buff += 3;
		//	size -= 3;
		//}
		//wxLogStatus("sizes %i", size);
		//wxMessageBox(script);
		wxString name = filename.AfterLast('\\');
		if (!filename.EndsWith("moon")){
			//wxLogStatus("file nie jest moonem");
			//LuaScriptReader script_reader(filename);
			bool ret = luaL_loadbuffer(L, buff, size, name.utf8_str().data()) == 0;

			//delete[] b;
			//wxLogStatus("file nie jest moonem %i", (int)ret);
			return ret;
			
		}
		//wxLogStatus("file jednak jest moonem");
		// We have a MoonScript file, so we need to load it with that
		// It might be nice to have a dedicated lua state for compiling
		// MoonScript to Lua
		lua_getfield(L, LUA_REGISTRYINDEX, "moonscript");

		// Save the text we'll be loading for the line number rewriting in the
		// error handling
		lua_pushlstring(L, buff, size);
		lua_pushvalue(L, -1);
		lua_setfield(L, LUA_REGISTRYINDEX, ("raw moonscript: " + name).utf8_str().data());

		push_value(L, name);
		if (lua_pcall(L, 2, 2, 0))
			return false; // Leaves error message on stack

		// loadstring returns nil, error on error or a function on success
		if (lua_isnil(L, 1)) {
			lua_remove(L, 1);
			return false;
		}

		lua_pop(L, 1); // Remove the extra nil for the stackchecker
		return true;
	}

	static int module_loader(lua_State *L) {
		int pretop = lua_gettop(L);
		wxString module(check_string(L, -1));
		module.Replace(".", LUA_DIRSEP);

		// Get the lua package include path (which the user may have modified)
		lua_getglobal(L, "package");
		lua_getfield(L, -1, "path");
		wxString package_paths(check_string(L, -1));
		lua_pop(L, 2);

		wxStringTokenizer token(package_paths, ";", wxTOKEN_STRTOK);

		while (token.HasMoreTokens()) {
		
			wxString filename=token.NextToken();
			filename.Replace("?", module);

			// If there's a .moon file at that path, load it instead of the
			// .lua file
			
			if (filename.EndsWith("lua")) {
				wxString moonpath = filename.BeforeLast('.') + ".moon";
				if (wxFileExists(moonpath))
					filename = moonpath;
			}

			if (!wxFileExists(filename))
				continue;

			
			if (!LoadFile(L, filename))
				return error(L, "Error loading Lua module \"%s\":\n%s", filename.utf8_str().data(), check_string(L, 1).utf8_str().data());
			break;
			
		}

		return lua_gettop(L) - pretop;
	}

	bool Install(lua_State *L, std::vector<wxString> const& include_path) {
		// set the module load path to include_path
		//if(LoadFile(L,include_path[include_path.size()-1]+"\\moonscript.lua"))
		//{
			//wxLogStatus("moonscript not loaded");
		//}
		lua_getglobal(L, "package");
		push_value(L, "path");

		push_value(L, "");
		for (auto const& path : include_path) {
			lua_pushfstring(L, "%s/?.lua;%s/?/init.lua;", path.utf8_str().data(), path.utf8_str().data());
			lua_concat(L, 2);
		}


		lua_settable(L, -3);

		// Replace the default lua module loader with our unicode compatible one
		lua_getfield(L, -1, "loaders");
		push_value(L, exception_wrapper<module_loader>);
		lua_rawseti(L, -2, 2);
		lua_pop(L, 2); // loaders, package

		luaL_loadstring(L, "return require('moonscript').loadstring");
		if (lua_pcall(L, 0, 1, 0)) {
			return false; // leave error message
		}
		lua_setfield(L, LUA_REGISTRYINDEX, "moonscript");
		return true;
	}
} 
