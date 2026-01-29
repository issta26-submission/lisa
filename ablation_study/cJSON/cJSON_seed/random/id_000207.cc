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
//<ID> 207
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *fetched_item = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "letters", arr);

    // step 3: Configure
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");

    // step 4: Operate
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    fetched_item = cJSON_GetArrayItem(arr, 1);
    picked = cJSON_CreateString(cJSON_GetStringValue(fetched_item));
    cJSON_AddItemToObject(root, "picked", picked);
    cJSON_DeleteItemFromObject(root, "letters");

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}