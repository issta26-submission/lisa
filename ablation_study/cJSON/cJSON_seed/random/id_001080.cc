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
//<ID> 1080
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *float_array = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *fetched_array = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_null = (cJSON_bool)0;
    cJSON_bool add_ok_array = (cJSON_bool)0;
    cJSON_bool add_ok_label = (cJSON_bool)0;
    int validation_score = 0;
    float numbers[3] = { 1.5f, 2.5f, 3.5f };

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    float_array = cJSON_CreateFloatArray(numbers, 3);
    label = cJSON_CreateString("sensor_readings");

    // step 3: Configure - attach items to root
    add_ok_array = cJSON_AddItemToObject(root, "values", float_array);
    add_ok_label = cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate - duplicate root and fetch items from the duplicate
    dup_root = cJSON_Duplicate(root, 1);
    fetched_array = cJSON_GetObjectItem(dup_root, "values");
    fetched_label = cJSON_GetObjectItem(dup_root, "label");
    is_null = cJSON_IsNull(fetched_array);

    // step 5: Validate - serialize and derive a small validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(dup_root != (cJSON *)0) + (int)(float_array != (cJSON *)0) + (int)(label != (cJSON *)0) + (int)(add_ok_array) + (int)(add_ok_label) + (int)(fetched_array != (cJSON *)0) + (int)(fetched_label != (cJSON *)0) + (int)(!is_null);
    (void)validation_score;

    // step 6: Cleanup - delete both original and duplicated structures
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    return 66;
}