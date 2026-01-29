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
//<ID> 1040
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
    cJSON *elem1 = NULL;
    cJSON *elem2 = NULL;
    cJSON *second = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "device42");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    elem1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, elem1);
    elem2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, elem2);
    cJSON_AddItemToArray(arr, cJSON_CreateString("gamma"));

    // step 3: Operate / Validate
    second = cJSON_GetArrayItem(arr, 1);
    dup = cJSON_Duplicate(second, 1);
    cJSON_AddItemToArray(arr, dup);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}