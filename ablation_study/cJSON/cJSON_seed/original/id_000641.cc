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
//<ID> 641
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *b1 = cJSON_CreateBool(1);
    cJSON *b2 = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, b1);
    cJSON_AddItemToArray(flags, b2);
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));

    // step 3: Operate
    int size_before = cJSON_GetArraySize(flags);
    cJSON_DeleteItemFromArray(flags, 0);
    int size_after = cJSON_GetArraySize(flags);
    cJSON_bool is_array = cJSON_IsArray(flags);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)size_before;
    (void)size_after;
    (void)is_array;
    (void)buffer;

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}