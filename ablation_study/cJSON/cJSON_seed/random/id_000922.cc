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
//<ID> 922
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_str = "label";
    const char *key_bool = "enabled";
    const char *name_val = "test-name";
    cJSON *root = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    const char *extracted = (const char *)0;
    cJSON_bool has_label = (cJSON_bool)0;
    cJSON_bool has_bool = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a false boolean item
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();

    // step 3: Configure - add a string to the object and attach the false item
    str_item = cJSON_AddStringToObject(root, key_str, name_val);
    cJSON_AddItemToObject(root, key_bool, false_item);

    // step 4: Operate - extract string value and check object keys
    extracted = cJSON_GetStringValue(str_item);
    has_label = cJSON_HasObjectItem(root, key_str);
    has_bool = cJSON_HasObjectItem(root, key_bool);

    // step 5: Validate - aggregate simple checks into a score
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(str_item != (cJSON *)0)
                     + (int)has_label
                     + (int)has_bool
                     + (int)(extracted != (const char *)0)
                     + (int)(extracted != (const char *)0 && extracted[0] == name_val[0]);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}