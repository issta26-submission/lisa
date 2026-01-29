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
//<ID> 1019
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"container\": {\"inner\": 123}, \"flag\": false, \"name\": \"root\"}";
    const char *numbers_key = "numbers";
    const char *flag_key = "added_flag";
    const char *ref_key = "container_ref";
    cJSON *root = (cJSON *)0;
    cJSON *container = (cJSON *)0;
    cJSON *container_ref = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *fetched_flag = (cJSON *)0;
    cJSON *fetched_numbers = (cJSON *)0;
    cJSON_bool add_ref_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse initial JSON into root
    root = cJSON_Parse(json_text);

    // step 3: Configure - obtain an existing object, create an array and add a boolean, then add a reference to the existing object
    container = cJSON_GetObjectItem(root, "container");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, numbers_key, arr);
    bool_item = cJSON_AddBoolToObject(root, flag_key, (cJSON_bool)1);
    add_ref_ok = cJSON_AddItemReferenceToObject(root, ref_key, container);

    // step 4: Operate - retrieve the items we just added or referenced
    fetched_flag = cJSON_GetObjectItem(root, flag_key);
    container_ref = cJSON_GetObjectItem(root, ref_key);
    fetched_numbers = cJSON_GetObjectItem(root, numbers_key);

    // step 5: Validate - compute a small validation score exercising pointers and type checks
    validation_score = (int)(root != (cJSON *)0) + (int)(container != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)(add_ref_ok) + (int)(container_ref != (cJSON *)0) + (int)(cJSON_IsObject(container_ref)) + (int)(cJSON_IsBool(fetched_flag));
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}