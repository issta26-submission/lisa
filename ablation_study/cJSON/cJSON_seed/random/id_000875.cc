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
//<ID> 875
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[10, 20, 30]";
    cJSON *parsed = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *detached_copy = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool cmp_equal = (cJSON_bool)0;
    cJSON_bool added_original = (cJSON_bool)0;
    cJSON_bool added_extracted = (cJSON_bool)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse(json);

    // step 3: Operate on parsed array (measure size and detach an element)
    size_before = cJSON_GetArraySize(parsed);
    detached = cJSON_DetachItemFromArray(parsed, 1);
    size_after = cJSON_GetArraySize(parsed);

    // step 4: Configure object and duplicate detached item for comparison
    root = cJSON_CreateObject();
    detached_copy = cJSON_Duplicate(detached, (cJSON_bool)1);
    cmp_equal = cJSON_Compare(detached, detached_copy, (cJSON_bool)1);

    // step 5: Add items into object and produce a compact print for validation
    added_original = cJSON_AddItemToObject(root, "original", parsed);
    added_extracted = cJSON_AddItemToObject(root, "extracted", detached);
    printed = cJSON_PrintUnformatted(root);
    validation_score = size_before + size_after + (int)cmp_equal + (int)added_original + (int)added_extracted + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_copy);

    // API sequence test completed successfully
    return 66;
}