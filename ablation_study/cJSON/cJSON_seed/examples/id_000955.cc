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
//<ID> 955
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    const char *version = cJSON_Version();
    cJSON *ver_str = cJSON_CreateString(version);
    cJSON *numA = cJSON_CreateNumber(3.1415);
    cJSON *numB = cJSON_CreateNumber(2.718);
    cJSON *raw = cJSON_CreateRaw("{\"sensor\":true,\"v\":1}");

    // step 2: Configure
    cJSON_AddItemToArray(arr, numA);
    cJSON_AddItemToArray(arr, numB);
    cJSON_AddItemToObject(info, "raw", raw);
    cJSON_AddItemToObject(root, "version", ver_str);
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double val = cJSON_GetNumberValue(first);
    cJSON_AddItemToObject(root, "first_value", cJSON_CreateNumber(val));
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw);
    cJSON_AddItemToObject(root, "raw_is_raw", cJSON_CreateBool(raw_is_raw));
    const char *version_again = cJSON_Version();
    cJSON_AddItemToObject(root, "version_repeat", cJSON_CreateString(version_again));
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}