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
//<ID> 118
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
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToArray(arr, raw);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, nul);
    size_t buf_size = 64;
    char *buf = (char *)cJSON_malloc(buf_size);
    memset(buf, 0, buf_size);
    strcpy(buf, "  {  \"min_key\"  :  \"min_value\"  }  ");
    cJSON_Minify(buf);
    cJSON *parsed = cJSON_Parse(buf);
    cJSON *minval = cJSON_GetObjectItem(parsed, "min_key");
    const char *min_text = cJSON_GetStringValue(minval);
    cJSON *min_label = cJSON_CreateString(min_text ? min_text : "");
    cJSON_AddItemToObject(root, "min_label", min_label);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool is_second_null = cJSON_IsNull(second);
    cJSON *is_null_num = cJSON_CreateNumber((double)is_second_null);
    cJSON_AddItemToObject(root, "second_is_null", is_null_num);
    cJSON *first_dup = cJSON_Duplicate(first, 1);
    cJSON_AddItemToObject(root, "first_dup", first_dup);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}