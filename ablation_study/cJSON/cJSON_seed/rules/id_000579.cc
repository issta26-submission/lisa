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
//<ID> 579
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *static_text = "fuzz_ref_value";
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(2026.0);
    cJSON *str_ref = cJSON_CreateStringReference(static_text);
    cJSON *keep_bool = cJSON_CreateTrue();
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    size_t printed_len = 0;
    size_t to_copy = 0;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "keep", keep_bool);
    cJSON_AddItemToObject(root, "count", num_item);
    cJSON_AddItemToObject(root, "detach_me", str_ref);
    cJSON *detached = cJSON_DetachItemFromObject(root, "detach_me");
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(array, detached);
    cJSON_AddItemToArray(array, cJSON_CreateNumber(7.0));

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(array);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 240 ? printed_len : 240;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    first_char = printed && printed[0] ? printed[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(array);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}