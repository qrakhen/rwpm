#pragma once

#include <vector>
#include <TlHelp32.h>

#define streql 0

using namespace std;

class MEM
{
private:
    HANDLE hPrcs;
    DWORD dwPrcsId;
    vector<MODULEENTRY32> modules;

public:
    MEM();
    MEM(const char* target);

    template <typename T>
    inline bool read(DWORD dwAddr, T& v) {
        return ReadProcessMemory(hPrcs, (PBYTE*)dwAddr, &v, sizeof(T), NULL) == TRUE;
    }

    template <typename T>
    inline bool write(DWORD dwAddr, T& v) {
        return WriteProcessMemory(hPrcs, (PBYTE*)dwAddr, &v, sizeof(T), NULL) == TRUE;
    }

    bool bind(const char* target);
    bool load(const char*, MODULEENTRY32&);

    void dispose();

    inline DWORD getModule(const char* name) {
        for (MODULEENTRY32 m : modules) {
            if (0 == strcmp(m.szModule, name)) {
                return reinterpret_cast<DWORD>(m.modBaseAddr);
            }
        }
        return 0x0;
    }
    
    HANDLE getHandle() {
        return this->hPrcs;
    }

    DWORD getPrcsId() {
        return this->dwPrcsId;
    }

    vector<MODULEENTRY32> getModules() {
        return this->modules;
    }
};

class RWPM 
{
private:

public:
    template <typename T>
    static inline T read(DWORD dwAddr) {
        T r;
        getInstance()->read<T>(dwAddr, r);
        return r;
    }

    template <typename T>
    static inline bool read(DWORD dwAddr, T& v) {
        return getInstance()->read<T>(dwAddr, v);
    }

    template <typename T>
    static inline bool write(DWORD dwAddr, T& v) {
        return getInstance()->write<T>(dwAddr, v);
    }

    static MEM* getInstance(bool reset = false) {
        static MEM mem;
        if (reset) mem = MEM();
        return &mem;
    }

    static bool init(const char* target) {
        try {
            getInstance()->dispose();
            getInstance()->bind(target);
            return true;
        }
        catch (exception e) {
            throw e;
        }
    }

    static MODULEENTRY32 loadModule(const char* target) {
        MODULEENTRY32 mdlEntry;
        if (getInstance()->load(target, mdlEntry)) return mdlEntry;
        else throw;
    }

    static DWORD getModule(const char* name) {
        DWORD dwModule;
        if (dwModule = getInstance()->getModule(name) == 0x0) {
            return reinterpret_cast<DWORD>(RWPM::loadModule(name).modBaseAddr);
        }
        else return dwModule;
    }
};
