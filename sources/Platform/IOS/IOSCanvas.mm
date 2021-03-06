/*
 * IOSCanvas.mm
 * 
 * This file is part of the "LLGL" project (Copyright (c) 2015 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IOSCanvas.h"
#include <LLGL/Platform/NativeHandle.h>


/*@interface AppDelegate : NSObject

- (id)initWithWindow:(LLGL::IOSCanvas*)window isResizable:(BOOL)resizable;
- (BOOL)isQuit;

@end

@implementation AppDelegate
{
    LLGL::IOSCanvas*  window_;
    BOOL                resizable_;
    BOOL                quit_;
}

- (id)initWithWindow:(LLGL::IOSCanvas*)window isResizable:(BOOL)resizable
{
    self = [super init];
    
    window_     = window;
    resizable_  = resizable;
    quit_       = FALSE;
    
    return (self);
}

- (void)windowWillClose:(id)sender
{
    window_->PostQuit();
    quit_ = TRUE;
}

- (NSSize)windowWillResize:(NSWindow*)sender toSize:(NSSize)frameSize
{
    if (resizable_)
        return frameSize;
    else
        return [sender frame].size;
}

- (void)windowDidResize:(NSNotification*)notification
{
    NSWindow* sender = [notification object];
    NSRect frame = [sender frame];
    window_->PostResize({ (int)frame.size.width, (int)frame.size.height });
}

- (BOOL) isQuit
{
    return (quit_);
}

@end*/


namespace LLGL
{


/*static NSString* ToNSString(const wchar_t* s)
{
    return [[NSString alloc]
        initWithBytes: s
        length: sizeof(*s)*wcslen(s)
        encoding:NSUTF32LittleEndianStringEncoding
    ];
}*/

std::unique_ptr<Canvas> Canvas::Create(const CanvasDescriptor& desc)
{
    return std::unique_ptr<Canvas>(new IOSCanvas(desc));
}

IOSCanvas::IOSCanvas(const CanvasDescriptor& desc) :
    desc_ { desc               }/*,
    view_ { CreateUIView(desc) }*/
{
}

IOSCanvas::~IOSCanvas()
{
}

void IOSCanvas::GetNativeHandle(void* nativeHandle) const
{
    //auto& handle = *reinterpret_cast<NativeHandle*>(nativeHandle);
    //handle.window = wnd_;
}

void IOSCanvas::Recreate()
{
    //todo...
}

Size IOSCanvas::GetContentSize() const
{
    return {}; //todo...
}

void IOSCanvas::SetTitle(const std::wstring& title)
{
    //todo...
}

std::wstring IOSCanvas::GetTitle() const
{
    return L""; //todo...
}


/*
 * ======= Private: =======
 */

void IOSCanvas::OnProcessEvents()
{
    //TODO...
}


} // /namespace LLGL



// ================================================================================
