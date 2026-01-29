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
//<ID> 867
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed_array = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *num_a = (cJSON *)0;
    cJSON *num_b = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    const char *parse_end = (const char *)0;
    double value0 = 0.0;
    double value1 = 0.0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    int validation_score = 0;

    // step 2: Setup / Parse
    const char *json = "[1,2,3]";
    parsed_array = cJSON_ParseWithLengthOpts(json, (size_t)7, &parse_end, (cJSON_bool)1);

    // step 3: Initialize objects and numbers
    root = cJSON_CreateObject();
    num_a = cJSON_CreateNumber(42.0);
    num_b = cJSON_CreateNumber(99.5);

    // step 4: Configure - add parsed array and numbers into the root object
    added_arr = cJSON_AddItemToObject(root, "array", parsed_array);
    added_a = cJSON_AddItemToObject(root, "first", num_a);
    added_b = cJSON_AddItemToObject(root, "second", num_b);

    // step 5: Operate - retrieve elements from the parsed array and read values
    elem0 = cJSON_GetArrayItem(parsed_array, 0);
    elem1 = cJSON_GetArrayItem(parsed_array, 1);
    value0 = cJSON_GetNumberValue(elem0);
    value1 = cJSON_GetNumberValue(elem1);

    // step 6: Validate & Cleanup
    validation_score = (int)added_arr + (int)added_a + (int)added_b + (parse_end != (const char *)0) + (elem0 != (cJSON *)0) + (elem1 != (cJSON *)0) + (value0 < value1);
    (void)validation_score;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}