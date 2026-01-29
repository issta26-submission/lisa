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
//<ID> 1061
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
    cJSON *dup = NULL;
    char *printed = NULL;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "child1");
    cJSON_AddItemToObject(root, "child", child);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", arr);
    cJSON_AddStringToObject(root, "status", "initial");

    // step 3: Operate / Validate
    cJSON_AddItemReferenceToArray(arr, child);
    dup = cJSON_Duplicate(root, 1);
    cmp_result = cJSON_Compare(root, dup, 1);
    cJSON_AddStringToObject(dup, "comparison", cmp_result ? "equal" : "not_equal");
    printed = cJSON_PrintUnformatted(dup);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}