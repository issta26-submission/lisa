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
//<ID> 1563
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s0 = NULL;
    cJSON *s1 = NULL;
    cJSON *retrieved = NULL;
    cJSON *copied = NULL;
    cJSON *true_item = NULL;
    char *unformatted = NULL;
    const char *strval = NULL;
    cJSON_bool is_str = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    s0 = cJSON_CreateString("hello");
    s1 = cJSON_CreateString("world");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(arr, 1);
    strval = cJSON_GetStringValue(retrieved);
    is_str = cJSON_IsString(retrieved);
    copied = cJSON_CreateString(strval);
    cJSON_AddItemToObject(root, "copied_value", copied);
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}