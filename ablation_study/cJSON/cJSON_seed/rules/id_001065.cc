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
//<ID> 1065
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *objA = NULL;
    cJSON *objB = NULL;
    cJSON *target = NULL;
    cJSON *target_copy = NULL;
    cJSON *arr = NULL;
    cJSON_bool equal = 0;
    const char *labels[2] = { "not_equal", "equal" };
    const char *comp_label = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    objA = cJSON_CreateObject();
    cJSON_AddStringToObject(objA, "name", "alpha");
    cJSON_AddItemToObject(root, "A", objA);
    objB = cJSON_CreateObject();
    cJSON_AddStringToObject(objB, "name", "alpha");
    cJSON_AddItemToObject(root, "B", objB);
    target = cJSON_CreateObject();
    cJSON_AddStringToObject(target, "role", "shared");
    cJSON_AddItemToObject(root, "target", target);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    target_copy = cJSON_Duplicate(target, 1);
    cJSON_AddItemToArray(arr, target_copy);

    // step 3: Operate / Validate
    equal = cJSON_Compare(objA, objB, 1);
    comp_label = labels[(equal != 0)];
    cJSON_AddStringToObject(root, "comparison", comp_label);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    return 66;
}