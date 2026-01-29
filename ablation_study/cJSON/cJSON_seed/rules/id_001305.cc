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
//<ID> 1305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *first_elem = NULL;
    cJSON *second_elem = NULL;
    cJSON *detached = NULL;
    cJSON *obj_key = NULL;
    char *key_string = NULL;
    char *printed_root = NULL;
    char *printed_detached = NULL;
    const char *json_text = "{\"arr\":[true,{\"key\":\"value\"}],\"flag\":true}";
    cJSON_bool is_true_before = 0;
    cJSON_bool is_true_after = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json_text);
    array = cJSON_GetObjectItem(root, "arr");
    first_elem = cJSON_GetArrayItem(array, 0);
    second_elem = cJSON_GetArrayItem(array, 1);

    // step 3: Operate / Validate
    is_true_before = cJSON_IsTrue(first_elem);
    detached = cJSON_DetachItemViaPointer(array, first_elem);
    is_true_after = cJSON_IsTrue(detached);
    obj_key = cJSON_GetObjectItem(second_elem, "key");
    key_string = cJSON_GetStringValue(obj_key);
    printed_root = cJSON_PrintUnformatted(root);
    printed_detached = cJSON_PrintUnformatted(detached);
    (void)key_string;
    (void)is_true_before;
    (void)is_true_after;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    // API sequence test completed successfully
    return 66;
}