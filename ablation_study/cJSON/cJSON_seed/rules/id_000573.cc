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
//<ID> 573
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *static_text = "fuzz_reference_string";
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *str_ref = cJSON_CreateStringReference(static_text);
    cJSON *num_item = cJSON_CreateNumber(2026.0);
    char *printed = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    const char *first_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(array, str_ref);
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToObject(root, "payload", array);

    // step 3: Operate / Validate
    cJSON *detached_array = cJSON_DetachItemFromObject(root, "payload");
    printed = cJSON_PrintUnformatted(detached_array);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    first_str = cJSON_GetStringValue(cJSON_GetArrayItem(detached_array, 0));
    scratch[511] = first_str ? first_str[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(detached_array);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}