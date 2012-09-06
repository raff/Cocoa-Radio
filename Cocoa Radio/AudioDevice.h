//
//  audioDevice.h
//  Cocoa Radio
//
//  Created by William Dillon on 5/27/12.
//  Copyright (c) 2012. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <AudioToolbox/AudioQueue.h>

@interface AudioDevice : NSObject
{
    bool prepared;
    
    int hwSampleRate;
}

@property (readwrite) int sampleRate;
@property (readwrite) int blockSize;
@property (readwrite) bool running;
@property (readwrite) bool stereo;
@property (readwrite) int deviceID;

+ (NSArray *)deviceDict;

- (bool)prepare;
- (void)unprepare;

- (void)start;
- (void)stop;

@end

// This (audio input) hasn't been implemented yet.
// It isn't needed yet, as the rtl-sdr doesn't transmit.
@interface AudioSource : AudioDevice
{
}

//+ (size_t)bufferSizeWithQueue:(AudioQueueRef)audioQueue
//                         Desc:(AudioStreamBasicDescription)ASBDescription
//                      Seconds:(Float64)seconds;

@end

@class AudioSink;

static const int kNumberBuffers = 10;

struct AQPlayerState {
    AudioStreamBasicDescription    mDataFormat;
    AudioQueueRef                  mQueue;
    AudioQueueBufferRef            mBuffers[kNumberBuffers];
    SInt64                         mCurrentPacket;
    UInt32                         mNumPacketsToRead;
    AudioStreamPacketDescription  *mPacketDescs;
    __unsafe_unretained AudioSink *context;
};

typedef struct {
    void *buffer;
    size_t bufferSize;
    void *nextBuffer;
} audioFIFO_buffer_t;

typedef struct {
    audioFIFO_buffer_t *head;
    audioFIFO_buffer_t *unused;

    int used;
    int available;

    pthread_mutex_t lock;
} audioFIFO_t;

@interface AudioSink : AudioDevice
{
    size_t bufferSize;
//    NSMutableArray *bufferFIFO;
    NSCondition *bufferCondition;

    audioFIFO_t fifo;
    
//    struct AQPlayerState state;
    NSMutableData *playerStateData;
}

- (void)bufferData:(NSData *)data;

@end
