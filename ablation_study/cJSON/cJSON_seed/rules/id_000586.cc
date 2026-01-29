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
//<ID> 586
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *child_name = "child_name";
    cJSON *array = cJSON_CreateArray();
    cJSON *child = cJSON_CreateObject();
    cJSON *keep_true = cJSON_CreateTrue();
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    int array_size = 0;
    cJSON *first_item = NULL;
    cJSON *third_item = NULL;
    cJSON_bool first_is_object = 0;
    cJSON_bool third_is_true = 0;
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(child, "key", "value");
    cJSON_AddItemToArray(array, child);
    cJSON_AddItemReferenceToArray(array, child);
    cJSON_AddItemToArray(array, keep_true);

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(array);
    first_item = cJSON_GetArrayItem(array, 0);
    third_item = cJSON_GetArrayItem(array, 2);
    first_is_object = cJSON_IsObject(first_item);
    third_is_true = cJSON_IsTrue(third_item);
    printed = cJSON_PrintUnformatted(array);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 240 ? printed_len : 240;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}