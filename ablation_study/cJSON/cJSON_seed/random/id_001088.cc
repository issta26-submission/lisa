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
//<ID> 1088
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *float_array = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *fetched_values = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    cJSON *dup_array = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool add_ok_values = (cJSON_bool)0;
    cJSON_bool add_ok_label = (cJSON_bool)0;
    cJSON_bool is_null_flag = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, a float array and a string
    root = cJSON_CreateObject();
    float nums_arr[3];
    nums_arr[0] = 1.0f;
    nums_arr[1] = 2.5f;
    nums_arr[2] = -3.14f;
    float_array = cJSON_CreateFloatArray(nums_arr, 3);
    str_item = cJSON_CreateString("sensor_readings");

    // step 3: Configure - attach items to the root object
    add_ok_values = cJSON_AddItemToObject(root, "values", float_array);
    add_ok_label = cJSON_AddItemToObject(root, "label", str_item);

    // step 4: Operate - fetch items, check nullness and duplicate the array
    fetched_values = cJSON_GetObjectItem(root, "values");
    fetched_label = cJSON_GetObjectItem(root, "label");
    is_null_flag = cJSON_IsNull(fetched_values);
    dup_array = cJSON_Duplicate(fetched_values, (cJSON_bool)1);

    // step 5: Validate - serialize and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(float_array != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(add_ok_values) + (int)(add_ok_label) + (int)(fetched_values != (cJSON *)0) + (int)(fetched_label != (cJSON *)0) + (int)(dup_array != (cJSON *)0) + (int)(!is_null_flag);
    (void)validation_score;

    // step 6: Cleanup - delete created cJSON structures
    cJSON_Delete(root);
    cJSON_Delete(dup_array);

    // API sequence test completed successfully
    return 66;
}