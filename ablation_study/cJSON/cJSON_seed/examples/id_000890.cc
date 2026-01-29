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
//<ID> 890
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
    cJSON *raw = cJSON_CreateRaw("{\"inner\":\"raw\",\"v\":123}");
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.71);

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, raw);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "values", array);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(array);
    cJSON *count_item = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_item);
    char *raw_string = cJSON_GetStringValue(raw);
    cJSON *raw_copy = cJSON_CreateString(raw_string);
    cJSON_AddItemToObject(root, "raw_copy", raw_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}