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
//<ID> 943
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
    cJSON *first_str = NULL;
    cJSON *alias_str = NULL;
    char *full_print = NULL;
    char *elem_print = NULL;
    char *scratch = NULL;
    const char *first_text = "alpha";
    const char *alias_text = "beta";
    size_t scratch_len = 64;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    first_str = cJSON_CreateString(first_text);
    cJSON_AddItemToArray(arr, first_str);
    alias_str = cJSON_CreateString(alias_text);
    cJSON_AddItemToObjectCS(root, "alias", alias_str);

    // step 3: Operate / Validate
    full_print = cJSON_Print(root);
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    scratch[0] = full_print[0];
    arr_size = cJSON_GetArraySize(arr);
    scratch[1] = (char)('0' + (arr_size % 10));
    elem_print = cJSON_Print(cJSON_GetArrayItem(arr, 0));

    // step 4: Cleanup
    cJSON_free(full_print);
    cJSON_free(elem_print);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}