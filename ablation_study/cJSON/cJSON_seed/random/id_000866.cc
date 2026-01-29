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
//<ID> 866
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *num_from_array = (cJSON *)0;
    cJSON *newnum = (cJSON *)0;
    const char *json = "[10,20,30]";
    const char *parse_end = (const char *)0;
    size_t buf_len = (size_t)10;
    cJSON_bool added_parsed = 0;
    cJSON_bool added_n1 = 0;
    cJSON_bool added_n2 = 0;
    cJSON_bool added_third = 0;
    double third_val = 0.0;
    int validation_score = 0;

    // step 2: Parse input with explicit length/options
    parsed = cJSON_ParseWithLengthOpts(json, buf_len, &parse_end, 0);

    // step 3: Initialize container and attach parsed array
    root = cJSON_CreateObject();
    added_parsed = cJSON_AddItemToObject(root, "numbers", parsed);

    // step 4: Create numeric items and add to object
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(-3.14);
    added_n1 = cJSON_AddItemToObject(root, "answer", num1);
    added_n2 = cJSON_AddItemToObject(root, "neg", num2);

    // step 5: Operate on parsed array: retrieve element and mirror its value
    num_from_array = cJSON_GetArrayItem(parsed, 2);
    third_val = cJSON_GetNumberValue(num_from_array);
    newnum = cJSON_CreateNumber(third_val);
    added_third = cJSON_AddItemToObject(root, "third_value", newnum);

    // step 6: Validate aggregated simple checks and cleanup
    validation_score = (int)(parsed != (cJSON *)0) + (int)added_parsed + (int)added_n1 + (int)added_n2 + (int)added_third + (int)(parse_end != (const char *)0);
    (void)validation_score;
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}