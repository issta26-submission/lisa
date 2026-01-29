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
//<ID> 860
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "[4.2,8.6,16.4]";
    const char * parse_end = (const char *)0;
    size_t buffer_length = sizeof(json) - 1;
    cJSON * parsed = (cJSON *)0;
    cJSON * root = (cJSON *)0;
    cJSON * arr_item = (cJSON *)0;
    cJSON * computed_num = (cJSON *)0;
    cJSON_bool added_array = 0;
    cJSON_bool added_computed = 0;
    double extracted_value = 0.0;
    int validation_score = 0;

    // step 2: Parse input with explicit length/options
    parsed = cJSON_ParseWithLengthOpts(json, buffer_length, &parse_end, 1);

    // step 3: Operate on parsed data and create new items
    arr_item = cJSON_GetArrayItem(parsed, 1);
    extracted_value = (arr_item != (cJSON *)0) ? arr_item->valuedouble : 0.0;
    computed_num = cJSON_CreateNumber(extracted_value * 2.0);

    // step 4: Configure object and add items
    root = cJSON_CreateObject();
    added_array = cJSON_AddItemToObject(root, "numbers", parsed);
    added_computed = cJSON_AddItemToObject(root, "double_of_second", computed_num);

    // step 5: Validate and Cleanup
    validation_score = (int)(root != (cJSON *)0) + (int)added_array + (int)added_computed + (int)(arr_item != (cJSON *)0) + (int)(computed_num != (cJSON *)0);
    (void)validation_score;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}