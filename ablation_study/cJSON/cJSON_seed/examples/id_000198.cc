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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":\"value\"}");
    cJSON *str_item = cJSON_CreateString("a_string");

    // step 2: Configure
    cJSON_AddItemToArray(array, raw_item);
    cJSON_AddItemToArray(array, str_item);
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddItemToObject(inner, "inner_raw", cJSON_CreateRaw("42"));
    cJSON_AddItemToObject(root, "inner", inner);

    // step 3: Operate & Validate
    cJSON_bool raw_detected = cJSON_IsRaw(raw_item);
    int count = cJSON_GetArraySize(array);
    cJSON_AddItemToObject(root, "raw_detected", cJSON_CreateBool(raw_detected));
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber((double)count));
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}