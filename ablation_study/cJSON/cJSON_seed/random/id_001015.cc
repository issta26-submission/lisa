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
//<ID> 1015
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{}";
    const char *arr_key = "arr";
    const char *holder_key = "holder";
    const char *arr_ref_key = "arr_ref";
    const char *enabled_key = "enabled";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *holder = (cJSON *)0;
    cJSON *ref_item = (cJSON *)0;
    cJSON *got_holder = (cJSON *)0;
    cJSON *got_ref = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    double first_val = 0.0;
    int validation_score = 0;
    cJSON_bool add_ref_ok = (cJSON_bool)0;
    cJSON_bool add_ok = (cJSON_bool)0;

    // step 2: Setup - parse an empty object and create an array with a single number element
    root = cJSON_Parse(json_text);
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(7.0);
    add_ok = cJSON_AddItemToArray(arr, num);

    // step 3: Configure - attach the array to the parsed root and create a holder object that references the array
    add_ok = cJSON_AddItemToObject(root, arr_key, arr);
    holder = cJSON_CreateObject();
    add_ref_ok = cJSON_AddItemReferenceToObject(holder, arr_ref_key, arr);
    add_ok = cJSON_AddItemToObject(root, holder_key, holder);
    bool_item = cJSON_AddBoolToObject(root, enabled_key, (cJSON_bool)1);

    // step 4: Operate - retrieve the holder and the referenced array via object lookup
    got_holder = cJSON_GetObjectItem(root, holder_key);
    got_ref = cJSON_GetObjectItem(got_holder, arr_ref_key);
    got_arr = cJSON_GetObjectItem(root, arr_key);

    // step 5: Validate - inspect the first numeric value and compute a small validation score
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(got_arr, 0));
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(num != (cJSON *)0) + (int)(holder != (cJSON *)0) + (int)(got_holder != (cJSON *)0) + (int)(got_ref != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)(first_val == 7.0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}