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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    size_t buf_size = 64;
    char *buffer = (char*)cJSON_malloc(buf_size);
    memset(buffer, 0, buf_size);
    buffer[0] = 'B';
    buffer[1] = '\0';
    cJSON *buffer_item = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(duplicate, "note", buffer_item);

    // step 3: Operate & Validate
    cJSON_bool has_in_root = cJSON_HasObjectItem(root, "greeting");
    cJSON_bool has_in_dup = cJSON_HasObjectItem(duplicate, "greeting");
    cJSON *flags = cJSON_CreateArray();
    cJSON *f1 = cJSON_CreateNumber((double)has_in_root);
    cJSON *f2 = cJSON_CreateNumber((double)has_in_dup);
    cJSON_AddItemToArray(flags, f1);
    cJSON_AddItemToArray(flags, f2);
    cJSON_AddItemToObject(root, "flags", flags);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}