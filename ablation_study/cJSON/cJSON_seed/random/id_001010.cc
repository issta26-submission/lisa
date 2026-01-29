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
//<ID> 1010
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"meta\":{\"version\":1}, \"data\":[\"one\",\"two\"]}";
    const char *key_data = "data";
    const char *key_data_ref = "data_ref";
    const char *key_extra = "extra";
    const char *key_active = "active";
    cJSON *root = (cJSON *)0;
    cJSON *parsed_data = (cJSON *)0;
    cJSON *data_ref = (cJSON *)0;
    cJSON *extra_arr = (cJSON *)0;
    cJSON *created_extra = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    const char *first_str = (const char *)0;
    cJSON_bool add_ref_ok1 = (cJSON_bool)0;
    cJSON_bool add_ref_ok2 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON and create an independent array
    root = cJSON_Parse(json_text);
    created_extra = cJSON_CreateArray();

    // step 3: Configure - reference existing parsed array and attach the created array by reference; add a boolean
    parsed_data = cJSON_GetObjectItem(root, key_data);
    add_ref_ok1 = cJSON_AddItemReferenceToObject(root, key_data_ref, parsed_data);
    add_ref_ok2 = cJSON_AddItemReferenceToObject(root, key_extra, created_extra);
    bool_item = cJSON_AddBoolToObject(root, key_active, (cJSON_bool)1);

    // step 4: Operate - retrieve referenced items and inspect the first element of the referenced data array
    data_ref = cJSON_GetObjectItem(root, key_data_ref);
    extra_arr = cJSON_GetObjectItem(root, key_extra);
    first_elem = cJSON_GetArrayItem(data_ref, 0);
    first_str = cJSON_GetStringValue(first_elem);

    // step 5: Validate - compute a small validation score exercising pointers and content
    validation_score = (int)(root != (cJSON *)0) + (int)(parsed_data != (cJSON *)0) + (int)(data_ref != (cJSON *)0) + (int)(extra_arr != (cJSON *)0) + (int)(bool_item != (cJSON *)0) + (int)(add_ref_ok1) + (int)(add_ref_ok2) + (int)(first_str != (const char *)0 && first_str[0] == 'o');
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items and references
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}