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
//<ID> 574
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    const char *static_text = "fuzz_ref_string";
    cJSON *str_ref = cJSON_CreateStringReference(static_text);
    cJSON *num_item = cJSON_CreateNumber(2026.0);
    cJSON *temp_obj = cJSON_CreateObject();
    cJSON *temp_child = cJSON_CreateString("to_be_detached");

    // step 2: Setup / Configure
    cJSON_AddItemToObject(temp_obj, "inner", temp_child);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddItemToObject(root, "temp", temp_obj);
    cJSON_AddItemReferenceToArray(array, str_ref);
    cJSON_AddItemToArray(array, num_item);

    // step 3: Operate / Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "temp");
    char *printed = cJSON_PrintUnformatted(array);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 240 ? printed_len : 240;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON *first = cJSON_GetArrayItem(array, 0);
    char *retrieved_str = cJSON_GetStringValue(first);
    (void)retrieved_str;
    (void)to_copy;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}