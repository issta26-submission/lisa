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
//<ID> 252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    const char *words_values[] = { "alpha", "beta", "gamma" };
    cJSON *words = cJSON_CreateStringArray(words_values, 3);
    cJSON *old_threshold = cJSON_CreateNumber(10.5);
    cJSON *label = cJSON_CreateString("primary");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "settings", settings);
    cJSON_AddItemToObjectCS(root, "words", words);
    cJSON_AddItemToObjectCS(settings, "threshold", old_threshold);
    cJSON_AddItemToObjectCS(settings, "label", label);

    // step 3: Operate & Validate
    cJSON *settings_ptr = cJSON_GetObjectItem(root, "settings");
    cJSON *threshold_ptr = cJSON_GetObjectItem(settings_ptr, "threshold");
    cJSON *new_threshold = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(settings_ptr, threshold_ptr, new_threshold);
    (void)replaced;
    cJSON_bool has_words = cJSON_HasObjectItem(root, "words");
    (void)has_words;
    double current_threshold = cJSON_GetNumberValue(cJSON_GetObjectItem(settings_ptr, "threshold"));
    cJSON *threshold_copy = cJSON_CreateNumber(current_threshold);
    cJSON_AddItemToObjectCS(root, "threshold_copy", threshold_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}