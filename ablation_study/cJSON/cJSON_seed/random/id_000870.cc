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
//<ID> 870
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
    cJSON *root = (cJSON *)0;
    cJSON *obj_item = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool added = (cJSON_bool)0;
    cJSON_bool equal = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse(json);

    // step 3: Operate on parsed array (measure, detach)
    size_before = cJSON_GetArraySize(parsed);
    detached = cJSON_DetachItemFromArray(parsed, 1);
    size_after = cJSON_GetArraySize(parsed);

    // step 4: Configure new object and attach detached item
    root = cJSON_CreateObject();
    added = cJSON_AddItemToObject(root, "extracted", detached);

    // step 5: Validate by retrieving and comparing
    obj_item = cJSON_GetObjectItem(root, "extracted");
    equal = cJSON_Compare(detached, obj_item, (cJSON_bool)1);
    validation_score = (int)(size_before == 3) + (int)(size_after == 2) + (int)added + (int)equal + (root != (cJSON *)0) + (parsed != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}