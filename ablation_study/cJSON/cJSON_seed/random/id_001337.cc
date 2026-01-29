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
//<ID> 1337
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *f0 = (cJSON *)0;
    cJSON *t1 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *validated_node = (cJSON *)0;
    int is_false0 = 0;
    int is_false1 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    f0 = cJSON_CreateFalse();
    t1 = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_InsertItemInArray(arr, 0, f0);
    cJSON_InsertItemInArray(arr, 1, t1);
    cJSON_AddItemToObject(root, "flags", arr);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "flags");
    elem0 = cJSON_GetArrayItem(retrieved, 0);
    elem1 = cJSON_GetArrayItem(retrieved, 1);
    is_false0 = (int)cJSON_IsFalse(elem0);
    is_false1 = (int)cJSON_IsFalse(elem1);
    validated_node = cJSON_CreateBool(is_false0 && !is_false1);
    cJSON_AddItemToObject(root, "validated", validated_node);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + is_false0 + (int)(validated_node != (cJSON *)0) + ((int)!is_false1);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}