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
//<ID> 673
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *shared_num = cJSON_AddNumberToObject(root, "shared", 5.0);
    cJSON *items = cJSON_CreateArray();

    // step 2: Configure
    cJSON *elem = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, elem);
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(items, shared_num);
    (void)ref_added;
    cJSON_AddItemToObject(root, "items", items);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate & Validate
    double new_val = cJSON_SetNumberHelper(shared_num, 99.5);
    (void)new_val;
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    char *final_print = cJSON_Print(root);
    cJSON_free(final_print);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    (void)version;
    // API sequence test completed successfully
    return 66;
}