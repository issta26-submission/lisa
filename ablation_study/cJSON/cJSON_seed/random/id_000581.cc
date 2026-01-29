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
//<ID> 581
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    const char *parse_end = (const char *)0;
    cJSON_bool require_null_terminated = 1;
    const char *json_text = "{\"values\": [1.5, 2.5]}";
    cJSON *arr = (cJSON *)0;
    cJSON *arr_cs = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *new_number = (cJSON *)0;
    cJSON_bool add_result = 0;
    double first_value = 0.0;
    double added_value = 0.0;

    // step 2: Initialize (parse input JSON)
    root = cJSON_ParseWithOpts(json_text, &parse_end, require_null_terminated);

    // step 3: Configure (locate array and append a new number)
    arr = cJSON_GetObjectItem(root, "values");
    arr_cs = cJSON_GetObjectItemCaseSensitive(root, "values");
    new_number = cJSON_CreateNumber(3.75);
    add_result = cJSON_AddItemToArray(arr, new_number);

    // step 4: Operate (read numeric values from array entries)
    first_item = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetNumberValue(first_item);
    added_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));

    // step 5: Validate (use variables to represent validation)
    (void)parse_end;
    (void)require_null_terminated;
    (void)arr_cs;
    (void)add_result;
    (void)first_value;
    (void)added_value;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}