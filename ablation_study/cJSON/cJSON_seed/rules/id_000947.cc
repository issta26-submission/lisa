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
//<ID> 947
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *name_item = NULL;
    const char *initial_name = "example_name";
    const char *retrieved_name = NULL;
    int arr_size = 0;
    char *printed = NULL;
    char *scratch = NULL;
    const int scratch_len = 48;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString(initial_name);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObjectCS(root, "name_cs", cJSON_CreateString("cs_value"));
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("second"));

    // step 3: Operate / Validate
    retrieved_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    arr_size = cJSON_GetArraySize(arr);
    printed = cJSON_Print(root);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = retrieved_name[0];
    scratch[1] = (char)('0' + (arr_size % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}