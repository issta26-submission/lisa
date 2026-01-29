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
//<ID> 1049
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *maybe_null = cJSON_AddNullToObject(child, "maybe");
    (void)maybe_null;
    cJSON *flag = cJSON_AddFalseToObject(root, "flag");
    (void)flag;

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);
    cJSON *entry = cJSON_CreateString("first");
    cJSON_AddItemToArray(list, entry);

    // step 3: Operate and Validate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    int list_size = cJSON_GetArraySize(got_list);
    cJSON_bool flag_is_false = cJSON_IsFalse(got_flag);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (list_size & 0xF));
    ((char *)scratch)[1] = (char)('0' + (flag_is_false ? 1 : 0));
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}