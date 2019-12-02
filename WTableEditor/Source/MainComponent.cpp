/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
    setApplicationCommandManagerToWatch(&commandManager);
    addKeyListener(commandManager.getKeyMappings());
    
    commandManager.registerAllCommandsForTarget(this);
    
#ifdef __APPLE__
    setMacMainMenu(this);
#else
    setMenuBar(this);
#endif

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}


StringArray MainComponent::getMenuBarNames()
{
    return {
        "File"
    };
}

PopupMenu MainComponent::getMenuForIndex(int index, const String& name)
{
    PopupMenu menu;
    if (name == "File")
    {
        menu.addCommandItem(&commandManager, static_cast<CommandID>(MenuId::FileNew));
        menu.addCommandItem(&commandManager, static_cast<CommandID>(MenuId::FileOpen));
        menu.addCommandItem(&commandManager, static_cast<CommandID>(MenuId::FileSave));
        menu.addSeparator();
        menu.addCommandItem(&commandManager, static_cast<CommandID>(MenuId::FileExit));
    }
    return menu;
}

void MainComponent::menuItemSelected(int menuId, int index)
{
    switch(static_cast<MenuId>(menuId))
    {
    case MenuId::FileNew:
        break;
    case MenuId::FileOpen:
        break;
    case MenuId::FileSave:
        break;
    case MenuId::FileExit:
        break;
    }
}

void MainComponent::getAllCommands(Array<CommandID>& c)
{
    Array<CommandID> commands {
        static_cast<CommandID>(MenuId::FileNew),
        static_cast<CommandID>(MenuId::FileOpen),
        static_cast<CommandID>(MenuId::FileSave),
        static_cast<CommandID>(MenuId::FileExit)
    };
    c.addArray(commands);
}

ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
    return nullptr;
}

void MainComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
    switch(static_cast<MenuId>(commandID))
    {
    case MenuId::FileNew:
        result.setInfo("New...", "", "Menu", 0);
        result.addDefaultKeypress('N', ModifierKeys::commandModifier);
        break;
    case MenuId::FileOpen:
        result.setInfo("Open...", "", "Menu", 0);
        result.addDefaultKeypress('O', ModifierKeys::commandModifier);
        break;
    case MenuId::FileSave:
        result.setInfo("Save...", "", "Menu", 0);
        result.addDefaultKeypress('S', ModifierKeys::commandModifier);
        break;
    case MenuId::FileExit:
        result.setInfo("Exit", "", "Menu", 0);
        result.addDefaultKeypress('Q', ModifierKeys::commandModifier);
        break;
    }
}

bool MainComponent::perform(const InvocationInfo& info)
{
    switch (static_cast<MenuId>(info.commandID))
    {
    case MenuId::FileNew:
        onNew();
        break;
    case MenuId::FileOpen:
        onOpen();
        break;
    case MenuId::FileSave:
        break;
    case MenuId::FileExit:
        JUCEApplication::getInstance()->systemRequestedQuit();
        break;
    }
    return true;
}

void MainComponent::onNew()
{
    
}

void MainComponent::onOpen()
{
    FileChooser chooser("Please select the wavetable file you want to load...",
                        File::getSpecialLocation (File::userHomeDirectory),
                        "*.wtf");
    if (chooser.browseForFileToOpen())
    {
        File wtFile(chooser.getResult());
        auto is = wtFile.createInputStream();
        std::vector<char> header(4);
        is->read(header.data(), 4);
        uint32_t tableSize;
        is->read(&tableSize, 4);
        std::vector<float> tableData(tableSize);
        is->read(tableData.data(), tableSize);
        
        //TODO pass to editor
    }
}

void MainComponent::onSave()
{
    FileChooser chooser("Please select where to save the wavetable file...",
                        File::getSpecialLocation (File::userHomeDirectory),
                        "*.wtf");
    if (chooser.browseForFileToSave(true))
    {
        File wtFile(chooser.getResult());
        auto os = wtFile.createOutputStream();
        std::string header = "OKWF";
        os->write(header.c_str(), header.size());
        
        //TODO get data from editor
        
        uint32_t tableSize = 0;
        os->write(&tableSize, sizeof(tableSize));
        std::vector<float> tableData(tableSize);
        os->write(tableData.data(), tableSize);
    }
}
