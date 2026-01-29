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
//<ID> 561
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *static_str = "hello_reference";
    cJSON *array = cJSON_CreateArray();
    cJSON *str_ref = cJSON_CreateStringReference(static_str);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    char *printed = NULL;
    cJSON *retrieved0 = NULL;
    cJSON_bool is_str = 0;
    size_t printed_len = 0;
    int combined_metric = 0;

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(array, str_ref);
    cJSON_AddItemToArray(array, num_item);

    // step 3: Operate / Validate
    retrieved0 = cJSON_GetArrayItem(array, 0);
    is_str = cJSON_IsString(retrieved0);
    printed = cJSON_PrintUnformatted(array);
    printed_len = printed ? strlen(printed) : 0;
    combined_metric = (int)printed_len + (int)is_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}