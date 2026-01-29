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
//<ID> 1148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *main_arr = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *fetched_meta = (cJSON *)0;
    cJSON *fetched_count = (cJSON *)0;
    cJSON *fetched_values = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    double v_count = 0.0;
    double v_elem0 = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object, meta object with a string and numeric member
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    label = cJSON_CreateString("example-label");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON_AddNumberToObject(meta, "count", 123.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Configure - prepare an integer array and a main array, detach an element and move it
    {
        int nums[2];
        nums[0] = 10;
        nums[1] = 20;
        int_arr = cJSON_CreateIntArray(nums, 2);
    }
    main_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", main_arr);
    fetched_elem0 = cJSON_GetArrayItem(int_arr, 1);
    detached_item = cJSON_DetachItemViaPointer(int_arr, fetched_elem0);
    cJSON_AddItemToArray(main_arr, detached_item);

    // step 4: Operate - retrieve members and array elements and obtain numeric values
    fetched_meta = cJSON_GetObjectItem(root, "meta");
    fetched_count = cJSON_GetObjectItem(fetched_meta, "count");
    v_count = cJSON_GetNumberValue(fetched_count);
    fetched_values = cJSON_GetObjectItem(root, "values");
    fetched_elem0 = cJSON_GetArrayItem(fetched_values, 0);
    v_elem0 = cJSON_GetNumberValue(fetched_elem0);

    // step 5: Validate - compute a simple validation score from retrieved numeric values
    validation_score = (int)(v_count == 123.0) + (int)(v_elem0 == 20.0);
    (void)validation_score;

    // step 6: Cleanup - delete created structures (root owns main_arr and its moved child)
    cJSON_Delete(root);
    cJSON_Delete(int_arr);

    // API sequence test completed successfully
    return 66;
}