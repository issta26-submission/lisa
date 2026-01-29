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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "flags", flags);

    // step 2: Configure
    cJSON *flag1 = cJSON_CreateBool(1);
    cJSON *flag2 = cJSON_CreateBool(0);
    cJSON_AddItemToObject(data, "flag1", flag1);
    cJSON_AddItemToObject(data, "flag2", flag2);
    cJSON *ref1 = cJSON_GetObjectItem(data, "flag1");
    cJSON *ref2 = cJSON_GetObjectItem(data, "flag2");
    cJSON_AddItemReferenceToArray(flags, ref1);
    cJSON_AddItemReferenceToArray(flags, ref2);

    // step 3: Operate & Validate
    cJSON *data_lookup = cJSON_GetObjectItem(root, "data");
    cJSON *flags_lookup = cJSON_GetObjectItem(data_lookup, "flags");
    int count = cJSON_GetArraySize(flags_lookup);
    cJSON *count_node = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_node);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}