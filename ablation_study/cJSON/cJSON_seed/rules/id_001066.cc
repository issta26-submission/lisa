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
//<ID> 1066
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *arr = NULL;
    cJSON *dup_child = NULL;
    char *printed = NULL;
    cJSON_bool cmp_result = 0;
    cJSON_bool add_ref_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "example");
    cJSON_AddStringToObject(child, "role", "tester");
    cJSON_AddItemToObject(root, "child", child);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate / Validate
    add_ref_result = cJSON_AddItemReferenceToArray(arr, child);
    dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "duplicate", dup_child);
    cmp_result = cJSON_Compare(child, dup_child, 1);
    cJSON_AddBoolToObject(root, "children_equal", cmp_result);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
}