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
//<ID> 588
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *array = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *obj_child = cJSON_CreateObject();
    cJSON *obj_ref = cJSON_CreateObjectReference(obj_child);
    size_t printed_len = 0;
    size_t to_copy = 0;
    int array_size = 0;
    cJSON *first = NULL;
    cJSON *second = NULL;
    cJSON_bool first_is_true = 0;
    cJSON_bool second_is_object = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(obj_child, "flag", cJSON_CreateTrue());
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemToArray(array, obj_ref);

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(array);
    first = cJSON_GetArrayItem(array, 0);
    second = cJSON_GetArrayItem(array, 1);
    first_is_true = cJSON_IsTrue(first);
    second_is_object = cJSON_IsObject(second);
    printed = cJSON_PrintUnformatted(array);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 127 ? printed_len : 127;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(array);
    cJSON_Delete(obj_child);

    // API sequence test completed successfully
    return 66;
}