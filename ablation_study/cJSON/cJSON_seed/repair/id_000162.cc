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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", null_item);

    // step 2: Configure
    void *mem = cJSON_malloc(sizeof(int));
    int *pval = (int*)mem;
    *pval = 7;
    cJSON *num = cJSON_CreateNumber((double)(*pval));
    cJSON_AddItemToObject(root, "number", num);

    // step 3: Operate and Validate
    cJSON *dup_null = cJSON_Duplicate(null_item, 0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, null_item, dup_null);
    cJSON_AddBoolToObject(root, "replaced", replaced);
    cJSON *copy = cJSON_Duplicate(root, 1);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_free(mem);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}