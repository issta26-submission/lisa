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
//<ID> 170
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *marker = (cJSON *)0;
    cJSON *got0 = (cJSON *)0;
    cJSON_bool insert_ok1 = (cJSON_bool)0;
    cJSON_bool insert_ok2 = (cJSON_bool)0;
    cJSON_bool first_is_true = (cJSON_bool)0;
    cJSON_bool marker_is_true = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    marker = cJSON_AddBoolToObject(root, "marker_before", 1);

    // step 4: Operate
    insert_ok1 = cJSON_InsertItemInArray(arr, 0, item1);
    insert_ok2 = cJSON_InsertItemInArray(arr, 0, item0);
    cJSON_InsertItemInArray(arr, 1, true_item);

    // step 5: Validate
    got0 = cJSON_GetArrayItem(arr, 0);
    first_is_true = cJSON_IsTrue(got0);
    marker_is_true = cJSON_IsTrue(marker);
    cJSON_AddBoolToObject(root, "insert_ok1", insert_ok1);
    cJSON_AddBoolToObject(root, "insert_ok2", insert_ok2);
    cJSON_AddBoolToObject(root, "first_is_true", first_is_true);
    cJSON_AddBoolToObject(root, "marker_is_true", marker_is_true);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}