//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

MainWindow::MainWindow(String name)
: DocumentWindow(name, Colours::white, DocumentWindow::allButtons) {
   setUsingNativeTitleBar(true);
   setResizable(false, false);
   setContentOwned(new MainComponent(), false);
    centreWithSize(392, 120);
   setVisible(true);
////
//#if JUCE_WINDOWS || JUCE_LINUX
//  setMenuBar(MainApplication::getApp().getMenuBarModel());
//
//#endif
//
//  // add our content component. owned means that when the window is
//  // deleted it will automatically delete our content component
////  auto maincomp = std::make_unique<MainComponent>();
////  setContentOwned(maincomp.release(), false);
//#if JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX
//
//        setResizable(true, true);
//        centreWithSize(392, 120);
//       // setResizeLimits(500, 250, 1000, 500);
//        centreWithSize(getWidth(), getHeight());
//
//    #else // JUCE_IOS || JUCE_ANDROID
//         setFullScreen (true);
//    #endif
//      // make the window visible
//      setVisible(true);
    
    
    
}

//==============================================================================
// DocumentWindow overrides

void MainWindow::closeButtonPressed() {
    MainApplication::getApp().systemRequestedQuit();
}

