#ifndef _IDOL_SENTENCE_BREAKING_H_
#define _IDOL_SENTENCE_BREAKING_H_

/* IDOL Sentence Breaking header file
 */

#ifdef WIN32
#define KBAPI extern "C" __declspec(dllexport)
#else
#define KBAPI extern "C"
#endif

#include "stdarg.h"
#include "stdlib.h"

// sentence breaking syntax mark
#define IDOL_SB_MARK "\xE2\x8A\xA1"

// error codes the sentence breaking implementation may return
#define IDOL_SB_ERROR_SUCCESS           0
#define IDOL_SB_ERROR_INIT_ERROR        -1
#define IDOL_SB_ERROR_BREAKING_ERROR    -2
#define IDOL_SB_ERROR_UNKNOWN           -3
#define IDOL_SB_ERROR_ENCODING_ERROR    -4
#define IDOL_SB_ERROR_INVALID_INPUT     -5
#define IDOL_SB_ERROR_ASSERT            -6
#define IDOL_SB_ERROR_DIR_NOT_FOUND     -7
#define IDOL_SB_ERROR_LIB_ALREADY_INIT  -8
#define IDOL_SB_ERROR_MEMORY            -9
#define IDOL_SB_ERROR_FILE_NOT_FOUND    -10

typedef struct {
    char *szOptions;                                    /* SentenceBreakingOptions config parameter */
    char *szNonBreakCharacters;                         /* characters that should not be broken (e.g. wildcard at query time) */
    char *szFullPathDirectory;                          /* Language directory path */
    void(*fnLogFunction)(const char *fmt, va_list ap);  /* If non-NULL, a function the library can use to output messages to log file*/
    int bQueryMode;                                     /* Is the breaking function being called in a query or non-query context */
    int nLangCode;                                      /* Language identifier, used if library supports multiple languages */
} t_lang2SentenceBreakingParam;

/* --- The following functions all need implementing --- */

/* Must return static string, sentence breaking library information */
KBAPI char* sentenceBreakGetInfo();

/* Called when library is loaded to perform required initialization */
KBAPI long sentenceBreakInitialise(char *szLangDirectory, char **pszErrorDescription);

/* Called on shutdown if library has KEEPRESIDENT="Y" */
KBAPI long sentenceBreakUninitialise(char **pszErrorDescription);

/* Release memory associated with error description */
KBAPI long sentenceBreakFreeErrorDescription(char **pszErrorDescription);

/**
 * @param szInput Input text to be sentence broken
 * @param pszOutput Returns the sentence broken output
 * @param pnOutputLength Returns the number of bytes allocated in *pszOutput
 */
KBAPI long sentenceBreakSelfAllocating(char *szInput, char **pszOutput, size_t *pnOutputLength, t_lang2SentenceBreakingParam *pParam);

/* Release memory from sentenceBreakSelfAllocating */
KBAPI void sentenceBreakFreeBuffer(char **pszBuffer);

#endif
