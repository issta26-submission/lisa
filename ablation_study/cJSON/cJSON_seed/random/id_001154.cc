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
//<ID> 1154
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
    cJSON *raw_added = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched1 = (cJSON *)0;
    const char *raw_json = "{\"note\":true,\"value\":42}";
    int validation_score = 0;

    // step 2: Initialize - create root object and array container
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create string items and add them to the array
    str1 = cJSON_CreateString("first");
    str2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);

    // step 4: Configure - attach a raw JSON fragment to the root object
    raw_added = cJSON_AddRawToObject(root, "fragment", raw_json);

    // step 5: Operate & Validate - fetch array elements and verify they are strings
    fetched0 = cJSON_GetArrayItem(arr, 0);
    fetched1 = cJSON_GetArrayItem(arr, 1);
    validation_score = (int)cJSON_IsString(fetched0) + (int)cJSON_IsString(fetched1) + (int)(raw_added != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}