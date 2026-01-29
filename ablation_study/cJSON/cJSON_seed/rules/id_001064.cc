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
//<ID> 1064
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *obj = NULL;
    cJSON *arr = NULL;
    cJSON *mirror = NULL;
    cJSON_bool cmp_res = 0;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    obj = cJSON_CreateObject();
    cJSON_AddStringToObject(obj, "name", "child1");
    cJSON_AddItemToObject(root, "child", obj);
    arr = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(arr, obj);
    mirror = cJSON_CreateObject();
    cJSON_AddStringToObject(mirror, "name", "child1");
    cJSON_AddItemToArray(arr, mirror);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate / Validate
    cmp_res = cJSON_Compare(obj, mirror, 1);
    cJSON_AddStringToObject(root, "comparison", cmp_res ? "equal" : "not_equal");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}