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
//<ID> 263
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    char *raw_buf = (char *)cJSON_malloc((size_t)32);
    memset(raw_buf, 0, (size_t)32);
    raw_buf[0] = '{';
    raw_buf[1] = '"';
    raw_buf[2] = 'r';
    raw_buf[3] = '"';
    raw_buf[4] = ':';
    raw_buf[5] = '1';
    raw_buf[6] = '}';
    cJSON *raw = cJSON_CreateRaw((const char *)raw_buf);
    cJSON_free(raw_buf);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *third_item = cJSON_GetArrayItem(arr, 2);
    cJSON_bool third_is_raw = cJSON_IsRaw(third_item);
    cJSON *flag_replaced = cJSON_CreateNumber((double)replaced);
    cJSON *flag_third_raw = cJSON_CreateNumber((double)third_is_raw);
    cJSON_AddItemToObject(root, "replaced_flag", flag_replaced);
    cJSON_AddItemToObject(root, "third_is_raw_flag", flag_third_raw);
    const char *ver = cJSON_Version();
    cJSON *verstr = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "cjson_version", verstr);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}