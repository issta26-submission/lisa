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
//<ID> 87
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
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(child, "name", name);
    cJSON *dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dup);
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON_bool has_dup = cJSON_HasObjectItem(root, "child_copy");
    cJSON *flag_child = cJSON_CreateNumber((double)has_child);
    cJSON *flag_dup = cJSON_CreateNumber((double)has_dup);
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToArray(flags, flag_child);
    cJSON_AddItemToArray(flags, flag_dup);
    cJSON_AddItemToObject(root, "flags", flags);

    // step 3: Operate & Validate
    char *buffer = (char*)cJSON_malloc(32);
    memset(buffer, 0, 32);
    buffer[0] = 't';
    buffer[1] = 'e';
    buffer[2] = 's';
    buffer[3] = 't';
    buffer[4] = '\0';
    cJSON *bufstr = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "buffered", bufstr);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}