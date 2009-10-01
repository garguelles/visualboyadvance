// -*- C++ -*-
// VisualBoyAdvance - Nintendo Gameboy/GameboyAdvance (TM) emulator.
// Copyright (C) 1999-2003 Forgotten
// Copyright (C) 2004 Forgotten and the VBA development team

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or(at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef __VBA_WINDOW_H__
#define __VBA_WINDOW_H__

#include <gtkmm.h>

#include "../System.h"
#include "InputSDL.h"

#include "ConfigFile.h"
#include "ScreenArea.h"

namespace VBA
{

class Window : public Gtk::Window
{
	friend class Gtk::Builder;

public:
	virtual ~Window();

	inline static Window * poGetInstance() {
		return m_poInstance;
	}
	static std::string sGetUiFilePath(const std::string &_sFileName);

	// GBA screen size
	const int m_iGBAScreenWidth;
	const int m_iGBAScreenHeight;

	bool bLoadROM(const std::string & _rsFile);
	void vPopupError(const char * _csFormat, ...);
	void vPopupErrorV(const char * _csFormat, va_list _args);
	void vDrawScreen(u32 *pix);
	void vComputeFrameskip(int _iRate);
	void vShowSpeed(int _iSpeed);
	void vApplyConfigScreenArea();
	void vApplyConfigVolume();
	void vApplyConfigSoundSampleRate();
	void vApplyConfigShowSpeed();
	void vApplyConfigFrameskip();
	void vUpdateScreen();
	void vUpdateGameSlots();

protected:
	Window(GtkWindow * _pstWindow,
	       const Glib::RefPtr<Gtk::Builder> & _poBuilder);

	enum EColorFormat
	{
		ColorFormatRGB,
		ColorFormatBGR
	};

	virtual void vOnMenuEnter();
	virtual void vOnMenuExit();
	virtual void vOnFileOpen();
	virtual void vOnFileLoad();
	virtual void vOnFileSave();
	virtual void vOnLoadGameMostRecent();
	virtual void vOnLoadGameAutoToggled(Gtk::CheckMenuItem * _poCMI);
	void vOnLoadGame(int _iSlot);
	virtual void vOnSaveGameOldest();
	void vOnSaveGame(int _iSlot);
	virtual void vOnFilePauseToggled(Gtk::CheckMenuItem * _poCMI);
	virtual void vOnFileReset();
	virtual void vOnRecentFile();
	virtual void vOnFileClose();
	virtual void vOnFileExit();
	virtual void vOnVideoFullscreen();
	virtual void vOnJoypadConfigure();
	virtual void vOnSettings();
	virtual void vOnHelpAbout();
	virtual bool bOnEmuIdle();

	virtual bool on_focus_in_event(GdkEventFocus * _pstEvent);
	virtual bool on_focus_out_event(GdkEventFocus * _pstEvent);
	virtual bool on_key_press_event(GdkEventKey * _pstEvent);
	virtual bool on_key_release_event(GdkEventKey * _pstEvent);
	virtual bool on_window_state_event(GdkEventWindowState* _pstEvent);

private:
	// Config limits
	const int m_iScaleMin;
	const int m_iScaleMax;
	const int m_iSoundSampleRateMin;
	const int m_iSoundSampleRateMax;
	const float m_fSoundVolumeMin;
	const float m_fSoundVolumeMax;

	static Window * m_poInstance;

	Glib::RefPtr<Gtk::Builder> m_poBuilder;

	std::string       m_sUserDataDir;
	std::string       m_sConfigFile;
	Config::File      m_oConfig;
	Config::Section * m_poDirConfig;
	Config::Section * m_poCoreConfig;
	Config::Section * m_poDisplayConfig;
	Config::Section * m_poSoundConfig;
	Config::Section * m_poInputConfig;

	Gtk::FileChooserDialog * m_poFileOpenDialog;

	ScreenArea *         m_poScreenArea;
	Gtk::CheckMenuItem * m_poFilePauseItem;
	Gtk::CheckMenuItem * m_poSoundOffItem;
	Gtk::MenuBar *       m_poMenuBar;

	struct SGameSlot
	{
		bool        m_bEmpty;
		std::string m_sFile;
		time_t      m_uiTime;
	};

	struct SJoypadKey
	{
		const char * m_csKey;
		const EKey   m_eKeyFlag;
	};

	static const SJoypadKey m_astJoypad[];

	Gtk::MenuItem * m_apoLoadGameItem[10];
	Gtk::MenuItem * m_apoSaveGameItem[10];
	SGameSlot       m_astGameSlot[10];

	Glib::RefPtr<Gtk::RecentManager> m_poRecentManager;
	Gtk::MenuItem *                  m_poRecentMenu;
	Gtk::RecentChooserMenu *         m_poRecentChooserMenu;

	std::list<Gtk::Widget *> m_listSensitiveWhenPlaying;

	sigc::connection m_oEmuSig;

	int m_bFullscreen;
	int m_iScreenWidth;
	int m_iScreenHeight;
	int m_iFrameCount;

	EmulatedSystem m_stEmulator;
	bool           m_bPaused;
	bool           m_bWasEmulating;
	bool           m_bAutoFrameskip;
	bool           m_bShowSpeed;

	void vInitSystem();
	void vUnInitSystem();
	void vInitSDL();
	void vInitConfig();
	void vCheckConfig();
	void vInitColors(EColorFormat _eColorFormat);
	void vLoadConfig(const std::string & _rsFile);
	void vSaveConfig(const std::string & _rsFile);
	void vHistoryAdd(const std::string & _rsFile);
	void vApplyConfigJoypads();
	void vSaveJoypadsToConfig();
	void vDrawDefaultScreen();
	void vSetDefaultTitle();
	void vCreateFileOpenDialog();
	void vLoadBattery();
	void vSaveBattery();
	void vStartEmu();
	void vStopEmu();
	void vToggleFullscreen();
	void vSDLPollEvents();
};

} // namespace VBA


#endif // __VBA_WINDOW_H__