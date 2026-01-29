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
//<ID> 1014
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"test\",\"value\":123,\"nested\":{\"flag\":false}}";
    const char *array_key = "items";
    const char *ref_key = "name_ref";
    const char *bool_key = "new_flag";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *ref_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON_bool add_ref_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse input JSON and create an array to attach
    root = cJSON_Parse(json_text);
    arr = cJSON_CreateArray();

    // step 3: Configure - attach the array to root, add a boolean member, and add a reference to an existing item
    cJSON_AddItemToObject(root, array_key, arr);
    bool_item = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);
    name_item = cJSON_GetObjectItem(root, "name");
    add_ref_ok = cJSON_AddItemReferenceToObject(root, ref_key, name_item);

    // step 4: Operate - retrieve the newly added reference and the boolean via GetObjectItem
    ref_item = cJSON_GetObjectItem(root, ref_key);
    cJSON *fetched_bool = cJSON_GetObjectItem(root, bool_key);

    // step 5: Validate - compute a small validation score exercising pointers and content
    const char *ref_name = (const char *)0;
    ref_name = cJSON_GetStringValue(ref_item);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(add_ref_ok != (cJSON_bool)0) + (int)(ref_item != (cJSON *)0) + (int)(fetched_bool != (cJSON *)0) + (int)(ref_name != (const char *)0 && ref_name[0] == 't');
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}