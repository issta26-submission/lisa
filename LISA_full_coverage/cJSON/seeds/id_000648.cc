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
//<ID> 648
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *btrue = cJSON_CreateBool(1);
    cJSON *bfalse = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, btrue);
    cJSON_AddItemToArray(flags, bfalse);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", cJSON_Version());

    // step 3: Operate
    int orig_size = cJSON_GetArraySize(flags);
    cJSON_DeleteItemFromArray(flags, 0);
    int new_size = cJSON_GetArraySize(flags);
    cJSON *first_remaining = cJSON_GetArrayItem(flags, 0);
    cJSON_bool is_bool = cJSON_IsBool(first_remaining);
    cJSON_AddItemToObject(root, "first_remaining_is_bool", cJSON_CreateBool(is_bool));

    // step 4: Validate & Cleanup
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    (void)orig_size;
    (void)new_size;
    (void)first_remaining;
    (void)is_bool;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}