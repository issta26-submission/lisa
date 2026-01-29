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
//<ID> 662
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *ver_item = cJSON_CreateString(version);
    cJSON *extra_item = cJSON_CreateString("extra_value");
    cJSON_bool add_ok = 0;
    cJSON_bool is_false_flag = 0;
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *false_ptr = cJSON_AddFalseToObject(root, "enabled");
    add_ok = cJSON_AddItemToArray(arr, num_item);
    add_ok = cJSON_AddItemToArray(arr, ver_item);
    add_ok = cJSON_AddItemToArray(arr, extra_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    is_false_flag = cJSON_IsFalse(false_ptr);
    cJSON_DeleteItemFromArray(arr, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}