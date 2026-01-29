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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON *str = cJSON_CreateString("hello");
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToArray(arr, nul);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "meta_null", cJSON_CreateNull());

    // step 2: Configure (serialize then prepare a buffer for minification)
    char *json_unformatted = cJSON_PrintUnformatted(root);
    size_t buf_len = json_unformatted ? (strlen(json_unformatted) + 1) : 1;
    char *buf = (char *)cJSON_malloc(buf_len);
    memset(buf, 0, buf_len);
    if (json_unformatted) {
        memcpy(buf, json_unformatted, buf_len);
    }

    // step 3: Operate and Validate
    cJSON_Minify(buf);
    cJSON *parsed = cJSON_Parse(buf);
    cJSON *parsed_arr = parsed ? cJSON_GetObjectItem(parsed, "arr") : NULL;
    int arr_size_original = cJSON_GetArraySize(arr);
    int arr_size_parsed = cJSON_GetArraySize(parsed_arr);
    cJSON *first_orig = cJSON_GetArrayItem(arr, 0);
    cJSON *first_parsed = cJSON_GetArrayItem(parsed_arr, 0);
    double val_orig = cJSON_GetNumberValue(first_orig);
    double val_parsed = cJSON_GetNumberValue(first_parsed);
    const char *s_orig = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    const char *s_parsed = cJSON_GetStringValue(cJSON_GetArrayItem(parsed_arr, 1));
    cJSON_bool null_is_null = cJSON_IsNull(cJSON_GetArrayItem(arr, 2));
    cJSON_bool parsed_null_is_null = parsed ? cJSON_IsNull(cJSON_GetArrayItem(parsed_arr, 2)) : 0;
    int summary = arr_size_original + arr_size_parsed + (int)(val_orig == val_parsed) + (int)(s_orig && s_parsed) + (int)null_is_null + (int)parsed_null_is_null;
    (void)summary;

    // step 4: Cleanup
    if (json_unformatted) cJSON_free(json_unformatted);
    cJSON_free(buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}