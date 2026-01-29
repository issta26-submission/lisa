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
//<ID> 1082
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
    cJSON *child = NULL;
    cJSON *flagItem = NULL;
    cJSON *gotArr = NULL;
    cJSON *arrChild = NULL;
    cJSON *nameItem = NULL;
    char *printed = NULL;
    char *nameStr = NULL;
    cJSON_bool addRes = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "example");
    addRes = cJSON_AddItemToArray(arr, child);
    flagItem = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flagItem);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    gotArr = cJSON_GetObjectItem(root, "arr");
    arrChild = cJSON_GetArrayItem(gotArr, 0);
    nameItem = cJSON_GetObjectItem(arrChild, "name");
    nameStr = cJSON_GetStringValue(nameItem);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}