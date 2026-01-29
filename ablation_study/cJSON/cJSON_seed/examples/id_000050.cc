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
//<ID> 50
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
    cJSON_AddItemToObject(root, "values", array);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.718);
    cJSON *n3 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(array);
    cJSON *size_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "size", size_item);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}