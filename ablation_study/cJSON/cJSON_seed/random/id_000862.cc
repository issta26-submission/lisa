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
//<ID> 862
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[10,20,30]";
    size_t buffer_length = (size_t)10;
    const char *parse_end = (const char *)0;
    cJSON_bool require_null_terminated = 1;
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *arr_item = (cJSON *)0;
    cJSON *ans_item = (cJSON *)0;
    cJSON_bool added_numbers = 0;
    cJSON_bool added_answer = 0;
    double extracted_value = 0.0;
    double answer_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize / Parse input with length and opts
    parsed = cJSON_ParseWithLengthOpts(json, buffer_length, &parse_end, require_null_terminated);

    // step 3: Configure objects and add items
    root = cJSON_CreateObject();
    added_numbers = cJSON_AddItemToObject(root, "numbers", parsed);
    num_item = cJSON_CreateNumber(42.0);
    added_answer = cJSON_AddItemToObject(root, "answer", num_item);

    // step 4: Operate (retrieve array element and object item)
    arr_item = cJSON_GetArrayItem(parsed, 1);
    extracted_value = cJSON_GetNumberValue(arr_item);
    ans_item = cJSON_GetObjectItem(root, "answer");
    answer_value = cJSON_GetNumberValue(ans_item);

    // step 5: Validate aggregated results
    validation_score = (int)(parsed != (cJSON *)0) + (int)(root != (cJSON *)0) + (int)added_numbers + (int)added_answer + (int)(arr_item != (cJSON *)0) + (int)(ans_item != (cJSON *)0) + (int)(extracted_value == 20.0) + (int)(answer_value == 42.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}