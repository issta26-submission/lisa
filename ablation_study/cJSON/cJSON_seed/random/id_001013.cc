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
//<ID> 1013
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"config\":{\"mode\":\"auto\",\"values\":[1,2,3,4]}}";
    const char *ref_key = "values_ref";
    const char *bool_key = "active";
    cJSON *root = (cJSON *)0;
    cJSON *config = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *fetched_ref = (cJSON *)0;
    cJSON *fetched_bool = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    int validation_score = 0;
    cJSON_bool is_array = (cJSON_bool)0;

    // step 2: Setup - parse JSON and locate nested items, create an independent array
    root = cJSON_Parse(json_text);
    config = cJSON_GetObjectItem(root, "config");
    values = cJSON_GetObjectItem(config, "values");
    arr = cJSON_CreateArray();

    // step 3: Configure - add a reference to the parsed values array into root and add a boolean member
    cJSON_AddItemReferenceToObject(root, ref_key, values);
    bool_item = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);

    // step 4: Operate - retrieve the referenced array and the boolean using GetObjectItem
    fetched_ref = cJSON_GetObjectItem(root, ref_key);
    fetched_bool = cJSON_GetObjectItem(root, bool_key);

    // step 5: Validate - size up simple properties and compute a small validation score
    is_array = (cJSON_bool)(fetched_ref != (cJSON *)0 && cJSON_IsArray(fetched_ref));
    validation_score = (int)(root != (cJSON *)0) + (int)(config != (cJSON *)0) + (int)(values != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)(fetched_ref != (cJSON *)0) + (int)(fetched_bool != (cJSON *)0) + (int)is_array;
    (void)validation_score;

    // step 6: Cleanup - delete created structures to free memory
    cJSON_Delete(root);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}