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
//<ID> 570
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *child_obj = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *name_item = cJSON_CreateString("root_name");
    const char *static_str = "array_ref";
    cJSON *str_ref = cJSON_CreateStringReference(static_str);
    cJSON *array = cJSON_CreateArray();

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "arr", array);
    cJSON_AddItemToObject(root, "child", child_obj);
    cJSON_AddItemToObject(child_obj, "value", num_item);
    cJSON_AddItemReferenceToArray(array, str_ref);
    cJSON_AddItemToArray(array, cJSON_CreateNumber(7.0));

    // step 3: Operate / Validate
    cJSON *detached_child = cJSON_DetachItemFromObject(root, "child");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t copy_len = printed_len < 240 ? printed_len : 240;
    memcpy(scratch, printed ? printed : "", copy_len);
    scratch[copy_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(detached_child);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}