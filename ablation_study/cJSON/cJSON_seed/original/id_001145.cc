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
//<ID> 1145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names1[] = {"alice", "bob", "carol"};
    cJSON *names_arr = cJSON_CreateStringArray(names1, 3);
    cJSON_AddItemToObject(root, "names", names_arr);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *childObj = cJSON_CreateObject();
    cJSON_AddItemToObject(childObj, "note", cJSON_CreateString("child payload"));
    cJSON *child_ref = cJSON_CreateObjectReference(childObj);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 2: Configure
    const char *names2[] = {"xavier", "yvonne"};
    cJSON *new_names = cJSON_CreateStringArray(names2, 2);
    cJSON *names_ptr = cJSON_GetObjectItem(root, "names");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, names_ptr, new_names);
    (void)replaced;
    cJSON_DeleteItemFromObject(root, "count");

    // step 3: Operate
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}