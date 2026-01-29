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
//<ID> 569
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *static_text = "fuzz_test_string";
    cJSON *array = cJSON_CreateArray();
    cJSON *str_ref = cJSON_CreateStringReference(static_text);
    cJSON *num_item = cJSON_CreateNumber(2026.0);
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *retrieved = NULL;
    cJSON_bool retrieved_is_string = 0;
    size_t printed_len = 0;
    size_t to_copy = 0;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(array, str_ref);
    cJSON_AddItemToArray(array, num_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(array, 0);
    retrieved_is_string = cJSON_IsString(retrieved);
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

    // API sequence test completed successfully
    return 66;
}