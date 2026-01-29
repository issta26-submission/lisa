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
//<ID> 1057
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", bfalse);
    cJSON *null_item = cJSON_AddNullToObject(root, "missing");

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", size_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    (void)null_item;
    return 66; // API sequence test completed successfully
}