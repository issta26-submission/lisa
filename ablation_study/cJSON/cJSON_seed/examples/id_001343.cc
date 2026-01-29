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
//<ID> 1343
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("example");
    cJSON *raw_embedded = cJSON_CreateRaw("{\"embedded\":true}");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToArray(arr, raw_embedded);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *false_item = cJSON_AddFalseToObject(root, "ok");
    cJSON *raw_conf = cJSON_AddRawToObject(root, "raw_config", "{\"mode\":\"fast\"}");

    // step 3: Operate & Validate
    int list_size = cJSON_GetArraySize(arr);
    cJSON *size_item = cJSON_CreateNumber((double)list_size);
    cJSON_AddItemToObject(root, "list_size", size_item);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *printed_raw = cJSON_CreateRaw(printed);
    cJSON_AddItemToObject(root, "printed", printed_raw);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}