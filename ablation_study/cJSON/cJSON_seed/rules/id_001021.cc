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
//<ID> 1021
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
    cJSON *item = NULL;
    cJSON *new_item = NULL;
    cJSON *dup = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    cJSON_bool flag_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item = cJSON_CreateString("value1");
    cJSON_AddItemToArray(arr, item);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateTrue());

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, 1);
    detached = cJSON_DetachItemFromObject(dup, "items");
    new_item = cJSON_Duplicate(item, 1);
    cJSON_AddItemToArray(arr, new_item);
    flag_true = cJSON_IsTrue(cJSON_GetObjectItem(dup, "flag"));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}