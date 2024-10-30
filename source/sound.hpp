#ifndef __SOUNDFX_H__
#define __SOUNDFX_H__

#ifdef HAS_SOUND
namespace snd {
    void init();
    void release();
    void playsound(int id);
}
#endif

#endif