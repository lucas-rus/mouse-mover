#import <Cocoa/Cocoa.h>
#include "Jiggler.h"

// Interface for the Status Bar Controller
@interface AppController : NSObject <NSApplicationDelegate>
@property (strong, nonatomic) NSStatusItem *statusItem;
@property (assign, nonatomic) Jiggler *jiggler;
@end

@implementation AppController

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    self.jiggler = new Jiggler();
    
    // Create Status Item
    self.statusItem = [[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength];
    
    // Set Icon (Programmatically drawn circle)
    [self updateIcon:NO];
    
    // Create Menu
    NSMenu *menu = [[NSMenu alloc] init];
    
    // Toggle Item
    NSMenuItem *toggleItem = [[NSMenuItem alloc] initWithTitle:@"Start" 
                                                        action:@selector(toggleAction:) 
                                                 keyEquivalent:@""];
    [toggleItem setTarget:self];
    [menu addItem:toggleItem];
    
    [menu addItem:[NSMenuItem separatorItem]];
    
    // Exit Item
    NSMenuItem *quitItem = [[NSMenuItem alloc] initWithTitle:@"Quit Mouse Mover" 
                                                      action:@selector(terminate:) 
                                               keyEquivalent:@"q"];
    [menu addItem:quitItem];
    
    self.statusItem.menu = menu;
}

- (void)updateIcon:(BOOL)active {
    NSImage *image = [NSImage imageWithSize:NSMakeSize(18, 18) flipped:NO drawingHandler:^BOOL(NSRect dstRect) {
        [[NSColor clearColor] set];
        NSRectFill(dstRect);
        
        // Draw circle
        NSBezierPath *path = [NSBezierPath bezierPathWithOvalInRect:NSInsetRect(dstRect, 4, 4)];
        
        if (active) {
            [[NSColor greenColor] set];
        } else {
            [[NSColor labelColor] set]; // Adapts to Dark/Light mode
        }
        [path fill];
        
        return YES;
    }];
    
    [image setTemplate:NO]; // Keep our specific colors
    self.statusItem.button.image = image;
}

- (void)toggleAction:(id)sender {
    NSMenuItem *item = (NSMenuItem *)sender;
    
    if (self.jiggler->isRunning()) {
        self.jiggler->stop();
        [item setTitle:@"Start"];
        [self updateIcon:NO];
    } else {
        self.jiggler->start();
        [item setTitle:@"Stop"];
        [self updateIcon:YES];
    }
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    if (self.jiggler) {
        delete self.jiggler;
    }
}

@end

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        NSApplication *app = [NSApplication sharedApplication];
        AppController *controller = [[AppController alloc] init];
        [app setDelegate:controller];
        [app run];
    }
    return 0;
}
