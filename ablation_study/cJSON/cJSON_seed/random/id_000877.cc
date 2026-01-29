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
//<ID> 877
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[{\"v\":10},{\"v\":20},{\"v\":10}]";
    cJSON *parsed = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool cmp_result = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse(json);

    // step 3: Operate on parsed array (measure size and detach an element)
    size_before = cJSON_GetArraySize(parsed);
    detached = cJSON_DetachItemFromArray(parsed, 2);
    size_after = cJSON_GetArraySize(parsed);

    // step 4: Configure object and attach items
    first_item = cJSON_GetArrayItem(parsed, 0);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "array", parsed);
    cJSON_AddItemToObject(root, "extracted", detached);

    // step 5: Validate by comparing detached item with the first item and aggregating checks
    cmp_result = cJSON_Compare(detached, first_item, (cJSON_bool)1);
    validation_score = (int)cmp_result + (size_before == 3) + (size_after == 2) + (root != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}