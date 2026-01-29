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
//<ID> 1211
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
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *label = cJSON_CreateString("dup-test");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "numbers");
    cJSON *container = cJSON_CreateArray();
    cJSON_AddItemToArray(container, detached);
    cJSON_AddItemToObject(root, "container", container);
    cJSON *dup_first = cJSON_GetArrayItem(dup_arr, 0);
    double first_val = cJSON_GetNumberValue(dup_first);
    cJSON *computed = cJSON_CreateNumber(first_val + 3.5);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}