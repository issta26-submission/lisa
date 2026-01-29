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
//<ID> 1029
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *dup = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *detached_name = NULL;
    cJSON *dup_elem = NULL;
    char *printed = NULL;
    cJSON_bool online_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("device42"));
    cJSON_AddItemToObject(root, "online", cJSON_CreateTrue());
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, num_item);
    str_item = cJSON_CreateString("labelA");
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate / Validate
    dup = cJSON_Duplicate(root, 1);
    detached_name = cJSON_DetachItemFromObject(dup, "name");
    cJSON_AddItemToObject(dup, "detached_name", detached_name);
    dup_elem = cJSON_Duplicate(cJSON_GetArrayItem(arr, 1), 0);
    cJSON_AddItemToArray(arr, dup_elem);
    online_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "online"));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}