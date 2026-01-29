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
//<ID> 1601
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *nested = NULL;
    cJSON *raw_payload = NULL;
    cJSON *true_flag = NULL;
    cJSON *child_array = NULL;
    cJSON *array_item0 = NULL;
    cJSON *array_item1 = NULL;
    cJSON *detached_payload = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    raw_payload = cJSON_CreateRaw("{\"a\":1,\"b\":2}");
    cJSON_AddItemToObject(nested, "payload", raw_payload);
    true_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(nested, "flag", true_flag);

    // step 3: Operate / Validate
    child_array = cJSON_CreateArray();
    array_item0 = cJSON_CreateString("elem0");
    array_item1 = cJSON_CreateString("elem1");
    cJSON_AddItemToArray(child_array, array_item0);
    cJSON_AddItemToArray(child_array, array_item1);
    cJSON_AddItemToObject(root, "orig_array", child_array);
    cJSON_AddItemReferenceToObject(root, "array_ref", child_array);
    detached_payload = cJSON_DetachItemFromObject(nested, "payload");
    cJSON_AddItemToObject(root, "detached_payload", detached_payload);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}