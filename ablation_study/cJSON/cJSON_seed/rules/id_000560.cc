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
//<ID> 560
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *name_val = "fuzz_name";
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *name_item = cJSON_CreateString(name_val);
    char *printed = NULL;
    cJSON *retrieved = NULL;
    cJSON_bool is_str = 0;
    int combined = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "names", arr);
    cJSON_AddItemToObject(root, "primary", name_item);
    cJSON_AddItemReferenceToArray(arr, name_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(arr, 0);
    is_str = cJSON_IsString(retrieved);
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    memcpy(buffer, printed ? printed : "", printed_len);
    combined = (int)is_str + (int)printed[0];
    buffer[0] = (char)combined;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}