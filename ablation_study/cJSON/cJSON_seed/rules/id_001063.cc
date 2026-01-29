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
//<ID> 1063
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
    cJSON *got_ref = NULL;
    char *printed = NULL;
    cJSON_bool cmp_result = 0;
    cJSON_bool added = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    obj = cJSON_CreateObject();
    cJSON_AddStringToObject(obj, "name", "sample");
    cJSON_AddStringToObject(obj, "type", "demo");
    cJSON_AddItemToObject(root, "original", obj);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", arr);
    added = cJSON_AddItemReferenceToArray(arr, obj);
    cJSON_AddStringToObject(root, "status", "configured");

    // step 3: Operate / Validate
    got_ref = cJSON_GetArrayItem(arr, 0);
    cmp_result = cJSON_Compare(obj, got_ref, 1);
    cJSON_AddItemToObject(root, "comparison_result", cJSON_CreateBool(cmp_result));

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}