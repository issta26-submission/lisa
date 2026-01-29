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
//<ID> 1366
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
    cJSON *raw = cJSON_CreateRaw("{\"raw\":\"value\"}");
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "source", "fuzzer");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    char *first_str = cJSON_GetStringValue(first);
    double second_num = cJSON_GetNumberValue(second);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = first_str ? first_str[0] : 0;
    ((char *)scratch)[2] = (char)('0' + (arr_size % 10));
    ((char *)scratch)[3] = (char)('0' + (((int)second_num) % 10));
    ((char *)scratch)[4] = 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}