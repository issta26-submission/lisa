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
//<ID> 869
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[1, 2, 3]";
    const char *parse_end = (const char *)0;
    size_t buffer_length = (sizeof("[1, 2, 3]") - 1);
    cJSON *parsed = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    double second_value = 0.0;
    cJSON *root = (cJSON *)0;
    cJSON *new_number = (cJSON *)0;
    cJSON_bool added = 0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize / Parse input with length options
    parsed = cJSON_ParseWithLengthOpts(json, buffer_length, &parse_end, (cJSON_bool)1);

    // step 3: Operate on parsed array (retrieve second element)
    second_item = cJSON_GetArrayItem(parsed, 1);
    second_value = cJSON_GetNumberValue(second_item);

    // step 4: Configure new object and add created number based on parsed data
    root = cJSON_CreateObject();
    new_number = cJSON_CreateNumber(second_value * 2.0);
    added = cJSON_AddItemToObject(root, "double", new_number);

    // step 5: Validate by printing and aggregating simple checks
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(parsed != (cJSON *)0) + (int)(second_item != (cJSON *)0) + (int)added + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}