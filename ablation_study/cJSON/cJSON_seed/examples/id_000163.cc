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
//<ID> 163
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
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON *owned_num = cJSON_CreateNumber(42.0);
    cJSON_SetNumberHelper(owned_num, 100.5);
    cJSON_AddItemToArray(arr, owned_num);
    cJSON *external_num = cJSON_CreateNumber(3.14);
    cJSON_AddItemReferenceToArray(arr, external_num);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *info = cJSON_CreateObject();
    cJSON *count_item = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(info, "count", count_item);
    cJSON_AddItemToObject(root, "info", info);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(external_num);

    // API sequence test completed successfully
    return 66;
}