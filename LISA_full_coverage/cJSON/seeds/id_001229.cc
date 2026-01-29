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
//<ID> 1229
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
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToArray(arr, str_item);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddItemToObject(inner, "inner_key", cJSON_CreateString("inner_value"));
    cJSON_AddItemToArray(arr, inner);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buf = (char *)cJSON_malloc(printed_len + 16);
    memset(buf, 0, printed_len + 16);
    if (printed && buf) memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    const char *version = cJSON_Version();
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddBoolToObject(root, "is_array", arr_is_array);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    (void)version;

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}