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
//<ID> 1017
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"alpha\":42,\"arr\":[1,2,3]}";
    const char *key_parsed_alpha = "alpha";
    const char *key_injected = "injected";
    const char *key_flag = "enabled";
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *got_injected = (cJSON *)0;
    cJSON *got_flag = (cJSON *)0;
    cJSON *got_alpha_item = (cJSON *)0;
    double alpha_val = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse JSON and create root and a standalone array
    parsed = cJSON_Parse(json_text);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - add the standalone array to root by reference and add a boolean member
    cJSON_AddItemReferenceToObject(root, key_injected, arr);
    cJSON_AddBoolToObject(root, key_flag, (cJSON_bool)1);

    // step 4: Operate - retrieve items using GetObjectItem including from parsed and root
    got_injected = cJSON_GetObjectItem(root, key_injected);
    got_flag = cJSON_GetObjectItem(root, key_flag);
    got_alpha_item = cJSON_GetObjectItem(parsed, key_parsed_alpha);
    alpha_val = cJSON_GetNumberValue(got_alpha_item);

    // step 5: Validate - compute a simple validation score using presence and values
    validation_score = (int)(parsed != (cJSON *)0) + (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(got_injected != (cJSON *)0) + (int)(got_flag != (cJSON *)0) + (int)(alpha_val == 42.0);
    (void)validation_score;

    // step 6: Cleanup - delete parsed and root which frees attached items
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}