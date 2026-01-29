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
//<ID> 861
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed = (cJSON *)0;
    cJSON *obj = (cJSON *)0;
    cJSON *num_a = (cJSON *)0;
    cJSON *num_b = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    const char *parse_end = (const char *)0;
    cJSON_bool added_parsed = 0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    int validation_score = 0;
    const char *json = "[1,2,3]";
    size_t json_len = (size_t)7;

    // step 2: Initialize / Parse input
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 3: Configure object and add items
    obj = cJSON_CreateObject();
    added_parsed = cJSON_AddItemToObject(obj, "numbers", parsed);
    num_a = cJSON_CreateNumber(42.0);
    num_b = cJSON_CreateNumber(100.5);
    added_a = cJSON_AddItemToObject(obj, "answer", num_a);
    added_b = cJSON_AddItemToObject(obj, "temp", num_b);

    // step 4: Operate and Validate
    retrieved = cJSON_GetArrayItem(parsed, 2);
    validation_score = (int)(parsed != (cJSON *)0) + (int)(obj != (cJSON *)0) + (int)added_parsed + (int)added_a + (int)added_b + (int)(retrieved != (cJSON *)0) + (int)(cJSON_GetNumberValue(retrieved) == 3.0);
    (void)validation_score;

    // step 5: Cleanup
    cJSON_Delete(obj);

    // API sequence test completed successfully
    return 66;
}