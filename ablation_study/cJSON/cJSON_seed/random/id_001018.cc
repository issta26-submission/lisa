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
//<ID> 1018
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"meta\":\"example\"}";
    const char *arr_key = "shared";
    const char *bool_key = "enabled";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_bool = (cJSON *)0;
    cJSON_bool ref_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse a small JSON object and create a standalone array
    root = cJSON_Parse(json_text);
    arr = cJSON_CreateArray();

    // step 3: Configure - add a boolean member to the parsed root and add a reference to the created array
    bool_item = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);
    ref_ok = cJSON_AddItemReferenceToObject(root, arr_key, arr);

    // step 4: Operate - retrieve the newly added members by name
    fetched_arr = cJSON_GetObjectItem(root, arr_key);
    fetched_bool = cJSON_GetObjectItem(root, bool_key);

    // step 5: Validate - compute a simple validation score based on pointers and operation results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(ref_ok) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_bool == bool_item);
    (void)validation_score;

    // step 6: Cleanup - delete parsed root and the standalone array
    cJSON_Delete(root);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}