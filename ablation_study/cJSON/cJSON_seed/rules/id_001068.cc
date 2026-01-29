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
//<ID> 1068
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *shared = NULL;
    cJSON *arr = NULL;
    cJSON *normal = NULL;
    cJSON *got = NULL;
    cJSON *eq_item = NULL;
    char *printed = NULL;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    shared = cJSON_CreateString("shared_value");
    cJSON_AddItemToObject(root, "shared", shared);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    normal = cJSON_CreateString("normal_value");
    cJSON_AddItemToArray(arr, normal);
    cJSON_AddItemReferenceToArray(arr, shared);
    cJSON_AddStringToObject(root, "status", "configured");

    // step 3: Operate / Validate
    got = cJSON_GetArrayItem(arr, 1);
    equal = cJSON_Compare(shared, got, 1);
    eq_item = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(root, "shared_equals_list_item", eq_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}