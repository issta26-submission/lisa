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
//<ID> 568
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("example_value");
    char *printed = NULL;
    cJSON *retrieved = NULL;
    cJSON *detached = NULL;
    cJSON_bool is_str = 0;
    const char *retrieved_val = NULL;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(array, str_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(array);
    retrieved = cJSON_GetArrayItem(array, 0);
    is_str = cJSON_IsString(retrieved);
    retrieved_val = cJSON_GetStringValue(retrieved);
    first_char = retrieved_val ? retrieved_val[0] : '\0';

    // step 4: Cleanup
    detached = cJSON_DetachItemViaPointer(array, retrieved);
    cJSON_Delete(array);
    cJSON_Delete(detached);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}