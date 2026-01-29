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
//<ID> 595
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *msg = "hello_cJSON";
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.1415);
    cJSON *num2 = cJSON_CreateNumber(2026.0);
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(192);
    memset(scratch, 0, 192);
    size_t printed_len = 0;
    size_t to_copy = 0;
    int array_size = 0;
    cJSON *first = NULL;
    const char *retrieved_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "greeting", msg);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    retrieved_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 191 ? printed_len : 191;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}