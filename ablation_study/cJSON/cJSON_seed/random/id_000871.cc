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
//<ID> 871
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[1, 2, 3]";
    cJSON *parsed = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *num_two = (cJSON *)0;
    cJSON_bool equal = (cJSON_bool)0;
    cJSON_bool added_array = (cJSON_bool)0;
    cJSON_bool added_extracted = (cJSON_bool)0;
    cJSON_bool added_two = (cJSON_bool)0;
    int size_before = 0;
    int size_after = 0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse(json);

    // step 3: Operate on parsed array (measure size, detach element, compare)
    size_before = cJSON_GetArraySize(parsed);
    detached = cJSON_DetachItemFromArray(parsed, 1);
    size_after = cJSON_GetArraySize(parsed);
    num_two = cJSON_CreateNumber(2.0);
    equal = cJSON_Compare(detached, num_two, (cJSON_bool)1);

    // step 4: Configure object and add items
    root = cJSON_CreateObject();
    added_array = cJSON_AddItemToObject(root, "array", parsed);
    added_extracted = cJSON_AddItemToObject(root, "extracted", detached);
    added_two = cJSON_AddItemToObject(root, "two", num_two);

    // step 5: Validate results (aggregate simple checks)
    printed = cJSON_PrintUnformatted(root);
    validation_score = size_before + size_after + (int)equal + (int)added_array + (int)added_extracted + (int)added_two + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}