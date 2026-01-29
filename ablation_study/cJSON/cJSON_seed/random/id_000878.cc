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
//<ID> 878
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[{\"x\":1},{\"x\":2}]";
    cJSON *parsed = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *created_obj = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON_bool cmp = (cJSON_bool)0;
    int original_size = 0;
    cJSON_bool added_remaining = (cJSON_bool)0;
    cJSON_bool added_detached = (cJSON_bool)0;
    cJSON_bool added_expected = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse(json);

    // step 3: Operate on parsed array
    original_size = cJSON_GetArraySize(parsed);
    detached = cJSON_DetachItemFromArray(parsed, 0);

    // step 4: Configure expected object and compare
    created_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(created_obj, "x", 1.0);
    cmp = cJSON_Compare(detached, created_obj, (cJSON_bool)1);

    // step 5: Assemble root object and attach items for validation and unified cleanup
    root = cJSON_CreateObject();
    added_remaining = cJSON_AddItemToObject(root, "remaining", parsed);
    added_detached = cJSON_AddItemToObject(root, "detached", detached);
    added_expected = cJSON_AddItemToObject(root, "expected", created_obj);
    validation_score = original_size + (int)cmp + (int)added_remaining + (int)added_detached + (int)added_expected + (root != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}