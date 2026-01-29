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
//<ID> 1149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    const char *names[] = { "alpha", "beta", "gamma" };
    cJSON *name_array = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", name_array);
    cJSON *child_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(child_obj, "inner", "value");
    cJSON_AddItemToObject(root, "child", child_obj);

    cJSON *child_dup = cJSON_Duplicate(child_obj, 1);
    cJSON_AddItemToObject(root, "child_ref", child_dup);
    cJSON *num_item = cJSON_AddNumberToObject(root, "count", 42.0);

    const char *new_names[] = { "one", "two" };
    cJSON *new_array = cJSON_CreateStringArray(new_names, 2);
    cJSON_ReplaceItemViaPointer(root, name_array, new_array);
    cJSON_DeleteItemFromObject(root, "child");

    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    return 66;
}