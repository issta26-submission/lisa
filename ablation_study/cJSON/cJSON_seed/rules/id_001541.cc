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
//<ID> 1541
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
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *arr_dup = NULL;
    cJSON *arr_ref = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "enabled");
    arr = cJSON_AddArrayToObject(root, "items");
    s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, s1);
    s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s2);
    arr_dup = cJSON_Duplicate(arr, 1);
    arr_ref = cJSON_CreateArrayReference(arr_dup);
    cJSON_AddItemToObject(root, "items_ref", arr_ref);

    // step 3: Operate / Validate
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}