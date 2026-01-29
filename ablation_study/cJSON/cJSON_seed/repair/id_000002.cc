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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *btrue_item = cJSON_CreateTrue();
    cJSON *bfalse_item = cJSON_CreateFalse();
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON *config_bool = NULL;
    cJSON_bool is_first_bool = 0;
    char tmpbuf[32];
    memset(tmpbuf, 0, sizeof(tmpbuf));

    // step 2: Configure
    cJSON_AddItemToObject(root, "array", arr);
    config_bool = cJSON_AddBoolToObject(root, "config", 1);

    // step 3: Operate
    cJSON_AddItemToArray(arr, btrue_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, bfalse_item);
    char *updated = cJSON_SetValuestring(str_item, "updated");

    // step 4: Validate & Cleanup
    is_first_bool = cJSON_IsBool(cJSON_GetArrayItem(arr, 0));
    tmpbuf[0] = updated ? updated[0] : '\0';
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}