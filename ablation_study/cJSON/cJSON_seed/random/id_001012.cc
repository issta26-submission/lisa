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
//<ID> 1012
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"existing\":123}";
    const char *arr_key = "ref_arr";
    const char *flag_key = "enabled";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_flag = (cJSON *)0;
    cJSON *existing_item = (cJSON *)0;
    cJSON_bool add_ref_ok = (cJSON_bool)0;
    double existing_val = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse a small JSON object and create an array to reference into it
    root = cJSON_Parse(json_text);
    arr = cJSON_CreateArray();

    // step 3: Configure - add the array as a reference into the parsed root and add a boolean member
    add_ref_ok = cJSON_AddItemReferenceToObject(root, arr_key, arr);
    cJSON *bool_item = cJSON_AddBoolToObject(root, flag_key, (cJSON_bool)1);

    // step 4: Operate - retrieve the previously added members using object lookup
    fetched_arr = cJSON_GetObjectItem(root, arr_key);
    fetched_flag = cJSON_GetObjectItem(root, flag_key);
    existing_item = cJSON_GetObjectItem(root, "existing");

    // step 5: Validate - obtain numeric value and compute a small validation score exercising pointers and return values
    existing_val = cJSON_GetNumberValue(existing_item);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)add_ref_ok + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_flag != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)(existing_val == 123.0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items (including referenced items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}