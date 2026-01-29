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
//<ID> 890
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"name\":\"tester\",\"values\":[1,2,3],\"count\":3}";
    cJSON *root = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *new_count_item = (cJSON *)0;
    cJSON *retrieved_new = (cJSON *)0;
    cJSON_bool is_values_array = (cJSON_bool)0;
    cJSON_bool added_new = (cJSON_bool)0;
    char *printed = (char *)0;
    double original_count = 0.0;
    double new_count_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    root = cJSON_Parse(json_text);
    values = cJSON_GetObjectItem(root, "values");
    count_item = cJSON_GetObjectItem(root, "count");

    // step 3: Operate - inspect and create new number based on parsed data
    is_values_array = cJSON_IsArray(values);
    original_count = cJSON_GetNumberValue(count_item);
    new_count_item = cJSON_CreateNumber(original_count + 10.0);
    added_new = cJSON_AddItemToObject(root, "count_plus", new_count_item);

    // step 4: Validate / Readback - print and re-retrieve new value
    printed = cJSON_Print(root);
    retrieved_new = cJSON_GetObjectItem(root, "count_plus");
    new_count_value = cJSON_GetNumberValue(retrieved_new);
    validation_score = (int)(root != (cJSON *)0) + (int)is_values_array + (int)added_new + (int)(printed != (char *)0) + (int)(new_count_value == original_count + 10.0);
    (void)validation_score;

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}