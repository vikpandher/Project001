#pragma once

#include "AL/al.h"
#include "AL/alc.h"

#include "Engine/Logger.h"



#ifndef NDEBUG
#define alcCheckError(x) Project001::ALCCheckError(x, __FILENAME__ , __LINE__)
#define alCheckError() Project001::ALCheckError(__FILENAME__ , __LINE__)
#define alcLogError(x) Project001::ALCLogError(x, __FILENAME__ , __LINE__)
#define alLogError() Project001::ALLogError(__FILENAME__ , __LINE__)
#else
#define alcCheckError(x) (alcGetError(x) != ALC_NO_ERROR)
#define alCheckError() (alGetError() != AL_NO_ERROR)
#define alcLogError(x)
#define alLogError()
#endif



namespace Project001
{
    inline bool ALCCheckError(ALCdevice* devicePtr, const char* file, int line)
    {
        ALCenum error = alcGetError(devicePtr);
        if (error != ALC_NO_ERROR)
        {
            Logger::Error("OpenAL Context Error: %s %d %s", file, line, alcGetString(devicePtr, error));
            return true;
        }
        return false;
    }

    inline bool ALCheckError(const char* file, int line)
    {
        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            Logger::Error("OpenAL Error: %s %d %s", file, line, alGetString(error));
            return true;
        }
        return false;
    }

    inline void ALCLogError(ALCdevice* devicePtr, const char* file, int line)
    {
        ALCenum error = alcGetError(devicePtr);
        if (error != ALC_NO_ERROR)
        {
            Logger::Error("OpenAL Context Error: %s %d %s", file, line, alcGetString(devicePtr, error));
        }
    }

    inline void ALLogError(const char* file, int line)
    {
        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            Logger::Error("OpenAL Error: %s %d %s", file, line, alGetString(error));
        }
    }
}