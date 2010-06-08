#include "lib/universal_include.h"

#ifdef USE_DARWINIA_MOD_SYSTEM


#include "lib/filesys/filesys_utils.h"
#include "lib/text_renderer.h"
#include "lib/resource.h"
#include "lib/language_table.h"

#include "interface/mods_window.h"
#include "interface/debugmenu.h"
#include "interface/input_field.h"

#include "app.h"
#include "global_world.h"
#include "renderer.h"



class LoadModButton : public DarwiniaButton
{
public:
    char *m_modName;

    void MouseUp()
    {
        g_app->m_resource->LoadMod( m_modName );
        
        g_app->SetProfileName( m_modName );
        g_app->LoadProfile();
        EclRemoveWindow( m_parent->m_name );
        EclRemoveWindow( "dialog_mainmenu" );
    }
};


class ToolsButton : public DarwiniaButton
{
    void MouseUp()
    {
	    g_app->m_requestToggleEditing = true;
    }
};


class NewModWindowButton : public DarwiniaButton
{
    void MouseUp()
    {
        EclRegisterWindow( new NewModWindow(), m_parent );
    }
};


ModsWindow::ModsWindow()
:   DarwiniaWindow( "dialog_mods" )
{
}


void ModsWindow::Render( bool hasFocus )
{
    DarwiniaWindow::Render( hasFocus );

    g_editorFont.DrawText2DCentre( m_x+m_w/2, m_y+GetMenuSize(30), GetMenuSize(12), LANGUAGEPHRASE("dialog_currentmod"));
	g_editorFont.DrawText2DCentre( m_x+m_w/2, m_y+GetMenuSize(45), GetMenuSize(16), "%s", 
		g_app->m_resource->IsModLoaded() ? g_app->m_resource->GetModName() : "none" );
}


void ModsWindow::Create()
{
    char modsDir[256];
    sprintf( modsDir, "%smods/*.*", g_app->GetProfileDirectory() );
    LList<char *> *modList = ListSubDirectoryNames( modsDir );
#ifdef TARGET_OS_VISTA
    sprintf( modsDir, "mods/*.*" );
    LList<char *> *baseModList = ListSubDirectoryNames( modsDir );

    for( int i = 0; i < baseModList->Size(); ++i )
    {
        modList->PutData( baseModList->GetData(i) );
    }
#endif
    int numMods = modList->Size();

    int windowH = 210 + numMods * 30;    
    SetMenuSize( 300, windowH );
	SetPosition( g_app->m_renderer->ScreenW()/2 - m_w/2, 
                 g_app->m_renderer->ScreenH()/2 - m_h/2 );

    DarwiniaWindow::Create();

    int y = GetMenuSize(50);
    int h = GetMenuSize(30);
    
    int invertY = y+GetMenuSize(20);

	int fontSize = GetMenuSize(13);

    //
    // Inverted box

    if( numMods > 0 )
    {
        InvertedBox *box = new InvertedBox();
        box->SetShortProperties( "Box", 10, invertY, m_w-20, numMods * GetMenuSize(30) + GetMenuSize(20), UnicodeString("Box") );
        RegisterButton( box );
    }

    
    for( int i = 0; i < modList->Size(); ++i )
    {
        char *thisMod = modList->GetData(i);
        wchar_t caption[256];
        swprintf( caption, sizeof(caption)/sizeof(wchar_t), L"%ls: '%ls'", LANGUAGEPHRASE("dialog_loadmod").m_unicodestring, UnicodeString(thisMod).m_unicodestring );
        LoadModButton *button = new LoadModButton();
        button->SetShortProperties( "dialog_loadmod", 20, y+=h, m_w-40, GetMenuSize(20), UnicodeString(caption) );
        button->m_modName = thisMod;
        button->m_fontSize = GetMenuSize(11);
        button->m_centered = true;
        RegisterButton( button );
		m_buttonOrder.PutData( button );
    }
    
    LoadModButton *loadNone = new LoadModButton();
    loadNone->SetShortProperties( "dialog_unloadmod", 10, m_h-GetMenuSize(105), m_w-20, GetMenuSize(20), LANGUAGEPHRASE("dialog_unloadmod") );
    loadNone->m_modName = strdup( "none" );
    loadNone->m_fontSize = fontSize;
    loadNone->m_centered = true;
    RegisterButton( loadNone );
	m_buttonOrder.PutData(loadNone);

    NewModWindowButton *newMod = new NewModWindowButton();
    newMod->SetShortProperties( "dialog_createnewmod", 10, m_h-GetMenuSize(80), m_w-20, GetMenuSize(20), LANGUAGEPHRASE("dialog_createnewmod") );
    newMod->m_fontSize = fontSize;
    newMod->m_centered = true;
    RegisterButton( newMod );
	m_buttonOrder.PutData(newMod);

    wchar_t toolsMenuName[256];
	swprintf( toolsMenuName, sizeof(toolsMenuName)/sizeof(wchar_t), L"%ls (F9)", LANGUAGEPHRASE("editor_toggleeditor").m_unicodestring );

    ToolsButton *tools = new ToolsButton();
    tools->SetShortProperties( "editor_toggleeditor", 10, m_h-GetMenuSize(55), m_w-20, GetMenuSize(20), UnicodeString(toolsMenuName) );
    tools->m_fontSize = fontSize;
    tools->m_centered = true;
    RegisterButton( tools );
	m_buttonOrder.PutData(tools);

	CloseButton *close = new CloseButton();
	close->SetShortProperties( "dialog_close", 10, m_h-GetMenuSize(30), m_w-20, GetMenuSize(20), LANGUAGEPHRASE("dialog_close") );
	close->m_fontSize = fontSize;
	close->m_centered = true;
	RegisterButton(close);
	m_buttonOrder.PutData(close);
}



