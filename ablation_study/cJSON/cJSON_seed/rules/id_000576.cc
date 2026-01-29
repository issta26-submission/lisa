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
//<ID> 576
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    const char *static_text = "ref_value";
    cJSON *str_ref = cJSON_CreateStringReference(static_text);
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(2026.0);
    char *printed = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *detached = NULL;
    cJSON *first_in_array = NULL;
    char *first_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(child, "num", num_item);
    cJSON_AddItemReferenceToArray(array, str_ref);
    cJSON_AddItemToObject(root, "arr", array);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(buffer, printed ? printed : "", to_copy);
    buffer[to_copy] = '\0';
    first_in_array = cJSON_GetArrayItem(array, 0);
    first_str = cJSON_GetStringValue(first_in_array);
    buffer[511] = first_str && first_str[0] ? first_str[0] : '\0';
    detached = cJSON_DetachItemFromObject(root, "child");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}