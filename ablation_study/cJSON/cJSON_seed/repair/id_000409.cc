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
//<ID> 409
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(32.5);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddNumberToObject(config, "threshold", 5.5);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);

    // step 3: Operate
    cJSON *orig_items = cJSON_GetObjectItem(root, "items");
    int orig_count = cJSON_GetArraySize(orig_items);
    cJSON *orig_mid = cJSON_GetArrayItem(orig_items, 1);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(orig_items, 0));
    double v1 = cJSON_GetNumberValue(orig_mid);
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(orig_items, 2));
    double sum_orig = v0 + v1 + v2;
    cJSON_AddNumberToObject(root, "sum_original", sum_orig);
    cJSON *copy_items = cJSON_GetObjectItem(root, "items_copy");
    int copy_count = cJSON_GetArraySize(copy_items);
    cJSON *copy_mid = cJSON_GetArrayItem(copy_items, 1);
    double cv0 = cJSON_GetNumberValue(cJSON_GetArrayItem(copy_items, 0));
    double cv1 = cJSON_GetNumberValue(copy_mid);
    double cv2 = cJSON_GetNumberValue(cJSON_GetArrayItem(copy_items, 2));
    double sum_copy = cv0 + cv1 + cv2;
    cJSON_AddNumberToObject(root, "sum_copy", sum_copy);

    // step 4: Validate and Cleanup
    double diff = sum_orig - sum_copy;
    cJSON_AddNumberToObject(root, "sum_difference", diff);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}