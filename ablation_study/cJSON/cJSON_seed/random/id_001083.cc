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
//<ID> 1083
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *name = (cJSON *)0;
    cJSON *dup_values = (cJSON *)0;
    cJSON *fetched_values = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    const char *name_value = (const char *)0;
    float numbers[3] = { 1.1f, 2.2f, 3.3f };
    cJSON_bool add_ok_values = (cJSON_bool)0;
    cJSON_bool add_ok_name = (cJSON_bool)0;
    cJSON_bool add_ok_dup = (cJSON_bool)0;
    cJSON_bool name_is_null = (cJSON_bool)0;
    int validation_score = 0;
    char *printed = (char *)0;

    // step 2: Setup - create object, float array and string
    root = cJSON_CreateObject();
    values = cJSON_CreateFloatArray(numbers, 3);
    name = cJSON_CreateString("sensor_A");

    // step 3: Configure - attach items to root
    add_ok_values = cJSON_AddItemToObject(root, "values", values);
    add_ok_name = cJSON_AddItemToObject(root, "name", name);

    // step 4: Operate - fetch items and inspect
    fetched_values = cJSON_GetObjectItem(root, "values");
    fetched_name = cJSON_GetObjectItem(root, "name");
    name_is_null = cJSON_IsNull(fetched_name);
    name_value = cJSON_GetStringValue(fetched_name);

    // step 5: Duplicate - duplicate the values array and attach the duplicate
    dup_values = cJSON_Duplicate(fetched_values, (cJSON_bool)1);
    add_ok_dup = cJSON_AddItemToObject(root, "values_copy", dup_values);

    // step 6: Validate and Cleanup - serialize, compute a small validation score and free
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(values != (cJSON *)0) + (int)(name != (cJSON *)0) + (int)(dup_values != (cJSON *)0) + (int)(add_ok_values) + (int)(add_ok_name) + (int)(add_ok_dup) + (int)(!name_is_null) + (int)(name_value != (const char *)0);
    (void)validation_score;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}