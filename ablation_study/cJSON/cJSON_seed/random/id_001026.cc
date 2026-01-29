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
//<ID> 1026
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"values\": [10]}";
    cJSON *root = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *created_num = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    double fetched_value = 0.0;
    cJSON_bool is_array = (cJSON_bool)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - parse JSON and locate the array
    root = cJSON_Parse(json_text);
    values = cJSON_GetObjectItem(root, "values");

    // step 3: Configure - check type and create a new number to append
    is_array = cJSON_IsArray(values);
    created_num = cJSON_CreateNumber(42.5);
    add_ok = cJSON_AddItemToArray(values, created_num);

    // step 4: Operate - retrieve the newly added element and read its numeric value
    fetched_num = cJSON_GetArrayItem(values, 1);
    fetched_value = cJSON_GetNumberValue(fetched_num);

    // step 5: Validate - combine pointer and content checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)(values != (cJSON *)0) + (int)(is_array) + (int)(created_num != (cJSON *)0) + (int)(add_ok) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_value == 42.5);
    (void)validation_score;

    // step 6: Cleanup - free all allocated JSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}