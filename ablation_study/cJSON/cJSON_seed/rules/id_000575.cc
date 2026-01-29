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
//<ID> 575
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *static_ref = "fuzz_ref_value";
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateString("inner_value");
    cJSON *array = cJSON_CreateArray();
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *detached = NULL;
    cJSON *retrieved = NULL;
    char *retrieved_str = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "key", child);
    cJSON_AddItemToObject(root, "static_ref", cJSON_CreateStringReference(static_ref));
    detached = cJSON_DetachItemFromObject(root, "key");
    cJSON_AddItemReferenceToArray(array, detached);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(array);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 240 ? printed_len : 240;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    retrieved = cJSON_GetArrayItem(array, 0);
    retrieved_str = cJSON_GetStringValue(retrieved);
    scratch[0] = retrieved_str && retrieved_str[0] ? retrieved_str[0] : scratch[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(array);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}