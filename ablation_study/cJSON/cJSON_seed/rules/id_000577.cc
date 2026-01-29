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
//<ID> 577
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
    cJSON *num = cJSON_CreateNumber(2026.0);
    cJSON *str = cJSON_CreateString("hello_world");
    cJSON_AddItemToObject(child, "number", num);
    cJSON_AddItemToObject(root, "greeting", str);
    cJSON_AddItemToObject(root, "child_obj", child);

    // step 2: Setup / Configure
    cJSON *array = cJSON_CreateArray();
    cJSON *str_ref = cJSON_CreateStringReference("static_ref");
    cJSON_AddItemReferenceToArray(array, str_ref);
    cJSON *detached = cJSON_DetachItemFromObject(root, "child_obj");
    cJSON_AddItemReferenceToArray(array, detached);
    cJSON_AddItemToObject(root, "items", array);

    // step 3: Operate / Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(buffer, printed ? printed : "", to_copy);
    buffer[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}