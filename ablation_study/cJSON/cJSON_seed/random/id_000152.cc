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
//<ID> 152
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *t = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *fetched = (cJSON *)0;
    cJSON *num_flag = (cJSON *)0;
    char *json_text = (char *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool fetched_is_string = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    t = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    fetched = cJSON_GetArrayItem(arr, 1);
    fetched_is_string = cJSON_IsString(fetched);
    num_flag = cJSON_CreateNumber((double)fetched_is_string);
    cJSON_AddItemToObject(root, "second_was_string", num_flag);
    detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(root, "detached_item", detached);

    // step 5: Validate
    size_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}