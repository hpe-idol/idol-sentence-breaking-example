
/* IDOL Sentence Breaking example implementation
 * This implements a sentence breaking library that allows
 * for customized stemming - edit the example_stemmer in
 * idol_sentence_breaking_example_stemmer.hpp
 */

#include "idol_sentence_breaking.h"

#include "idol_sentence_breaking_example_stemmer.hpp"

#include <memory>
#include <sstream>
#include <string>
#include <string.h>

namespace {
    char * strDuplicate(const char * szOriginal, size_t *pnLen) {
        if (szOriginal) {
            size_t nLen = strlen(szOriginal) + 1;
            char * szReturn = (char*)malloc(nLen);
            if (szReturn) {
                memcpy(szReturn, szOriginal, nLen);
                *pnLen = nLen;
                return szReturn;
            }
        }
        return 0;
    }

    void closeToken(std::stringstream & output, std::string token, int nLangCode) {
        if (token.length() == 0) {
            return;
        }
        switch (nLangCode) {
        default: example_stemmer(token); break;
        }
        output << token;
    }
}

KBAPI char* sentenceBreakGetInfo() {
    static std::string info = R"info(<?xml version="1.0" encoding="UTF-8" ?>
<ROOT>
<VERSION NAME="Eng-Example" MAJOR="1" MINOR="0" PATCH="0" DATE="8 February 2018" REVISION="10000"/>
<PRIMARYLANGUAGE NAME="English" DEFAULTENCODING="UTF8"/>
<RUNTIME THREADSAFE="N" HIGHLIGHT="NQSP"/>
<OPTIONS />
</ROOT>
)info";
    return const_cast<char*>(info.c_str());
}

KBAPI long sentenceBreakInitialise(char *szLangDirectory, char **pszErrorDescription) {
    if (!szLangDirectory || !pszErrorDescription) return IDOL_SB_ERROR_INIT_ERROR;
    return IDOL_SB_ERROR_SUCCESS;
}

KBAPI long sentenceBreakUninitialise(char **pszErrorDescription) {
    return IDOL_SB_ERROR_SUCCESS;
}

KBAPI long sentenceBreakFreeErrorDescription(char **pszErrorDescription) {
    if (pszErrorDescription && *pszErrorDescription) {
        free(*pszErrorDescription);
        *pszErrorDescription = 0;
    }
    return IDOL_SB_ERROR_SUCCESS;
}

KBAPI long sentenceBreakSelfAllocating(char *szInput, char **pszOutput, size_t *pnOutputLength,
    t_lang2SentenceBreakingParam *pParam) {
    int nRet = IDOL_SB_ERROR_SUCCESS;
    char *cin = szInput;

    std::stringstream output;
    std::string separators(R"s( ,.<>[]{}()!"$%^&=+|:;)s" + std::string("\t\r\n"));    // strictly language dependent
        
    if (pParam->bQueryMode) {
        std::string leadingseps(separators + "'~"); 
        while (*cin) {
            size_t leading_separators = strspn(cin, leadingseps.c_str());
            if (leading_separators) {
                output << std::string(cin, leading_separators);
                cin += leading_separators;
            }
            size_t not_separators = strcspn(cin, separators.c_str());
            if (not_separators) {
                std::string curtoken = std::string(cin, not_separators);
                if (strchr(curtoken.c_str(), '*') || strchr(curtoken.c_str(), '?') || '~'==curtoken.back()) {
                    // wildcard or stemmed form - do not modify
                    output << std::string(cin, not_separators);
                }
                else {
                    closeToken(output, curtoken, pParam->nLangCode);
                }
                cin += not_separators;
            }

            // deal with possible syntax after the token
            while (*cin) {
                if ('[' == *cin) {
                    // weight or occurrence restriction
                    char * close = strchr(cin, ']');
                    if (!close) {
                        // malformed query syntax - try to recover
                        close = strpbrk(cin, " \t\r\n");
                    }
                    if (close) {
                        output << std::string(cin, 1 + close - cin);
                        cin = close + 1;
                    }
                    else {
                        // treat as end of buffer
                        output << std::string(cin);
                        cin += strlen(cin);
                    }
                    continue;
                }
                if (')' == *cin || '"' == *cin) {
                    // closing bracket/quote, but might be followed by weight/occ/fieldrestriction
                    output << *cin;
                    ++cin;
                    continue;
                }
                if (':' == *cin) {
                    // fieldrestriction - expect terminated by whitespace
                    char * whitespace = strpbrk(cin, " \t\r\n");
                    if (whitespace) {
                        output << std::string(cin, 1 + whitespace - cin);
                        cin = whitespace + 1;
                    }
                    else {
                        // end of buffer
                        output << std::string(cin);
                        cin += strlen(cin);
                    }
                    break;
                }
                break;
            }
        } // end while
    } 
    else {  // indexing mode
        std::string indexseps(":~*?/");
        std::string leadingseps(separators + indexseps + "'");
        std::string nottoken(separators + indexseps);
        while (*cin) {
            size_t leading_separators = strspn(cin, leadingseps.c_str());
            if (leading_separators) {
                output << std::string(cin, leading_separators);
                cin += leading_separators;
            }
            size_t not_separators = strcspn(cin, nottoken.c_str());
            if (not_separators) {
                std::string curtoken = std::string(cin, not_separators);
                closeToken(output, curtoken, pParam->nLangCode);
                cin += not_separators;
            }
        }
    }

    *pszOutput = strDuplicate(output.str().c_str(), pnOutputLength);
    return  *pszOutput ? IDOL_SB_ERROR_SUCCESS : IDOL_SB_ERROR_MEMORY;
}

KBAPI void sentenceBreakFreeBuffer(char **pszBuffer) {
    if (pszBuffer && *pszBuffer) {
        free(*pszBuffer);
        *pszBuffer = 0;
    }
}

#ifdef WIN32
#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        /*
        Shouldn't really call any fns here which do calls to system dlls.
        */
        return TRUE;
        break;
    case DLL_PROCESS_DETACH:
        return TRUE;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        /*
        Do nothing - any special cases for thread attaching?
        */
        return TRUE;
    }
}
#endif

