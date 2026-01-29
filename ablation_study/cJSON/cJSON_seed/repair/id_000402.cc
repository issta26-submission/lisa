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
//<ID> 402
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_arr);
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    int size = cJSON_GetArraySize(got_items);
    cJSON *first = cJSON_GetArrayItem(got_items, 0);

    // step 3: Operate
    double v = cJSON_GetNumberValue(first);
    double computed = v * 3.0 + (double)size;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    (void)out;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}