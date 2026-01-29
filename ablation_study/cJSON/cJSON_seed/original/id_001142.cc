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
//<ID> 1142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names[] = { "alice", "bob", "carol" };
    cJSON *names_arr = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 2: Configure
    cJSON *meta_ref = cJSON_CreateObjectReference(meta);
    cJSON_AddItemToObject(root, "meta_ref", meta_ref);
    cJSON *first_item = cJSON_GetArrayItem(names_arr, 0);
    cJSON *replacement = cJSON_CreateString("dave");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(names_arr, first_item, replacement);
    (void)replaced;

    // step 3: Operate
    cJSON *new_first = cJSON_GetArrayItem(names_arr, 0);
    const char *first_val = cJSON_GetStringValue(new_first);
    double version = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "version"));
    double computed = (double)(first_val[0]) + version;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "meta_ref");
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}