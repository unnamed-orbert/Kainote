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

#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "Videobox.h"
#include "DshowRenderer.h"
#include "dshowplayer.h"
#include "Visuals.h"

#undef DrawText

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if (x !=NULL) { delete x; x = NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if (x != NULL) { x->Release(){}; x = NULL; } 
#endif



#ifndef PTR
#define PTR(what,err) if(!what) {KaiLog(err){}; return false;}
#endif

#ifndef PTR1
#define PTR1(what,err) if(!what) {KaiLog(err){}; return;}
#endif

#ifndef HR
#define HR(what,err) if(FAILED(what)) {KaiLog(err){}; return false;}
#endif

#ifndef HRN
#define HRN(what,err) if(FAILED(what)) {KaiLog(err){}; return;}
#endif



typedef void csri_inst;
//typedef void csri_rend;

//enum PlaybackState
//{
//	Playing,
//	Paused,
//	Stopped,
//	None
//};


struct VERTEX
{
	float fX;
	float fY;
	float fZ;
	D3DCOLOR Color;
};

class FloatRect
{
public:
	FloatRect(float _x, float _y, float _width, float _height){ x = _x; y = _y; width = _width; height = _height; };
	FloatRect(){ x = 0; y = 0; width = 0; height = 0; }
	float GetBottom() const { return y - height - 1; }
	float GetRight()  const { return x - width - 1; }
	float x;
	float y;
	float width;
	float height;
};

void CreateVERTEX(VERTEX *v, float X, float Y, D3DCOLOR Color, float Z = 0.0f){};


class AudioDisplay;
class DShowPlayer;
struct csri_fmt;
struct csri_frame;
//class RendererDirectShow;


class RendererVideo
{
	friend class RendererDirectShow;
	friend class RendererFFMS2;
	friend class VideoCtrl;
public:
	RendererVideo(VideoCtrl *control){};
	~RendererVideo(){};

	virtual bool OpenFile(const wxString &fname, wxString *textsubs, bool vobsub, bool changeAudio = true){ return false; };
	virtual bool OpenSubs(wxString *textsubs, bool redraw = true, bool fromFile = false){ return false; };
	virtual bool Play(int end = -1){ return false; };
	virtual bool Pause(){ return false; };
	virtual bool Stop(){ return false; };
	virtual void SetPosition(int _time, bool starttime = true, bool corect = true, bool async = true){};
	virtual void SetFFMS2Position(int time, bool starttime){};
	virtual void GoToNextKeyframe(){};
	virtual void GoToPrevKeyframe(){};
	virtual int GetFrameTime(bool start = true){ return 0; };
	virtual void GetStartEndDelay(int startTime, int endTime, int *retStart, int *retEnd){};
	virtual int GetFrameTimeFromTime(int time, bool start = true){ return 0; };
	virtual int GetFrameTimeFromFrame(int frame, bool start = true){ return 0; };
	//if nothing loaded or loaded via Direct Show VFF is NULL
	//return true if VFF is present
	//bool GetStartEndDurationFromMS(Dialogue *dial, STime &duration);
	virtual int GetPlayEndTime(int time){ return 0; };
	virtual int GetDuration(){ return 0; };
	virtual int GetVolume(){ return 0; };
	virtual void GetVideoSize(int *width, int *height){};
	virtual void GetFpsnRatio(float *fps, long *arx, long *ary){};
	virtual void SetVolume(int vol){};
	virtual void Render(bool RecreateFrame = true, bool wait = true){};
	virtual void RecreateSurface(){};
	virtual void EnableStream(long index){};
	virtual void ChangePositionByFrame(int cpos){};
	virtual void ChangeVobsub(bool vobsub = false){};
	virtual wxArrayString GetStreams(){ wxArrayString empty; return empty; };
	virtual byte *GetFramewithSubs(bool subs, bool *del){ return NULL; };
	//int GetPreciseTime(bool start = true){};
	virtual void DeleteAudioCache(){}
	virtual void SetColorSpace(const wxString& matrix, bool render = true){}
	virtual void OpenKeyframes(const wxString &filename){};
	
	LPDIRECT3DSURFACE9 m_MainSurface;
	LPDIRECT3DDEVICE9 m_D3DDevice;
	D3DFORMAT m_D3DFormat;
	volatile bool m_BlockResize;
	bool seek;
	bool m_HasVisualEdition;
	bool m_HasDummySubs = true;
	bool m_VideoResized;
	bool m_HasZoom;
	bool m_SwapFrame = false;
	int m_Width;
	int m_Height;
	int m_Pitch;
	int m_Time;
	int m_Frame;
	char *m_FrameBuffer;
	byte m_Format;
	float m_FrameDuration;
	float m_ZoomParcent;
	wxString m_ProgressBarTime;
	ID3DXLine *m_D3DLine;
	LPD3DXFONT m_D3DFont;
	wxCriticalSection m_MutexRendering;
	wxMutex m_MutexProgressBar;
	wxMutex m_MutexOpen;
	wxMutex m_MutexVisualChange;
	PlaybackState m_State;
	int m_PlayEndTime;
	size_t m_LastTime;
	FloatRect m_ZoomRect;
	std::vector<chapter> m_Chapters;
	wxString m_KeyframesFileName;
	IDirectXVideoProcessorService *m_DXVAService;
	IDirectXVideoProcessor *m_DXVAProcessor;
	LPDIRECT3D9 m_D3DObject;
	LPDIRECT3DSURFACE9 m_BlackBarsSurface;

#if byvertices
	LPDIRECT3DVERTEXBUFFER9 vertex;
	LPDIRECT3DTEXTURE9 texture;
#endif

	virtual bool EnumFilters(Menu *menu){ return false; };
	virtual bool FilterConfig(wxString name, int idx, wxPoint pos){ return false; };
	virtual bool HasFFMS2(){ return false; };
	virtual VideoFfmpeg * GetFFMS2(){ return NULL; };
	// Non virtual functions
	void DrawProgBar();
	bool DrawTexture(byte *nframe = NULL, bool copy = false);
	void Zoom(const wxSize &size);
	void DrawZoom();
	void ZoomMouseHandle(wxMouseEvent &evt);
	void SetZoom();
	void ResetZoom();
	void SetVisualZoom();
	void SetVisual(bool settext = false, bool noRefresh = false);
	void ResetVisual();
	//it's safe to not exist visual
	//returns true if removed
	bool RemoveVisual(bool noRefresh = false);
	int GetCurrentPosition();
	int GetCurrentFrame();
	bool PlayLine(int start, int end);
	void UpdateVideoWindow();
	bool UpdateRects(bool changeZoom = true);
	void VisualChangeTool(int tool);
	bool HasVisual();
	Visuals *GetVisual();
	
private:

	bool InitDX(bool reset = false);
	virtual bool InitRendererDX(){ return false; };
	void Clear(bool clearObject = false);

	HWND m_HWND;
	bool m_DeviceLost;

	int diff;
	char m_Grabbed;

	csri_inst *instance;
	RECT m_ProgressBarRect;
	RECT m_WindowRect;
	RECT m_BackBufferRect;
	RECT m_MainStreamRect;
	wxPoint m_ZoomDiff;

	int m_AverangeFrameTime;
	D3DXVECTOR2 vectors[12];
	AudioDisplay *m_AudioPlayer;
	csri_frame *framee;
	csri_fmt *format;
	Visuals *m_Visual;
	VideoCtrl *videoControl;
	TabPanel* tab;
};
