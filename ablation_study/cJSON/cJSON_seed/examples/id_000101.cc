#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const words[] = { "alpha", "beta", "gamma" };
    cJSON *words_array = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "words", words_array);

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    const char source_json[] = "{ \"nested\" : { \"value\" : \"x\" } }";
    char *raw_buf = (char *)cJSON_malloc(sizeof(source_json));
    memcpy(raw_buf, source_json, sizeof(source_json));
    cJSON_Minify(raw_buf);
    cJSON *parsed = cJSON_Parse(raw_buf);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate & Validate
    cJSON_bool has_greeting = cJSON_HasObjectItem(root, "greeting");
    cJSON_bool has_words = cJSON_HasObjectItem(root, "words");
    const char *greet_text = cJSON_GetStringValue(greeting);
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToArray(flags, cJSON_CreateNumber((double)has_greeting));
    cJSON_AddItemToArray(flags, cJSON_CreateNumber((double)has_words));
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *label = cJSON_CreateString(greet_text ? greet_text : "");
    cJSON_AddItemToObject(root, "label", label);

    // step 4: Cleanup
    cJSON_free(raw_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}