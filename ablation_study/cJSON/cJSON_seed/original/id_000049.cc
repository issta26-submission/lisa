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
//<ID> 49
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
    cJSON *ver_item = cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *true_node = cJSON_CreateTrue();
    cJSON *false_node = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON *n20 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(items, 0, n20);
    cJSON_InsertItemInArray(items, 2, cJSON_CreateNumber(30.0));
    cJSON_AddItemToArray(items, true_node);

    // step 3: Operate and Validate
    cJSON_InsertItemInArray(items, 1, false_node);
    int count = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_is_false = cJSON_IsFalse(second);
    cJSON *last = cJSON_GetArrayItem(items, count - 1);
    cJSON_bool last_is_true = cJSON_IsTrue(last);
    int summary = (int)count + (int)first_val + (int)second_is_false + (int)last_is_true;
    (void)summary;
    (void)ver_item;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}