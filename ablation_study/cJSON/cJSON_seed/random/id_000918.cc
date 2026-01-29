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
//<ID> 918
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *str_key = "greeting";
    const char *num_key = "version";
    const char *greeting = "hello";
    double version = 2.5;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *greeting_item = (cJSON *)0;
    cJSON *version_item = (cJSON *)0;
    cJSON_bool added_arr = (cJSON_bool)0;
    cJSON_bool invalid_check = (cJSON_bool)0;
    double extracted_version = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    greeting_item = cJSON_AddStringToObject(root, str_key, greeting);
    version_item = cJSON_AddNumberToObject(root, num_key, version);
    added_arr = cJSON_AddItemToObject(root, arr_key, arr);

    // step 4: Operate
    invalid_check = cJSON_IsInvalid(greeting_item);
    extracted_version = cJSON_GetNumberValue(version_item);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(greeting_item != (cJSON *)0) + (int)(version_item != (cJSON *)0) + (int)(added_arr) + (int)(!invalid_check) + (int)(extracted_version == version);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}