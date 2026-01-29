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
//<ID> 1016
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "numbers";
    const char *ref_key = "first";
    const char *flag_key = "enabled";
    const char *json_text = "{}";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *fetched = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON_bool add_arr_ok = (cJSON_bool)0;
    cJSON_bool add_ref_ok = (cJSON_bool)0;
    cJSON_bool bool_added = (cJSON_bool)0;
    double fetched_num = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse an empty root object and create an array
    root = cJSON_Parse(json_text);
    arr = cJSON_CreateArray();

    // step 3: Configure - attach array to root and populate it with a numeric item
    add_arr_ok = cJSON_AddItemToObject(root, array_key, arr);
    n1 = cJSON_CreateNumber(7.0);
    add_arr_ok = cJSON_AddItemToArray(arr, n1);

    // step 4: Operate - add a reference to the existing number into the root and add a boolean flag
    add_ref_ok = cJSON_AddItemReferenceToObject(root, ref_key, n1);
    bool_item = cJSON_AddBoolToObject(root, flag_key, (cJSON_bool)1);
    bool_added = (bool_item != (cJSON *)0);

    // step 5: Validate - retrieve the referenced item from root and compute a small validation score
    fetched = cJSON_GetObjectItem(root, ref_key);
    fetched_num = cJSON_GetNumberValue(fetched);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(n1 != (cJSON *)0) + (int)add_arr_ok + (int)add_ref_ok + (int)bool_added + (int)(fetched_num == 7.0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}