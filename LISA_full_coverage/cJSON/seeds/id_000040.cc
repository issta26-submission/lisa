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
//<ID> 40
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
    cJSON_AddItemToObject(root, "items", items);
    cJSON *root_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", root_flag);

    // step 2: Configure
    cJSON *ver_item = cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON_InsertItemInArray(items, 0, n0);
    cJSON *t1 = cJSON_CreateTrue();
    cJSON_InsertItemInArray(items, 1, t1);
    cJSON *f1 = cJSON_CreateFalse();
    cJSON_InsertItemInArray(items, 2, f1);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(items);
    cJSON *item0 = cJSON_GetArrayItem(items, 0);
    double val0 = cJSON_GetNumberValue(item0);
    cJSON *item1 = cJSON_GetArrayItem(items, 1);
    cJSON_bool is_item1_true = cJSON_IsTrue(item1);
    cJSON *item2 = cJSON_GetArrayItem(items, 2);
    cJSON_bool is_item2_false = cJSON_IsFalse(item2);
    double version_val = cJSON_GetNumberValue(ver_item);
    int summary = size + (int)val0 + (int)version_val + (int)is_item1_true + (int)is_item2_false;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}