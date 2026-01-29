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
//<ID> 1147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    const char *names[] = {"alpha", "beta", "gamma"};
    cJSON *names_arr = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(child, "names", names_arr);
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "answer", answer);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "root_pi", 3.1415);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 3: Operate
    cJSON *ref_item = cJSON_GetObjectItem(root, "child_ref");
    cJSON *replacement = cJSON_CreateString("replaced_reference");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, ref_item, replacement);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "child");
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    (void)replaced;
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}