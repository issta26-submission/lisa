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
//<ID> 84
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

    // step 2: Configure
    cJSON *name = cJSON_CreateString("fuzz-string");
    cJSON_AddItemToObject(child, "name", name);
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "dup_child", dup_child);

    // step 3: Operate & Validate
    cJSON_bool has_name = cJSON_HasObjectItem(child, "name");
    cJSON_bool has_dup = cJSON_HasObjectItem(root, "dup_child");
    void *buffer = cJSON_malloc(16);
    memset(buffer, 0, 16);
    ((char*)buffer)[0] = (char)('0' + has_name);
    ((char*)buffer)[1] = '\0';
    cJSON *alloc_str = cJSON_CreateString((const char *)buffer);
    cJSON_AddItemToObject(root, "alloc", alloc_str);
    (void)has_dup;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}