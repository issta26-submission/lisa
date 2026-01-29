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
//<ID> 590
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *key = "label";
    const char *val = "fuzz_value";
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    size_t printed_len = 0;
    size_t to_copy = 0;
    char first_char = '\0';
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *number_item = cJSON_CreateNumber(2026.0);
    cJSON *child_obj = cJSON_CreateObject();

    // step 2: Setup / Configure
    cJSON_AddStringToObject(child_obj, key, val);
    cJSON_AddItemToArray(array, number_item);
    cJSON_AddItemToArray(array, child_obj);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddStringToObject(root, "summary", "test_sequence");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 127 ? printed_len : 127;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    first_char = printed && printed[0] ? printed[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}