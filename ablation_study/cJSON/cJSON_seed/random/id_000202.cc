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
//<ID> 202
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
    cJSON *str_keep = (cJSON *)0;
    cJSON *str_remove = (cJSON *)0;
    cJSON *fetched = (cJSON *)0;
    char *json_text = (char *)0;
    char *fetched_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    str_keep = cJSON_CreateString("keep_me");
    str_remove = cJSON_CreateString("remove_me");
    cJSON_AddItemToArray(arr, str_keep);
    cJSON_AddItemToArray(arr, str_remove);

    // step 4: Operate
    fetched = cJSON_GetArrayItem(arr, 1);
    fetched_text = cJSON_GetStringValue(fetched);
    cJSON_AddStringToObject(root, "selected", fetched_text);
    cJSON_DeleteItemFromObject(root, "items");

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}