// ============================================================================

class NewModButton : public DarwiniaButton
{
    void MouseUp()
    {
        NewModWindow *parent = (NewModWindow *) m_parent;
        g_app->m_resource->LoadMod( parent->s_modName );

        g_app->SetProfileName( "none" );
        g_app->LoadProfile();

        if( g_app->m_globalWorld )
        {
            delete g_app->m_globalWorld;
            g_app->m_globalWorld = NULL;
        }
    
        g_app->m_globalWorld = new GlobalWorld();
        for( int i = 0; i < GlobalResearch::NumResearchItems; ++i )
        {
            g_app->m_globalWorld->m_research->m_researchLevel[i] = 2;
        }        
        g_app->m_globalWorld->SaveGame( "game.txt" );

        EclRemoveWindow( m_parent->m_name );
        EclRemoveWindow( "dialog_mods" );
        EclRemoveWindow( "dialog_mainmenu" );

        g_app->m_requestToggleEditing = true;
    }
};

char NewModWindow::s_modName[] = "MyNewMod";


NewModWindow::NewModWindow()
:   DarwiniaWindow( "dialog_createnewmod" )
{

}

 
void NewModWindow::Create()
{
    SetMenuSize( 300, 110 );
	SetPosition( g_app->m_renderer->ScreenW()/2 - m_w/2, 
                 g_app->m_renderer->ScreenH()/2 - m_h/2 );

    DarwiniaWindow::Create();

    InvertedBox *box = new InvertedBox();
    box->SetShortProperties( "box", 10, GetMenuSize(30), m_w-20, GetMenuSize(40), UnicodeString("box") );
    RegisterButton( box );
    
    CreateValueControl( "dialog_name", s_modName, GetMenuSize(40), 0, 0, sizeof(s_modName), NULL, 20, m_w-40 );

	int y = m_h - 30;

    CloseButton *close = new CloseButton();
    close->SetShortProperties( "dialog_cancel", 10, y, m_w/2-15, GetMenuSize(20), LANGUAGEPHRASE("dialog_cancel") );
	close->m_fontSize = GetMenuSize(11);
    RegisterButton( close );
    
    NewModButton *newProfile = new NewModButton();
    newProfile->SetShortProperties( "dialog_create", close->m_x+close->m_w+10, y, m_w/2-15, GetMenuSize(20), LANGUAGEPHRASE("dialog_create") );
	newProfile->m_fontSize = GetMenuSize(11);
    RegisterButton( newProfile );
}

#endif // USE_DARWINIA_MOD_SYSTEM
