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
//<ID> 1362
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
    cJSON *num = cJSON_AddNumberToObject(root, "num", 42.5);
    cJSON *raw_item = cJSON_CreateRaw("{\"in_raw\":123}");
    cJSON_AddItemToArray(arr, raw_item);
    cJSON *str_item = cJSON_CreateString("example_string");
    cJSON_AddItemToArray(arr, str_item);
    cJSON *arr_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, arr_num);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "note", "configured");
    cJSON_AddItemToObject(root, "meta", meta);
    char *out = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    double main_num = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "num"));
    const char *second_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    double third_num = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out ? out[0] : 0;
    buf[1] = second_str ? second_str[0] : 0;
    buf[2] = (char)('0' + ((int)main_num % 10));
    buf[3] = (char)('0' + ((int)third_num % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}