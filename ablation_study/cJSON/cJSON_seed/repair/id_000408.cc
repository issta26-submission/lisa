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
//<ID> 408
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_items);
    cJSON *retrieved_items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(retrieved_items, 0);
    cJSON_SetNumberHelper(first_item, 15.0);

    // step 3: Operate
    cJSON *retrieved_copy = cJSON_GetObjectItem(root, "items_copy");
    cJSON *copy_second = cJSON_GetArrayItem(retrieved_copy, 1);
    double val_copy_second = cJSON_GetNumberValue(copy_second);
    double val_first = cJSON_GetNumberValue(first_item);
    double sum = val_copy_second + val_first;
    cJSON_AddNumberToObject(root, "computed_sum", sum);

    // step 4: Validate and Cleanup
    int original_size = cJSON_GetArraySize(items);
    int copy_size = cJSON_GetArraySize(dup_items);
    char *out = cJSON_PrintUnformatted(root);
    (void)original_size;
    (void)copy_size;
    (void)sum;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}