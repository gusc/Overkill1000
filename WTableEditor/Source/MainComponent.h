/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public MenuBarModel,
                        public ApplicationCommandTarget
{   
    enum class MenuId
    {
        FileNew = 1001,
        FileOpen,
        FileSave,
        FileExit
    };
    
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    void onNew();
    void onOpen();
    void onSave();
    
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex(int index, const String& name) override;
    void menuItemSelected(int menuId, int index) override;
    
    void getAllCommands(Array<CommandID>& c) override;
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
    bool perform(const InvocationInfo& info) override;
    

private:
    //==============================================================================
    // Your private member variables go here...

    ApplicationCommandManager commandManager;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
