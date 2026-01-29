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
//<ID> 1283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *child_a = cJSON_CreateString("alpha");
    cJSON *child_b = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, child_a);
    cJSON_AddItemToArray(items, child_b);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemReferenceToObject(root, "items_ref", items);
    cJSON_AddStringToObject(root, "label", "dataset");
    char *printed = cJSON_Print(root);
    const char *orig = cJSON_GetStringValue(child_a);
    size_t orig_len = orig ? strlen(orig) : 0;
    const char *src = orig ? orig : "";
    char *copy_buf = (char *)cJSON_malloc(orig_len + 1);
    memset(copy_buf, 0, orig_len + 1);
    memcpy(copy_buf, src, orig_len + 1);
    cJSON_AddStringToObject(root, "copied", copy_buf);
    cJSON_free(printed);
    cJSON_free(copy_buf);
    cJSON_Delete(root);
    return 66;
}