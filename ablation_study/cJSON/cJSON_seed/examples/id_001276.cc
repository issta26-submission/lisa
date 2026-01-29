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
//<ID> 1276
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
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON *label = cJSON_CreateString("example_label");

    // step 2: Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNullToObject(config, "optional");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *count = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", count);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}