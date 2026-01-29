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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    const char *const words[] = { "alpha", "beta", "gamma" };
    cJSON *str_arr = cJSON_CreateStringArray(words, 3);
    cJSON *nval = cJSON_CreateNumber(123.0);
    cJSON *s = cJSON_CreateString("initial");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(root, "strings", str_arr);
    cJSON_AddItemToObjectCS(root, "value", nval);
    cJSON_AddItemToObjectCS(root, "greeting", s);

    // step 3: Operate & Validate
    cJSON_bool has_strings = cJSON_HasObjectItem(root, "strings");
    cJSON *has_flag = cJSON_CreateNumber((double)has_strings);
    cJSON_AddItemToObjectCS(root, "has_strings", has_flag);
    double original_value = cJSON_GetNumberValue(nval);
    cJSON *value_copy = cJSON_CreateNumber(original_value);
    cJSON_AddItemToObjectCS(root, "value_copy", value_copy);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replace_ok = cJSON_ReplaceItemViaPointer(root, s, replacement);
    cJSON *replace_flag = cJSON_CreateNumber((double)replace_ok);
    cJSON_AddItemToObjectCS(root, "replaced_ok", replace_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}