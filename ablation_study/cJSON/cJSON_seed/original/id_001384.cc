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
//<ID> 1384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[\"one\",\"two\",\"three\"],\"flag\":false} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 0);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *replacement = cJSON_CreateString("TWO");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(summary, "flag_was_bool", cJSON_CreateBool(flag_is_bool));
    cJSON_AddItemToObject(summary, "used_true", true_item);
    cJSON *item_after_replace = cJSON_GetArrayItem(arr, 1);
    char *replaced_value = cJSON_GetStringValue(item_after_replace);
    cJSON_AddItemToObject(summary, "replaced_value", cJSON_CreateString(replaced_value));

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(parsed);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(summary);
    // API sequence test completed successfully
    return 66;
}