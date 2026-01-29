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
//<ID> 511
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const int numbers[] = {10, 20, 30, 40};
    const int numbers_count = 4;
    cJSON *root = (cJSON *)0;
    cJSON *ints_array = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    cJSON *retrieved_label = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    int array_size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    ints_array = cJSON_CreateIntArray(numbers, numbers_count);
    label = cJSON_CreateString("example");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "numbers", ints_array);
    add_ok2 = cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate
    retrieved_array = cJSON_GetObjectItem(root, "numbers");
    retrieved_label = cJSON_GetObjectItem(root, "label");
    array_size = cJSON_GetArraySize(retrieved_array);

    // step 5: Validate
    count_item = cJSON_CreateNumber((double)array_size);
    add_ok3 = cJSON_AddItemToObject(root, "count", count_item);
    (void)retrieved_label;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}