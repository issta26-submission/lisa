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
//<ID> 1060
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *arr = NULL;
    cJSON *elem = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    cJSON_bool added_ref = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "original");
    cJSON_AddItemToObject(root, "child", child);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    elem = cJSON_CreateString("element0");
    cJSON_AddItemToArray(arr, elem);
    added_ref = cJSON_AddItemReferenceToArray(arr, child);
    cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate / Validate
    dup = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup, 1);
    printed = cJSON_Print(dup);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}