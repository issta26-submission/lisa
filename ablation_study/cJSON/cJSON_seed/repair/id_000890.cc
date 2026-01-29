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
//<ID> 890
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *ttrue = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, ttrue);
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(flags, num);
    cJSON *str = cJSON_CreateString("ok");
    cJSON_AddItemToArray(flags, str);

    // step 2: Configure
    int buf_len = 256;
    char *buffer = (char*)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(flags);
    cJSON *first_item = cJSON_GetArrayItem(flags, 0);
    cJSON *first_dup = cJSON_Duplicate(first_item, 1);
    cJSON_AddItemToObject(root, "first_dup", first_dup);
    cJSON_AddNumberToObject(root, "array_count", (double)count);
    cJSON_AddNumberToObject(root, "printed_flag", (double)printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}