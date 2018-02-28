#ifndef _IDOL_SENTENCE_BREAKING_EXAMPLE_STEMMER_HPP_
#define _IDOL_SENTENCE_BREAKING_EXAMPLE_STEMMER_HPP_ 

/* IDOL Sentence Breaking example stemming
 */

#include "idol_sentence_breaking.h"

#include <algorithm>
#include <string>

namespace {
    /* 
    * This example provides custom stemming for a small number of non-standard plurals
    * To use the full sentence breaking syntax, return token in form 1x2x3x4x5 (where x is IDOL_SB_MARK)
    * stemmed form:   124
    * unbroken form:  134
    * unstemmed form: 154
    */
    void example_stemmer(std::string& token) {
        // transform to lowercase
        std::string data = token;
        std::transform(data.begin(), data.end(), data.begin(), ::tolower);

        if ("mice" == data) {
            token = "" IDOL_SB_MARK "MOUS" IDOL_SB_MARK + token + IDOL_SB_MARK "" IDOL_SB_MARK "mice";
        }
        else if ("geese" == data) {
            token = "" IDOL_SB_MARK "GOOS" IDOL_SB_MARK + token + IDOL_SB_MARK "" IDOL_SB_MARK "geese";
        }
        else if ("octopodes" == data) {
            token = "" IDOL_SB_MARK "OCTOPU" IDOL_SB_MARK + token + IDOL_SB_MARK "" IDOL_SB_MARK "octopodes";
        }

        return; // unmodified - use default stemming
    }
}
#endif
