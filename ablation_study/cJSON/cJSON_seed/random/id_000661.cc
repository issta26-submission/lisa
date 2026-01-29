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
//<ID> 661
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *obj = (cJSON *)0;
    char *json_str = (char *)0;
    char *moved_val = (char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_obj = 0;
    int nonnull_count = 0;

    // step 2: Initialize
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("one");
    s1 = cJSON_CreateString("two");
    added0 = cJSON_AddItemToArray(arr, s0);
    added1 = cJSON_AddItemToArray(arr, s1);

    // step 3: Configure
    detached = cJSON_DetachItemFromArray(arr, 1);

    // step 4: Operate
    obj = cJSON_CreateObject();
    added_obj = cJSON_AddItemToObject(obj, "moved", detached);
    json_str = cJSON_PrintUnformatted(obj);

    // step 5: Validate
    moved_val = cJSON_GetStringValue(cJSON_GetObjectItem(obj, "moved"));
    nonnull_count = (moved_val != (char *)0) + (json_str != (char *)0);
    (void)added0;
    (void)added1;
    (void)added_obj;
    (void)moved_val;
    (void)nonnull_count;

    // step 6: Cleanup
    cJSON_free((void *)json_str);
    cJSON_Delete(obj);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}