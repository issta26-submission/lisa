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
//<ID> 1405
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, str1);
    cJSON_AddItemToObjectCS(root, "values", array);
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("beta");
    cJSON_ReplaceItemInArray(array, 1, replacement);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(array);
    cJSON *size_num = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObjectCS(root, "count", size_num);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}