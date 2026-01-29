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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);

    // step 2: Configure
    cJSON *btrue = cJSON_CreateBool(1);
    cJSON *bfalse = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "primary", btrue);
    cJSON_AddItemReferenceToArray(flags, btrue);
    cJSON_AddItemToArray(flags, bfalse);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "label", label);

    // step 3: Operate & Validate
    int flags_count = cJSON_GetArraySize(flags);
    cJSON *count_num = cJSON_CreateNumber((double)flags_count);
    cJSON_AddItemToObject(root, "flags_count", count_num);
    cJSON *primary = cJSON_GetObjectItem(root, "primary");
    cJSON_bool primary_truth = cJSON_IsTrue(primary);
    cJSON *valid = cJSON_CreateBool(primary_truth);
    cJSON_AddItemToObject(root, "primary_valid", valid);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}