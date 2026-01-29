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
//<ID> 1605
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
    cJSON *raw = NULL;
    cJSON *tr = NULL;
    cJSON *arr_ref = NULL;
    cJSON *detached = NULL;
    cJSON *nested = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw = cJSON_CreateRaw("{\"payload\":123}");
    tr = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToArray(arr, tr);
    cJSON_AddItemToObject(root, "original", arr);
    /* use the provided API to create a reference entry to the same array */
    cJSON_AddItemReferenceToObject(root, "ref", arr);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromObject(root, "ref");
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(nested, "moved_ref", detached);
    out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}