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
//<ID> 1069
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
    cJSON *dup_root = NULL;
    cJSON_bool cmp_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "example");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemReferenceToArray(arr, child);

    // step 3: Operate / Validate
    dup_root = cJSON_Duplicate(root, 1);
    cmp_equal = cJSON_Compare(root, dup_root, 1);

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}