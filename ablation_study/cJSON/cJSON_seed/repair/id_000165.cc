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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "optional", null_item);
    void *mem = cJSON_malloc((size_t)16);
    memset(mem, 0, (size_t)16);
    char *buf = (char *)mem;
    buf[0] = 'v'; buf[1] = '1'; buf[2] = '\0';
    cJSON *note = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "note", note);

    // step 2: Configure
    cJSON *dup_null = cJSON_Duplicate(null_item, 1);
    cJSON_ReplaceItemViaPointer(config, null_item, dup_null);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);

    // step 3: Operate and Validate
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    cJSON_free(mem);
    // API sequence test completed successfully
    return 66;
}