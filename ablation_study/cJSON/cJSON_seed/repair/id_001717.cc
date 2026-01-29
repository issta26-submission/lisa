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
//<ID> 1717
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, true_item);
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(flags, num_item);
    cJSON *str_item = cJSON_CreateString("ok");
    cJSON_AddItemToArray(flags, str_item);

    // step 3: Operate and Validate
    cJSON *retr_flags = cJSON_GetObjectItem(root, "flags");
    cJSON_bool is_array = cJSON_IsArray(retr_flags);
    int size = cJSON_GetArraySize(retr_flags);
    cJSON *first = cJSON_GetArrayItem(retr_flags, 0);
    cJSON_bool first_true = cJSON_IsTrue(first);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)is_array);
    buffer[2] = (char)('0' + (size % 10));
    buffer[3] = (char)('0' + (int)first_true);
    (void)buffer;
    (void)retr_flags;
    (void)first;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}