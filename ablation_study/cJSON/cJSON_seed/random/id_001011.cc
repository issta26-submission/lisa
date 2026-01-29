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
//<ID> 1011
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"alpha\":{\"value\":3.5},\"beta\":true}";
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *alpha = (cJSON *)0;
    cJSON *ref_ok_item = (cJSON *)0;
    cJSON *ref_obj = (cJSON *)0;
    cJSON *value_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON_bool ref_ok = (cJSON_bool)0;
    double extracted = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse existing JSON and create a fresh root and array
    parsed = cJSON_Parse(json_text);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - attach array and a boolean to the new root, then reference parsed child into root
    cJSON_AddItemToObject(root, "collection", arr);
    bool_item = cJSON_AddBoolToObject(root, "active", (cJSON_bool)1);
    alpha = cJSON_GetObjectItem(parsed, "alpha");
    ref_ok = cJSON_AddItemReferenceToObject(root, "alpha_ref", alpha);

    // step 4: Operate - retrieve the referenced object from root and extract its numeric member
    ref_obj = cJSON_GetObjectItem(root, "alpha_ref");
    value_item = cJSON_GetObjectItem(ref_obj, "value");
    extracted = cJSON_GetNumberValue(value_item);

    // step 5: Validate - compute a small validation score exercising pointers and values
    validation_score = (int)(parsed != (cJSON *)0) + (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(alpha != (cJSON *)0) + (int)ref_ok + (int)(bool_item != (cJSON *)0) + (int)(value_item != (cJSON *)0) + (int)(extracted == 3.5);
    (void)validation_score;

    // step 6: Cleanup - delete root first (it holds a reference into parsed), then delete parsed
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}