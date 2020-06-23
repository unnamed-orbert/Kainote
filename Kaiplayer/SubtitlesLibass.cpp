//  Copyright (c) 2020, Marcin Drob

//  Kainote is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.

//  Kainote is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with Kainote.  If not, see <http://www.gnu.org/licenses/>.

#include "SubtitlesProvider.h"
#ifdef subsProvider


#include "RendererVideo.h"
#include "OpennWrite.h"
#include "kainoteMain.h"
#include "DshowRenderer.h"
#include <boost/gil/algorithm.hpp>
#include <process.h>




void MessageCallback(int level, const char *fmt, va_list args, void *) {
	if (level >= 4) return;
	char buf[1024];

	vsprintf_s(buf, sizeof(buf), fmt, args);

	if (level < 2) // warning/error
		KaiLog(L"Libass: " + wxString(buf, wxConvUTF8));
	else // verbose
		KaiLogDebug(L"Libass: " + wxString(buf, wxConvUTF8));
}

unsigned int __stdcall  ProcessLibassCache(void *data)
{
	SubtitlesLibass * libass = (SubtitlesLibass*)data;
	if (!libass->m_Libass) {
		libass->m_Libass = ass_renderer_init(libass->m_Library);
		if (libass->m_Libass) {
			ass_set_font_scale(libass->m_Libass, 1.);
			ass_set_fonts(libass->m_Libass, "Arial", "Arial", 1, NULL, true);
		}
	}
	if (libass->m_SubsSkipped) {
		TabPanel * tab = Notebook::GetTab();
		if(tab->Video->GetState() != None)
			tab->Video->OpenSubs(OPEN_DUMMY, true, true);
	}
	libass->m_IsReady.store(true);

	return 0;
}

SubtitlesLibass::SubtitlesLibass()
{
	if (!m_Library) {
		m_Library = ass_library_init();
		ass_set_message_cb(m_Library, MessageCallback, NULL);
	}
	unsigned int threadid = 0;
	thread = (HANDLE)_beginthreadex(0, 0, ProcessLibassCache, this, 0, &threadid);
	//SetThreadPriority(thread, THREAD_PRIORITY_TIME_CRITICAL);
	SetThreadName(threadid, "LibassCache");

}
	
SubtitlesLibass::~SubtitlesLibass()
{
	//close it by force can make memory leaks
	if (thread){
		CloseHandle(thread);
	}

	if (m_AssTrack)
		ass_free_track(m_AssTrack);
}

// code taken from Aegisub
#define _r(c) ((c)>>24)
#define _g(c) (((c)>>16)&0xFF)
#define _b(c) (((c)>>8)&0xFF)
#define _a(c) ((c)&0xFF)

void SubtitlesLibass::Draw(unsigned char* buffer, int time)
{
	if (m_IsReady.load() && m_AssTrack){
		ass_set_frame_size(m_Libass, m_VideoSize.GetWidth(), m_VideoSize.GetHeight());

		ASS_Image* img = ass_render_frame(m_Libass, m_AssTrack, time, NULL);

		// libass actually returns several alpha-masked monochrome images.
		// Here, we loop through their linked list, get the colour of the current, and blend into the frame.
		// This is repeated for all of them.

		using namespace boost::gil;
		auto dst = interleaved_view(m_VideoSize.GetWidth(), m_VideoSize.GetHeight(), (bgra8_pixel_t*)buffer, m_VideoSize.GetWidth() * m_BytesPerColor);
		if (m_IsSwapped)
			dst = flipped_up_down_view(dst);

		for (; img; img = img->next) {
			unsigned int opacity = 255 - ((unsigned int)_a(img->color));
			unsigned int r = (unsigned int)_r(img->color);
			unsigned int g = (unsigned int)_g(img->color);
			unsigned int b = (unsigned int)_b(img->color);

			auto srcview = interleaved_view(img->w, img->h, (gray8_pixel_t*)img->bitmap, img->stride);
			auto dstview = subimage_view(dst, img->dst_x, img->dst_y, img->w, img->h);

			transform_pixels(dstview, srcview, dstview, [=](const bgra8_pixel_t frame, const gray8_pixel_t src) -> bgra8_pixel_t {
				unsigned int k = ((unsigned)src) * opacity / 255;
				unsigned int ck = 255 - k;

				bgra8_pixel_t ret;
				ret[0] = (k * b + ck * frame[0]) / 255;
				ret[1] = (k * g + ck * frame[1]) / 255;
				ret[2] = (k * r + ck * frame[2]) / 255;
				ret[3] = 0;
				return ret;
			});
		}
	}
	else{
		//make an info of loading fonts to Libass
		//best to draw it on video 
		//make function for it in renderer and use it here;
	}
}

bool SubtitlesLibass::Open(TabPanel *tab, int flag, wxString *text)
{
	if (!m_IsReady || !m_HasParameters) {
		SAFE_DELETE(text);
		if (!m_HasParameters)
			KaiLog("Libass only works with with FFMS2");
		else
			m_SubsSkipped = true;

		return false;
	}

	if (m_AssTrack){
		ass_free_track(m_AssTrack);
		m_AssTrack = NULL;
	}

	RendererVideo* renderer = tab->Video->GetRenderer();
	if (!renderer) {
		SAFE_DELETE(text);
		return false;
	}

	wxString *textsubs = text;
	switch (flag){
	case OPEN_DUMMY:
		textsubs = tab->Grid->GetVisible();
		renderer->m_HasDummySubs = true;
		break;
	case OPEN_WHOLE_SUBTITLES:
		//make here some function to buffor
		//or even add olny here a bool
		textsubs = tab->Grid->GetVisible(NULL, NULL, NULL, true);
		renderer->m_HasDummySubs = false;
		break;
	case CLOSE_SUBTITLES:
	case OPEN_HAS_OWN_TEXT:
		break;
	default:
		break;
	}


	if (!textsubs) {
		return true;
	}

	if (renderer->m_HasVisualEdition && renderer->m_Visual->Visual == VECTORCLIP && renderer->m_Visual->dummytext){
		wxString toAppend = renderer->m_Visual->dummytext->Trim().AfterLast(L'\n') + L"\r\n";
		(*textsubs) << toAppend;
	}

	wxScopedCharBuffer buffer = textsubs->mb_str(wxConvUTF8);
	int size = strlen(buffer);
	m_AssTrack = ass_read_memory(m_Library, buffer.data(), size, NULL);
	delete textsubs;

	if (!m_AssTrack){
		KaiLog("Libass only works with ASS and SSA subtiltes");
		return false;
	}
	return true;
}

bool SubtitlesLibass::OpenString(wxString *text)
{
	if (!m_IsReady) {
		SAFE_DELETE(text);
		return false;
	}

	if (m_AssTrack){
		ass_free_track(m_AssTrack);
		m_AssTrack = NULL;
	}

	wxScopedCharBuffer buffer = text->mb_str(wxConvUTF8);
	int size = strlen(buffer);
	m_AssTrack = ass_read_memory(m_Library, buffer.data(), size, NULL);

	delete text;

	if (!m_AssTrack){
		KaiLog(L"Nie mo�na otworzy� napis�w w Libass");
		return false;
	}
	return true;
}

void SubtitlesLibass::SetVideoParameters(const wxSize & size, unsigned char format, bool isSwapped)
{
	m_VideoSize = size;
	m_IsSwapped = isSwapped;
	m_Format = format;
	m_HasParameters = format == RGB32;
}

#endif