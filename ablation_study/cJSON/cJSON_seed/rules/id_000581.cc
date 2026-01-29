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
//<ID> 581
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *label = "keep_true";
    cJSON *array = cJSON_CreateArray();
    cJSON *keep_true = cJSON_CreateTrue();
    cJSON *obj_ref = cJSON_CreateObjectReference(keep_true);
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    size_t printed_len = 0;
    size_t to_copy = 0;
    size_t arr_size = 0;
    cJSON *retrieved = NULL;
    cJSON_bool retrieved_is_true = 0;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(array, obj_ref);

    // step 3: Operate / Validate
    arr_size = (size_t)cJSON_GetArraySize(array);
    retrieved = cJSON_GetArrayItem(array, 0);
    retrieved_is_true = cJSON_IsTrue(retrieved);
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
    cJSON_Delete(keep_true);

    // API sequence test completed successfully
    return 66;
}