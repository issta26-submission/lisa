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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    int count_after = 0;
    cJSON_bool first_is_true = (cJSON_bool)0;
    cJSON *flag_node = (cJSON *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateNumber(10.0);
    item2 = cJSON_CreateNumber(20.0);
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_InsertItemInArray(arr, 0, true_item);

    // step 4: Operate
    first = cJSON_GetArrayItem(arr, 0);
    first_is_true = cJSON_IsTrue(first);
    count_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    flag_node = cJSON_AddBoolToObject(root, "first_is_true", first_is_true);
    cJSON_AddNumberToObject(root, "count_after", (double)count_after);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}