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
//<ID> 587
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *child_obj = cJSON_CreateObject();
    cJSON *child_flag = cJSON_CreateTrue();
    cJSON *array = cJSON_CreateArray();
    cJSON *obj_ref = NULL;
    cJSON *true_item = NULL;
    int size = 0;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    int computed = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(child_obj, "flag", child_flag);
    obj_ref = cJSON_CreateObjectReference(child_obj);
    cJSON_AddItemToArray(array, obj_ref);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, true_item);

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(array);
    item0 = cJSON_GetArrayItem(array, 0);
    item1 = cJSON_GetArrayItem(array, 1);
    computed = size + cJSON_IsTrue(item1);

    // step 4: Cleanup
    cJSON_Delete(array);
    // API sequence test completed successfully
    (void)computed;
    (void)item0;
    (void)item1;
    return 66;
}