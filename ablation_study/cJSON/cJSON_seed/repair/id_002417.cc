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
//<ID> 2417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("original");
    cJSON_AddItemToArray(items, num_item);
    cJSON_AddItemToArray(items, str_item);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *dup_deep = cJSON_Duplicate(root, 1);
    cJSON *dup_shallow = cJSON_Duplicate(root, 0);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';

    // step 3: Operate
    cJSON *second = cJSON_GetArrayItem(items, 1);
    char *oldstr = cJSON_GetStringValue(second);
    buffer[1] = oldstr ? oldstr[0] : '\0';
    char *newstr = cJSON_SetValuestring(second, "modified");
    buffer[2] = newstr ? newstr[0] : '\0';

    // step 4: Validate and Cleanup
    char *printed_dup = cJSON_PrintUnformatted(dup_deep);
    buffer[3] = printed_dup ? printed_dup[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed_dup);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(dup_deep);
    cJSON_Delete(dup_shallow);
    // API sequence test completed successfully
    return 66;
}