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
//<ID> 619
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double a = 1.5;
    double b = 2.5;
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(a);
    cJSON *n2 = cJSON_CreateNumber(b);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemToObject(root, "nums", nums);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "scale", 2.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *nums_ref = cJSON_GetObjectItem(root, "nums");
    cJSON *first = cJSON_GetArrayItem(nums_ref, 0);
    cJSON *second = cJSON_GetArrayItem(nums_ref, 1);
    double v1 = cJSON_GetNumberValue(first);
    double v2 = cJSON_GetNumberValue(second);
    double product = v1 * v2;

    // step 3: Operate
    cJSON *product_item = cJSON_CreateNumber(product);
    cJSON_AddItemToArray(nums_ref, product_item);
    cJSON *ref_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemReferenceToArray(nums_ref, ref_item);
    char *unformatted = cJSON_PrintUnformatted(root);
    double product_val = cJSON_GetNumberValue(product_item);

    // step 4: Validate & Cleanup
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON_DeleteItemFromObject(root, "nums");
    cJSON_free(unformatted);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}