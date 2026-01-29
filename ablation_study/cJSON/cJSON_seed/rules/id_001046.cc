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
//<ID> 1046
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
    cJSON *elem0 = NULL;
    cJSON *elem1 = NULL;
    cJSON *picked = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "type", "sensor");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    elem0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, elem0);
    elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddStringToObject(root, "status", "active");

    // step 3: Operate / Validate
    picked = cJSON_GetArrayItem(arr, 1);
    printed = cJSON_Print(picked);
    cJSON_AddStringToObject(root, "picked_value", cJSON_GetStringValue(picked));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}