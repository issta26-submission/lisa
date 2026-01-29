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
//<ID> 876
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[\"one\",\"two\",\"three\"]";
    cJSON *parsed = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *compare_item = (cJSON *)0;
    cJSON_bool compare_equal = 0;
    int array_size = 0;
    cJSON_bool added_parsed = 0;
    cJSON_bool added_detached = 0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse(json);

    // step 3: Operate on parsed array
    array_size = cJSON_GetArraySize(parsed);
    detached = cJSON_DetachItemFromArray(parsed, 1);

    // step 4: Configure object and comparison
    root = cJSON_CreateObject();
    added_parsed = cJSON_AddItemToObject(root, "list", parsed);
    added_detached = cJSON_AddItemToObject(root, "extracted", detached);
    compare_item = cJSON_CreateString("two");
    compare_equal = cJSON_Compare(compare_item, detached, (cJSON_bool)1);

    // step 5: Validate results
    validation_score = array_size + (int)added_parsed + (int)added_detached + (int)compare_equal + (root != (cJSON *)0) + (detached != (cJSON *)0) + (compare_item != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(compare_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}