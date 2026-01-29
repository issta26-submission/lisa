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
//<ID> 567
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("hello_ref");
    char *printed = NULL;
    cJSON *retrieved = NULL;
    cJSON_bool added_ref = 0;
    cJSON_bool is_string = 0;
    char *retrieved_str = NULL;
    size_t printed_len = 0;
    size_t copy_len = 0;

    // step 2: Setup / Configure
    added_ref = cJSON_AddItemReferenceToArray(arr, str_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(arr);
    retrieved = cJSON_GetArrayItem(arr, 0);
    is_string = cJSON_IsString(retrieved);
    retrieved_str = cJSON_GetStringValue(retrieved);
    printed_len = printed ? strlen(printed) : 0;
    copy_len = printed_len < 255 ? printed_len : 255;
    memcpy(buffer, printed ? printed : "", copy_len);
    buffer[copy_len] = '\0';
    buffer[255] = retrieved_str ? retrieved_str[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(arr);
    cJSON_Delete(str_item);

    // API sequence test completed successfully
    return 66;
}