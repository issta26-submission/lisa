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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateBool((cJSON_bool)1);
    cJSON *false_item = cJSON_CreateBool((cJSON_bool)0);
    cJSON_AddItemToObject(root, "true_flag", true_item);
    cJSON_AddItemToObject(root, "false_flag", false_item);
    cJSON_AddItemReferenceToArray(refs, true_item);
    cJSON_AddItemReferenceToArray(refs, false_item);

    // step 3: Operate & Validate
    cJSON *refs_lookup = cJSON_GetObjectItem(root, "refs");
    int refs_count = cJSON_GetArraySize((const cJSON *)refs_lookup);
    cJSON *count_item = cJSON_CreateNumber((double)refs_count);
    cJSON_AddItemToObject(root, "count", count_item);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}