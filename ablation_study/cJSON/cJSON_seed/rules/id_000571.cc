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
//<ID> 571
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
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(2026.0);
    const char *static_str = "hello_ref";
    cJSON *str_ref = cJSON_CreateStringReference(static_str);
    char *printed = NULL;
    char *printed_child = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *detached = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "value", num);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemReferenceToArray(arr, str_ref);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(buffer, printed ? printed : "", to_copy);
    buffer[to_copy] = '\0';
    detached = cJSON_DetachItemFromObject(root, "child");
    printed_child = cJSON_PrintUnformatted(detached);
    buffer[0] = printed_child && printed_child[0] ? printed_child[0] : (buffer[0] ? buffer[0] : '\0');

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_child);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}