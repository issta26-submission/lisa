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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON *b_false = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "true_val", b_true);
    cJSON_AddItemToObject(root, "false_val", b_false);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);

    // step 2: Configure
    cJSON_AddItemReferenceToArray(refs, b_true);
    cJSON_AddItemReferenceToArray(refs, b_false);
    cJSON_AddItemReferenceToArray(refs, b_true);

    // step 3: Operate & Validate
    cJSON *got_refs = cJSON_GetObjectItem(root, "refs");
    int size = cJSON_GetArraySize(got_refs);
    cJSON *primary = cJSON_GetObjectItem(root, "true_val");
    cJSON *count = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *primary_dup = cJSON_Duplicate(primary, 1);
    cJSON_AddItemToObject(root, "true_copy", primary_dup);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}