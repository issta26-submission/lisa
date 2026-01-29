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
//<ID> 864
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[1, 2, 3]";
    size_t buffer_length = (sizeof("[1, 2, 3]") - 1);
    const char *parse_end = (const char *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *newnum = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    double first_value = 0.0;
    cJSON_bool added_parsed = 0;
    cJSON_bool added_num = 0;
    int validation_score = 0;

    // step 2: Parse input with explicit length/options
    parsed = cJSON_ParseWithLengthOpts(json, buffer_length, &parse_end, 1);

    // step 3: Initialize container and create a new number to add
    root = cJSON_CreateObject();
    newnum = cJSON_CreateNumber(42.0);

    // step 4: Configure object by adding parsed array and the new number
    added_parsed = cJSON_AddItemToObject(root, "array", parsed);
    added_num = cJSON_AddItemToObject(root, "answer", newnum);

    // step 5: Operate (retrieve first array element) and validate simple invariants
    first_item = cJSON_GetArrayItem(parsed, 0);
    first_value = cJSON_GetNumberValue(first_item);
    validation_score = (int)added_parsed + (int)added_num + (first_item != (cJSON *)0) + (first_value == 1.0) + (root != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}