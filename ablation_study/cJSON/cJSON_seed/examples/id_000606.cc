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
//<ID> 606
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    const char *names_arr[] = { "alpha", "beta", "gamma" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_true", flag_true);
    cJSON_AddItemToObject(root, "flag_false", flag_false);

    // step 2: Configure
    int orig_count = cJSON_GetArraySize(names);
    cJSON_AddNumberToObject(root, "count", (double)orig_count);
    cJSON_AddStringToObject(root, "creator", "tester");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_names = cJSON_HasObjectItem(parsed, "names");
    cJSON *parsed_names = cJSON_GetObjectItem(parsed, "names");
    cJSON *first_name_item = cJSON_GetArrayItem(parsed_names, 0);
    const char *first_name = cJSON_GetStringValue(first_name_item);
    cJSON *parsed_flag_true = cJSON_GetObjectItem(parsed, "flag_true");
    cJSON *parsed_flag_false = cJSON_GetObjectItem(parsed, "flag_false");
    cJSON_bool parsed_is_true = cJSON_IsTrue(parsed_flag_true);
    cJSON_bool parsed_is_false = cJSON_IsFalse(parsed_flag_false);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has_names=%d;count=%d;first=%s;true=%d;false=%d",
            version ? version : "null",
            (int)has_names,
            orig_count,
            first_name ? first_name : "null",
            (int)parsed_is_true,
            (int)parsed_is_false);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}