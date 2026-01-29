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
//<ID> 562
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("fuzz_string");
    cJSON *retrieved = NULL;
    char *printed = NULL;
    cJSON_bool is_str = 0;
    size_t printed_len = 0;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(array, str_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(array, 0);
    is_str = cJSON_IsString(retrieved);
    printed = cJSON_PrintUnformatted(array);
    printed_len = printed ? strlen(printed) : 0;
    first_char = printed_len > 0 ? printed[0] : '\0';
    (void)is_str;
    (void)first_char;
    (void)printed_len;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(array);
    cJSON_Delete(str_item);

    // API sequence test completed successfully
    return 66;
}