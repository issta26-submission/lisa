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
//<ID> 598
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *key = "name";
    const char *val = "fuzz_value";
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    size_t printed_len = 0;
    size_t to_copy = 0;
    int array_size = 0;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, key, val);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(7.0));
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    array_size = cJSON_GetArraySize(arr);
    first_char = printed && printed[0] ? printed[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}