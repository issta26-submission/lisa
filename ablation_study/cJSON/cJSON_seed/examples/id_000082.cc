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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *src = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "source", src);

    // step 2: Configure
    cJSON *to_dup = cJSON_CreateString("duplicate me");
    cJSON_AddItemToObject(root, "to_dup", to_dup);
    cJSON *dup_item = cJSON_Duplicate(to_dup, 1);
    cJSON_AddItemToObject(root, "duplicated", dup_item);
    char *buffer = (char *)cJSON_malloc(16);
    memset(buffer, 0, 16);
    buffer[0] = 'b';
    buffer[1] = 'u';
    buffer[2] = 'f';
    buffer[3] = 'f';
    buffer[4] = 0;
    cJSON *bufstr = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "bufstr", bufstr);
    cJSON_free(buffer);

    // step 3: Operate & Validate
    cJSON_bool has_source = cJSON_HasObjectItem(root, "source");
    cJSON_bool has_duplicated = cJSON_HasObjectItem(root, "duplicated");
    cJSON *flags = cJSON_CreateArray();
    cJSON *flag1 = cJSON_CreateNumber((double)has_source);
    cJSON *flag2 = cJSON_CreateNumber((double)has_duplicated);
    cJSON_AddItemToArray(flags, flag1);
    cJSON_AddItemToArray(flags, flag2);
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *root_copy = cJSON_Duplicate(root, 1);

    // step 4: Cleanup
    cJSON_Delete(root_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}