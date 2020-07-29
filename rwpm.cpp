
#include "stdafx.h"
#include "rwpm.h"

#include <cstdio>
#include <windows.h>

MEM::MEM() {
    this->hPrcs = INVALID_HANDLE_VALUE;
    this->dwPrcsId = 0;
    this->modules.clear();
}

MEM::MEM(const char* target) {
    this->hPrcs = INVALID_HANDLE_VALUE;
    this->dwPrcsId = 0;
    this->modules.clear();
    if (!bind(target)) throw;
}

bool MEM::bind(const char* target) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (hSnap == INVALID_HANDLE_VALUE) return false;
    PROCESSENTRY32 prcsEntry;
    prcsEntry.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnap, &prcsEntry)) {
        if (streql == strcmp(prcsEntry.szExeFile, target)) {
            CloseHandle(hSnap);
            this->hPrcs = OpenProcess(PROCESS_ALL_ACCESS, FALSE, prcsEntry.th32ProcessID);
            this->dwPrcsId = prcsEntry.th32ProcessID;
            return true;
        }
    }
    else {
        CloseHandle(hSnap);
        return false;
    }
    while (Process32Next(hSnap, &prcsEntry)) {
        if (streql == lstrcmp(prcsEntry.szExeFile, target)) {
            CloseHandle(hSnap);
            this->hPrcs = OpenProcess(PROCESS_ALL_ACCESS, FALSE, prcsEntry.th32ProcessID);
            this->dwPrcsId = prcsEntry.th32ProcessID;
            return true;
        }
    }
    CloseHandle(hSnap);
    return false;
}

bool MEM::load(const char* target, MODULEENTRY32& mdlEntry) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->dwPrcsId);
    if (hSnap == INVALID_HANDLE_VALUE) return false;
    mdlEntry.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(hSnap, &mdlEntry)) {
        if (streql == lstrcmp(mdlEntry.szModule, target)) {
            CloseHandle(hSnap);
            this->modules.push_back(mdlEntry);
            return false;
        }
    }
    else {
        CloseHandle(hSnap);
        return false;
    }
    while (Module32Next(hSnap, &mdlEntry)) {
        if (streql == lstrcmp(mdlEntry.szModule, target)) {
            CloseHandle(hSnap);
            this->modules.push_back(mdlEntry);
            return true;
        }
    }
    CloseHandle(hSnap);
    return false;
}

void MEM::dispose() {
    CloseHandle(hPrcs);
    this->dwPrcsId = 0;
    this->modules.clear();
}
