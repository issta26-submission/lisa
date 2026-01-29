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
//<ID> 1382
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
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "arr_copy", arr_dup);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "added_true", true_item);

    // step 3: Operate
    cJSON_bool replace_result = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON_AddItemToObject(root, "source_arr", cJSON_CreateArrayReference(arr));
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON *replaced_item = cJSON_GetArrayItem(arr, 1);
    const char *replaced_str = cJSON_GetStringValue(replaced_item);
    (void)replace_result;
    (void)replaced_str;
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}